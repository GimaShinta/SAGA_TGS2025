#include "TitleScene.h"
#include "DxLib.h"
#include <corecrt_math.h>

TitleScene::TitleScene()
    : m_selectedIndex(0)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    m_selectedIndex = 0;

    // タイトル用フォント（48px、太字）作成
    m_titleFontHandle = CreateFontToHandle("メイリオ", 48, 6);
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

    return GetNowSceneType();
}


void TitleScene::Draw()
{
    // 背景グリッド（サイン波でパルス変化）
    int grid_size = 40;
    int pulseColor = 50 + (int)(sinf(GetNowCount() / 60.0f) * 50);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, pulseColor, 100));
    for (int y = 0; y < D_WIN_MAX_Y; y += grid_size)
        DrawLine(0, y, D_WIN_MAX_X, y, GetColor(0, pulseColor, 100));
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // タイトルロゴ描画（中央・グロー演出）
    const char* title = "== DIGITAL NEXUS ==";
    int titleWidth = GetDrawStringWidthToHandle(title, strlen(title), m_titleFontHandle);
    int titleX = (D_WIN_MAX_X - titleWidth) / 2;
    int glow = 150 + (int)(sinf(GetNowCount() / 50.0f) * 105);
    DrawStringToHandle(titleX, 100, title, GetColor(0, glow, 255), m_titleFontHandle);

    // メニュー描画（中央揃え・選択中点滅＋揺らぎ）
    const char* menuItems[] = {
        "ゲームスタート",
        "ランキング",
        "クレジット",
        "エンド"
    };

    for (int i = 0; i < 4; ++i)
    {
        int y = 300 + i * 40;
        int textWidth = GetDrawStringWidth(menuItems[i], strlen(menuItems[i]));
        int x = (D_WIN_MAX_X - textWidth) / 2;

        if (i == m_selectedIndex)
        {
            int blink = (int)((sinf(GetNowCount() / 50.0f) + 1.0f) * 127);
            int offsetX = (rand() % 3) - 1;
            DrawString(x - 30 + offsetX, y, "→", GetColor(255, 255, blink));
            DrawString(x + offsetX, y, menuItems[i], GetColor(255, 255, blink));
        }
        else
        {
            DrawString(x, y, menuItems[i], GetColor(255, 255, 255));
        }
    }
}

void TitleScene::Finalize()
{
    // フォントハンドル解放（リソースリーク対策）
    DeleteFontToHandle(m_titleFontHandle);
}

eSceneType TitleScene::GetNowSceneType() const
{
    return eSceneType::eTitle;
}
