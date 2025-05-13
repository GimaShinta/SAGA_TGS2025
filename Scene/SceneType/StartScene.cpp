#include "StartScene.h"
#include "DxLib.h"
#include <cstring>

StartScene::StartScene() {}
StartScene::~StartScene() {}

void StartScene::Initialize()
{
    // �X�^�[�g���̏�Ԃƃ^�C�}�[�ȂǏ�����
    m_step = Step::WaitForInput;
    m_elapsedTime = 0.0f;
    m_displayCharCount = 0;

    // �\������C���g���e�L�X�g�i2�s�j
    m_introText[0] = "����̓v���g�^�C�vAI�ɑ����ꂽ�C���\�\ �L�����������f�W�^�����E������̊�@�ɕm���Ă���B";
    m_introText[1] = "�N�I����E�C���X��r�����A���������߂��B�N�̐킢���A�ċN���̌��ƂȂ�B";
}


eSceneType StartScene::Update(float delta_second)
{
    InputManager* input = Singleton<InputManager>::GetInstance();
    m_elapsedTime += delta_second; // ���Ԍo�߂����Z

    switch (m_step)
    {
    case Step::WaitForInput:
        // A�L�[ �܂��� XInput A�{�^���ŊJ�n
        if (input->GetKeyDown(KEY_INPUT_A) || input->GetAnyInput(KEY_INPUT_A, XINPUT_BUTTON_A)) {
            m_step = Step::IntroText;
            m_elapsedTime = 0.0f;
            m_displayCharCount = 0;
        }
        break;

    case Step::IntroText:
    {
        // ���������������₵�ĕ\��
        int totalLength = (int)(std::strlen(m_introText[0]) + std::strlen(m_introText[1]));
        if (m_displayCharCount < totalLength) {
            if (m_elapsedTime > 0.04f) {
                m_displayCharCount++;
                m_elapsedTime = 0.0f;
            }
        }
        else {
            // �S���\����̓^�C�g���J�ڏ���
            m_step = Step::GoToTitle;
            m_elapsedTime = 0.0f;
        }

        // Z�L�[�E�X�y�[�X�EA�{�^���ŃX�L�b�v�\��
        if (input->GetKeyDown(KEY_INPUT_Z) || input->GetKeyDown(KEY_INPUT_SPACE) ||
            input->GetButtonDown(XINPUT_BUTTON_A)) {
            m_displayCharCount = totalLength;
            m_step = Step::GoToTitle;
            m_elapsedTime = 0.0f;
        }
        break;
    }

    case Step::GoToTitle:
        // ���Ԍo�߂܂��̓{�^���Ń^�C�g����
        if (m_elapsedTime > 2.0f || input->GetKeyDown(KEY_INPUT_A) || input->GetAnyInput(KEY_INPUT_A, XINPUT_BUTTON_A)) {
            return eSceneType::eTitle;
        }
        break;
    }

    return GetNowSceneType();
}


void StartScene::Draw()
{
    // �w�i�`��i�Â߂̍��F�j
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(10, 10, 30), TRUE);

    switch (m_step)
    {
    case Step::WaitForInput:
        // �_�łŁuPRESS ANY BUTTON�v�\���i���������j
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
        // �C���g���e�L�X�g�𒆉��\���{�m�C�Y�h�炬
        int remain = m_displayCharCount;
        for (int i = 0; i < LINE_COUNT; ++i) {
            int lineLen = (int)std::strlen(m_introText[i]);
            int drawLen = remain > lineLen ? lineLen : remain;

            if (drawLen > 0) {
                char buffer[256] = { 0 };
                strncpy_s(buffer, sizeof(buffer), m_introText[i], drawLen);
                buffer[drawLen] = '\0';

                int textWidth = GetDrawStringWidth(buffer, strlen(buffer));
                int offsetX = (rand() % 3) - 1; // �h�炬
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
