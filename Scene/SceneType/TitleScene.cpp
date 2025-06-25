#include "TitleScene.h"
#include "DxLib.h"
#include <corecrt_math.h>

TitleScene::TitleScene()
    : m_selectedIndex(0), m_menuFontHandle(0), m_logoHandle(0), m_logoScale(0.0f), m_logoAlpha(0),
    m_logoX(0), m_logoY(0),m_logoW(0), m_logoH(0),drawY(0)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    m_selectedIndex = 0;
    m_menuFontHandle = CreateFontToHandle("Orbitron", 36, 6); // メニュー専用フォント
    m_cursorY = 370.0f; // 初期Y座標

    m_startTransitioning = false;
    m_transitionTimer = 0.0f;
    m_menuScale = 1.0f;
    m_menuAlpha = 255.0f;

    m_logoAppearTimer = 0.0f;
    m_logoAppeared = false;
    m_logoScale = 1.3f; // ←控えめな拡大率に
    m_logoAlpha = 0;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    logo_se = rm->GetSounds("Resource/sound/se/battle/audiostock_1498491.mp3");

    ChangeVolumeSoundMem(255 * 60 / 100, logo_se);




    // ロゴ画像読み込み（透過PNG）
    m_logoHandle = rm->GetImages("Resource/Image/BackGround/Title/DigitalNexus.png")[0];

    m_logoScale = 1.5f;   // 拡大から始まる
    m_logoAlpha = 0;      // フェードイン開始

    // ロゴサイズ取得
    GetGraphSize(m_logoHandle, &m_logoW, &m_logoH);
    m_logoScale = 0.7f;
    m_logoAlpha = 0;

    // ロゴ表示位置（中央寄せ）
    m_logoX = (D_WIN_MAX_X - (int)(m_logoW * m_logoScale)) / 2;
    m_logoY = -120;

    // 粒子を初期化（ランダムな位置に配置）
    for (int i = 0; i < 30; ++i)
    {
        Particle p;
        p.x = (float)(rand() % D_WIN_MAX_X);
        p.y = (float)(D_WIN_MAX_Y - rand() % 100); // ← 下からランダム
        p.vy = -0.3f - (rand() % 30) / 100.0f;
        p.alpha = 128 + rand() % 128;
        m_particles.push_back(p);
    }

}


eSceneType TitleScene::Update(float delta_second)
{
    InputManager* input = Singleton<InputManager>::GetInstance();

    // 上下キー or コントローラ十字で選択
    if (input->GetKeyDown(KEY_INPUT_UP) || input->GetKeyDown(KEY_INPUT_W) ||
        input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
    {
        m_selectedIndex = (m_selectedIndex + 3) % 4;
    }
    if (input->GetKeyDown(KEY_INPUT_DOWN) || input->GetKeyDown(KEY_INPUT_S) ||
        input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
    {
        m_selectedIndex = (m_selectedIndex + 1) % 4;
    }

    // 決定（スペース or Aボタン）
    if (!m_startTransitioning && (input->GetKeyDown(KEY_INPUT_SPACE) || input->GetButtonDown(XINPUT_BUTTON_A)))
    {
        if (m_selectedIndex == 0) {
            m_startTransitioning = true;
            m_transitionTimer = 0.0f;
        }
        else if (m_selectedIndex == 1) {
            return eSceneType::eRanking;
        }
        else if (m_selectedIndex == 3) {
            return eSceneType::eXit;
        }
    }

    // 出撃演出中のタイマー進行
    if (m_startTransitioning)
    {
        m_transitionTimer += delta_second;

        if (m_transitionTimer >= 2.0f) // 2秒で次のシーンへ
            return eSceneType::eGameMain;
    }


    // ロゴ演出（ゆっくり縮小＆透明度アップ）
    if (!m_logoAppeared)
    {
        m_logoAppearTimer += delta_second;
        float t = m_logoAppearTimer / 1.0f;
        if (t >= 1.0f)
        {
            t = 1.0f;
            m_logoAppeared = true;
        }

        // 1.3 → 0.7 にイージング（緩やかに縮小）
        m_logoScale = 0.7f + (1.3f - 0.7f) * powf(1.0f - t, 2); // EaseOutQuad
        m_logoAlpha = (int)(t * 255.0f);
    }



    // --- カーソル（矢印）のY位置をイージングで追従 ---
    float targetY = 400.0f + m_selectedIndex * 50.0f;
    float diff = targetY - m_cursorY;
    m_cursorY += diff * 0.2f;
    if (fabsf(diff) < 1.0f) m_cursorY = targetY;

    if (m_startTransitioning)
    {
        m_transitionTimer += delta_second;

        // メニュー項目の拡大・消失演出
        m_menuScale += delta_second * 1.0f;   // 拡大
        m_menuAlpha -= delta_second * 300.0f; // フェードアウト
        if (m_menuAlpha < 0.0f) m_menuAlpha = 0.0f;

        if (m_transitionTimer >= 2.0f)
            return eSceneType::eGameMain;
    }

    // パーティクルの更新
    for (auto& p : m_particles)
    {
        p.y += p.vy;
        if (p.y < 0) p.y = D_WIN_MAX_Y; // 上に出たら下から出るようにループ
    }


    return GetNowSceneType();
}

void TitleScene::Draw()
{
    DrawBackgroundGrid();
    DrawLogo();
    DrawParticles();
    DrawMenu();
    if (m_startTransitioning)
    {
        float scale = 0.7f - m_transitionTimer * 0.5f;
        if (scale < 0.0f) scale = 0.0f;
        int alpha = (int)((1.0f - m_transitionTimer / 2.0f) * 255);
        if (alpha < 0) alpha = 0;

        int scaledW = (int)(m_logoW * scale);
        int scaledH = (int)(m_logoH * scale);
        int drawX = (D_WIN_MAX_X - scaledW) / 2;
        int drawY = m_logoY + (int)((m_logoH * 0.7f - scaledH) / 2); // ★中央から吸い込むように！

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawExtendGraph(drawX, drawY, drawX + scaledW, drawY + scaledH, m_logoHandle, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // 暗転フェード
        int fade = (int)(m_transitionTimer * 200.0f);
        if (fade > 255) fade = 255;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);
        DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // CONNECTING 表示
        if (m_transitionTimer > 1.4f)
        {
            const char* msg = "CONNECTING...";
            int msgWidth = GetDrawStringWidthToHandle(msg, strlen(msg), m_menuFontHandle);
            int cx = (D_WIN_MAX_X - msgWidth) / 2;
            int cy = D_WIN_MAX_Y / 2;

            int pulse = 180 + (int)(sinf(GetNowCount() / 10.0f) * 75);
            DrawStringToHandle(cx, cy, msg, GetColor(pulse, pulse, 255), m_menuFontHandle);
        }

    }



}


void TitleScene::Finalize()
{
    // フォントハンドル解放
    DeleteFontToHandle(m_menuFontHandle);

}

eSceneType TitleScene::GetNowSceneType() const
{
    return eSceneType::eTitle;
}

void TitleScene::DrawMenu()
{
    const char* menuItems[] = {
        "START GAME",
        "RANKING",
        "CREDITS",
        "EXIT"
    };

    for (int i = 0; i < 4; ++i)
    {
        int y = 400 + i * 50;
        int textWidth = GetDrawStringWidthToHandle(menuItems[i], strlen(menuItems[i]), m_menuFontHandle);
        int x = (D_WIN_MAX_X - textWidth) / 2;

        if (i == m_selectedIndex)
        {
            // =========================
            // 背景ハイライトバー（画面端まで）
            // =========================
            int barHeight = 40;
            int barAlpha = 120 + (int)(sinf(GetNowCount() / 60.0f) * 50); // パルス
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, barAlpha);
            DrawBox(
                0, y - 5,
                D_WIN_MAX_X, y + barHeight,
                GetColor(0, 200, 255), TRUE
            );

            // =========================
            // 両端のエッジエフェクト（流れる光）
            // =========================
            int edgeWidth = 80;
            int edgeHeight = 4;
            int scrollSpeed = 2;
            int edgeOffset = (GetNowCount() * scrollSpeed) % (D_WIN_MAX_X + edgeWidth * 2);

            // 左から右へ流れる（2個表示してループ感を出す）
            for (int j = 0; j < 2; ++j)
            {
                int edgeX = edgeOffset - edgeWidth * j;
                SetDrawBlendMode(DX_BLENDMODE_ADD, 50);
                DrawBox(
                    edgeX, y + 10,
                    edgeX + edgeWidth, y + 10 + edgeHeight,
                    GetColor(200, 255, 255), TRUE
                );
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }

            // =========================
            // テキスト（アウトライン＋グロー）
            // =========================
            int offsetX = (rand() % 3) - 1;
            int offsetY = (rand() % 3) - 1;

            DrawStringToHandle(x + offsetX - 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
            DrawStringToHandle(x + offsetX + 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
            DrawStringToHandle(x + offsetX, y + offsetY - 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
            DrawStringToHandle(x + offsetX, y + offsetY + 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);

            int glow = (int)((sinf(GetNowCount() / 30.0f) + 1.0f) * 127);
            DrawStringToHandle(x + offsetX, y + offsetY, menuItems[i], GetColor(100 + glow, 255, 255), m_menuFontHandle);
        }
        else
        {
            DrawStringToHandle(x, y, menuItems[i], GetColor(180, 180, 180), m_menuFontHandle);
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        int y = 400 + i * 50;
        int textWidth = GetDrawStringWidthToHandle(menuItems[i], strlen(menuItems[i]), m_menuFontHandle);
        int x = (D_WIN_MAX_X - textWidth) / 2;

        // --- 出撃中の特別演出 ---
        if (m_startTransitioning)
        {
            if (i == m_selectedIndex)
            {
                // 拡大・発光・フェード演出
                float glowScale = m_menuScale;
                int baseTextWidth = GetDrawStringWidthToHandle(menuItems[i], strlen(menuItems[i]), m_menuFontHandle);
                int scaledTextWidth = (int)(baseTextWidth * glowScale);

                int drawX = (D_WIN_MAX_X - scaledTextWidth) / 2;
                int drawY = (int)(400 + i * 50 - (glowScale - 1.0f) * 20); // 少し上に補正

                SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)m_menuAlpha);
                DrawExtendStringToHandle(drawX, drawY, glowScale, glowScale,
                    menuItems[i], GetColor(255, 255, 255), m_menuFontHandle);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }

            continue; // 他メニューは非表示
        }

        // 通常の描画処理（省略せずそのまま）
        if (i == m_selectedIndex)
        {
            int barHeight = 40;
            int barAlpha = 120 + (int)(sinf(GetNowCount() / 60.0f) * 50);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, barAlpha);
            DrawBox(0, y - 5, D_WIN_MAX_X, y + barHeight, GetColor(0, 200, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            int offsetX = (rand() % 3) - 1;
            int offsetY = (rand() % 3) - 1;

            DrawStringToHandle(x + offsetX - 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
            DrawStringToHandle(x + offsetX + 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
            DrawStringToHandle(x + offsetX, y + offsetY - 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
            DrawStringToHandle(x + offsetX, y + offsetY + 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);

            int glow = (int)((sinf(GetNowCount() / 30.0f) + 1.0f) * 127);
            DrawStringToHandle(x + offsetX, y + offsetY, menuItems[i], GetColor(100 + glow, 255, 255), m_menuFontHandle);
        }
        else
        {
            DrawStringToHandle(x, y, menuItems[i], GetColor(180, 180, 180), m_menuFontHandle);
        }
    }

}




void TitleScene::DrawBackgroundGrid()
{
    int grid_size = 40;
    int pulseColor = 50 + (int)(sinf(GetNowCount() / 60.0f) * 50);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, pulseColor, 100));
    for (int y = 0; y < D_WIN_MAX_Y; y += grid_size)
        DrawLine(0, y, D_WIN_MAX_X, y, GetColor(0, pulseColor, 100));
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::DrawLogo()
{
    int scaledW = (int)(m_logoW * m_logoScale);
    int scaledH = (int)(m_logoH * m_logoScale);
    int drawX = (D_WIN_MAX_X - scaledW) / 2;
    int drawY = m_logoY + (int)((m_logoH * 0.7f - scaledH) / 2);

    // 控えめフラッシュ（中央周囲のみ）
    if (!m_logoAppeared)
    {
        //int flashAlpha = (int)((1.0f - m_logoAppearTimer / 1.0f) * 100.0f);
        //SetDrawBlendMode(DX_BLENDMODE_ADD, flashAlpha);
        //DrawBox(drawX - 10, drawY - 10, drawX + scaledW + 10, drawY + scaledH + 10, GetColor(100, 200, 255), TRUE);
        //SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_logoAlpha);
    DrawExtendGraph(drawX, drawY, drawX + scaledW, drawY + scaledH, m_logoHandle, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}



void TitleScene::DrawParticles()
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    for (const auto& p : m_particles)
    {
        DrawCircle((int)p.x, (int)p.y, 2, GetColor(100, 200, 255), TRUE);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

