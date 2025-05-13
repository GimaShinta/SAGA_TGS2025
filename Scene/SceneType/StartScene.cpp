#include "StartScene.h"
#include "DxLib.h"
#include <cstring>

StartScene::StartScene() {}
StartScene::~StartScene() {}

void StartScene::Initialize()
{
    // スタート時の状態とタイマーなど初期化
    m_step = Step::WaitForInput;
    m_elapsedTime = 0.0f;
    m_displayCharCount = 0;

    // 表示するイントロテキスト（2行）
    m_introText[0] = "これはプロトタイプAIに託された任務―― 記憶を失ったデジタル世界が崩壊の危機に瀕している。";
    m_introText[1] = "侵蝕するウイルスを排除し、秩序を取り戻せ。君の戦いが、再起動の鍵となる。";
}


eSceneType StartScene::Update(float delta_second)
{
    InputManager* input = Singleton<InputManager>::GetInstance();
    m_elapsedTime += delta_second; // 時間経過を加算

    switch (m_step)
    {
    case Step::WaitForInput:
        // Aキー または XInput Aボタンで開始
        if (input->GetKeyDown(KEY_INPUT_A) || input->GetAnyInput(KEY_INPUT_A, XINPUT_BUTTON_A)) {
            m_step = Step::IntroText;
            m_elapsedTime = 0.0f;
            m_displayCharCount = 0;
        }
        break;

    case Step::IntroText:
    {
        // 文字を少しずつ増やして表示
        int totalLength = (int)(std::strlen(m_introText[0]) + std::strlen(m_introText[1]));
        if (m_displayCharCount < totalLength) {
            if (m_elapsedTime > 0.04f) {
                m_displayCharCount++;
                m_elapsedTime = 0.0f;
            }
        }
        else {
            // 全文表示後はタイトル遷移準備
            m_step = Step::GoToTitle;
            m_elapsedTime = 0.0f;
        }

        // Zキー・スペース・Aボタンでスキップ表示
        if (input->GetKeyDown(KEY_INPUT_Z) || input->GetKeyDown(KEY_INPUT_SPACE) ||
            input->GetButtonDown(XINPUT_BUTTON_A)) {
            m_displayCharCount = totalLength;
            m_step = Step::GoToTitle;
            m_elapsedTime = 0.0f;
        }
        break;
    }

    case Step::GoToTitle:
        // 時間経過またはボタンでタイトルへ
        if (m_elapsedTime > 2.0f || input->GetKeyDown(KEY_INPUT_A) || input->GetAnyInput(KEY_INPUT_A, XINPUT_BUTTON_A)) {
            return eSceneType::eTitle;
        }
        break;
    }

    return GetNowSceneType();
}


void StartScene::Draw()
{
    // 背景描画（暗めの紺色）
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(10, 10, 30), TRUE);

    switch (m_step)
    {
    case Step::WaitForInput:
        // 点滅で「PRESS ANY BUTTON」表示（中央揃え）
        if ((int)(m_elapsedTime * 2) % 2 == 0) {
            const char* msg = "PRESS ANY BUTTON";
            int msgWidth = GetDrawStringWidth(msg, strlen(msg));
            int x = (D_WIN_MAX_X - msgWidth) / 2;
            DrawString(x, 400, msg, GetColor(255, 255, 255));
        }
        break;

    case Step::IntroText:
    case Step::GoToTitle:
    {
        // イントロテキストを中央表示＋ノイズ揺らぎ
        int remain = m_displayCharCount;
        for (int i = 0; i < LINE_COUNT; ++i) {
            int lineLen = (int)std::strlen(m_introText[i]);
            int drawLen = remain > lineLen ? lineLen : remain;

            if (drawLen > 0) {
                char buffer[256] = { 0 };
                strncpy_s(buffer, sizeof(buffer), m_introText[i], drawLen);
                buffer[drawLen] = '\0';

                int textWidth = GetDrawStringWidth(buffer, strlen(buffer));
                int offsetX = (rand() % 3) - 1; // 揺らぎ
                int x = (D_WIN_MAX_X - textWidth) / 2 + offsetX;
                int y = 380 + i * 40;
                DrawString(x, y, buffer, GetColor(255, 255, 255));
            }

            remain -= drawLen;
            if (remain <= 0) break;
        }
        break;
    }
    }
}

void StartScene::Finalize() {}
eSceneType StartScene::GetNowSceneType() const { return eSceneType::eStart; }
