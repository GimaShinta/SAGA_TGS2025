#include "StartScene.h"
#include "DxLib.h"
#include <cstring> // strlen�p

StartScene::StartScene() {}
StartScene::~StartScene() {}

void StartScene::Initialize()
{
    m_step = Step::WaitForInput;
    m_elapsedTime = 0.0f;
    m_displayCharCount = 0;
    m_introText[0] = "����̓v���g�^�C�vAI�ɑ����ꂽ�C���\�\ �L�����������f�W�^�����E������̊�@�ɕm���Ă���B";
    m_introText[1] = "�N�I����E�C���X��r�����A���������߂��B�N�̐킢���A�ċN���̌��ƂȂ�B";
}

eSceneType StartScene::Update(float delta_second)
{
    InputManager* input = Singleton<InputManager>::GetInstance();
    m_elapsedTime += delta_second;

    switch (m_step)
    {
    case Step::WaitForInput:
        if (input->GetKeyDown(KEY_INPUT_A)) { // �C�ӃL�[�Ői�s
            m_step = Step::IntroText;
            m_elapsedTime = 0.0f;
            m_displayCharCount = 0;
        }
        break;

    case Step::IntroText: {
        // 2�s���܂Ƃ߂�1���������₷
        int totalLength = (int)(std::strlen(m_introText[0]) + std::strlen(m_introText[1]));
        if (m_displayCharCount < totalLength) {
            if (m_elapsedTime > 0.04f) {
                m_displayCharCount++;
                m_elapsedTime = 0.0f;
            }
        }
        else {
            m_step = Step::GoToTitle;
            m_elapsedTime = 0.0f;
        }

        // �X�L�b�v����
        if (input->GetKeyDown(KEY_INPUT_Z) || input->GetKeyDown(KEY_INPUT_SPACE) ||
            input->GetButtonDown(XINPUT_BUTTON_A)) {
            m_displayCharCount = totalLength;
            m_step = Step::GoToTitle;
            m_elapsedTime = 0.0f;
        }
        break;
    }

    case Step::GoToTitle:
        if (m_elapsedTime > 2.0f || input->GetKeyDown(KEY_INPUT_A)) {
            return eSceneType::eTitle;
        }
        break;
    }

    return GetNowSceneType();
}

void StartScene::Draw()
{
    switch (m_step)
    {
    case Step::WaitForInput:
        if ((int)(m_elapsedTime * 2) % 2 == 0) {
            DrawString(500, 400, "PRESS ANY BUTTON", GetColor(255, 255, 255));
        }
        break;

    case Step::IntroText:
    case Step::GoToTitle:
    {
        // 2�s�ɕ����Ĉꕶ�����i�K�\��
        int remain = m_displayCharCount;
        for (int i = 0; i < LINE_COUNT; ++i) {
            int lineLen = (int)std::strlen(m_introText[i]);
            int drawLen = remain > lineLen ? lineLen : remain;
            if (drawLen > 0) {
                char buffer[256] = { 0 };
                strncpy_s(buffer, sizeof(buffer), m_introText[i], drawLen);
                buffer[drawLen] = '\0';
                DrawString(100, 380 + i * 40, buffer, GetColor(255, 255, 255));
            }
            remain -= drawLen;
            if (remain <= 0) break;
        }
        break;
    }
    }
}

void StartScene::Finalize()
{
}

eSceneType StartScene::GetNowSceneType() const
{
    return eSceneType::eStart;
}
