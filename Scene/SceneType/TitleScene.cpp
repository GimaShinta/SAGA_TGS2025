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
    m_menuFontHandle = CreateFontToHandle("メイリオ", 36, 6); // メニュー専用フォント


    // ロゴ画像読み込み（透過PNG）
    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
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
        p.y = (float)(rand() % 200 + drawY + 50);
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
    if (input->GetKeyDown(KEY_INPUT_SPACE) || input->GetButtonDown(XINPUT_BUTTON_A))
    {
        switch (m_selectedIndex)
        {
        case 0: return eSceneType::eGameMain;
        case 1: return eSceneType::eResult;
        case 3: return eSceneType::eXit;
        }
    }

    // ロゴ演出（ゆっくり縮小＆透明度アップ）
    if (m_logoScale > 0.85f)
        m_logoScale -= delta_second * 0.7f;
    else
        m_logoScale = 0.65f;

    if (m_logoAlpha < 255)
        m_logoAlpha += (int)(delta_second * 1500);  //フェード速度
    if (m_logoAlpha > 255)
        m_logoAlpha = 255;

    for (auto& p : m_particles)
    {
        p.y += p.vy;
        if (p.y < m_logoY)
            p.y = m_logoY + (int)(m_logoH * m_logoScale);
    }

    return GetNowSceneType();
}

void TitleScene::Draw()
{
    DrawBackgroundGrid();
    DrawLogo();
    DrawParticles();
    DrawMenu();
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
            int glow = (int)((sinf(GetNowCount() / 30.0f) + 1.0f) * 127);
            int offsetX = (rand() % 3) - 1;
            int offsetY = (rand() % 3) - 1;

            // アウトライン描画（黒縁）
            DrawStringToHandle(x + offsetX - 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
            DrawStringToHandle(x + offsetX + 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
            DrawStringToHandle(x + offsetX, y + offsetY - 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
            DrawStringToHandle(x + offsetX, y + offsetY + 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);

            // 本体描画（グロー）
            DrawStringToHandle(x + offsetX, y + offsetY, menuItems[i], GetColor(100 + glow, 255, 255), m_menuFontHandle);

            // 三角形の矢印
            int arrowX = x - 50;
            int arrowY = y + offsetY + 10;
            int size = 14;

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
            DrawTriangle(
                arrowX, arrowY - size,
                arrowX, arrowY + size,
                arrowX + size, arrowY,
                GetColor(100 + glow, 255, 255), TRUE
            );
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
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
    int drawY = m_logoY;

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

