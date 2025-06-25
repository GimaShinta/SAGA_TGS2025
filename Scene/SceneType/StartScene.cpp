#include "StartScene.h"
#include "DxLib.h"
#include <cstring>

StartScene::StartScene() {}
StartScene::~StartScene() {}

void StartScene::Initialize()
{

   // m_seTypeSound = LoadSoundMem("C:\PG\SAGA_TGS2025\Resource\sound\se\se_effect\tap.mp3"); // �C�ӂ�SE�t�@�C��

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    m_seTypeSound = rm->GetSounds("Resource/sound/se/se_effect/tap.mp3");

    m_nextCharDelay = 0.08f + (GetRand(120) / 1000.0f);  // �� 0.08�`0.2�b


    m_step = Step::WaitForInput;
    m_elapsedTime = 0.0f;
    m_displayCharCount = 0;

    m_fontDigital = CreateFontToHandle("DS-Digital", 28, 6, DX_FONTTYPE_ANTIALIASING);
    m_fontJP = CreateFontToHandle("���C���I", 22, 6, DX_FONTTYPE_ANTIALIASING);


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
        if (input->GetKeyDown(KEY_INPUT_A) || input->GetAnyInput(KEY_INPUT_A, XINPUT_BUTTON_A)) {
            m_step = Step::IntroText;
            m_elapsedTime = 0.0f;
            m_displayCharCount = 0;
        }
        break;

    case Step::IntroText:
    {
        int totalLength = (int)(std::strlen(m_introText[0]) + std::strlen(m_introText[1]));
        if (m_displayCharCount < totalLength)
        {
            if (m_elapsedTime > m_nextCharDelay)
            {
                PlaySoundMem(m_seTypeSound, DX_PLAYTYPE_BACK);
                m_displayCharCount++;
                m_elapsedTime = 0.0f;

                // ���̕����\���܂ł̃f�B���C�i0.08?0.2�b�j
                m_nextCharDelay = 0.08f + (GetRand(120) / 1000.0f);
            }
        }

        else {
            m_step = Step::GoToTitle;
            m_elapsedTime = 0.0f;
        }

        if (input->GetKeyDown(KEY_INPUT_Z) || input->GetKeyDown(KEY_INPUT_SPACE) ||
            input->GetButtonDown(XINPUT_BUTTON_A)) {
            m_displayCharCount = totalLength;
            m_step = Step::GoToTitle;
            m_elapsedTime = 0.0f;
        }
        break;
    }

    case Step::GoToTitle:
        if (m_elapsedTime > 2.0f || input->GetKeyDown(KEY_INPUT_A) || input->GetAnyInput(KEY_INPUT_A, XINPUT_BUTTON_A)) {
            return eSceneType::eTitle;
        }
        break;
    }

    return GetNowSceneType();
}

void StartScene::Draw()
{
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(10, 10, 30), TRUE);

    switch (m_step)
    {
    case Step::WaitForInput:
        if ((int)(m_elapsedTime * 2) % 2 == 0) {
            const char* msg = "PRESS ANY BUTTON";
            int msgWidth = GetDrawStringWidthToHandle(msg, strlen(msg), m_fontDigital);
            int x = (D_WIN_MAX_X - msgWidth) / 2;
            DrawStringToHandle(x, 400, msg, GetColor(255, 255, 255), m_fontJP);
        }
        break;

    case Step::IntroText:
    case Step::GoToTitle:
    {
        int remain = m_displayCharCount;
        for (int i = 0; i < LINE_COUNT; ++i) {
            int lineLen = (int)std::strlen(m_introText[i]);
            int drawLen = remain > lineLen ? lineLen : remain;

            if (drawLen > 0) {
                char buffer[256] = { 0 };
                strncpy_s(buffer, sizeof(buffer), m_introText[i], drawLen);
                buffer[drawLen] = '\0';

                int textWidth = GetDrawStringWidthToHandle(buffer, strlen(buffer), m_fontJP);
                int offsetX = (rand() % 3) - 1;
                int x = (D_WIN_MAX_X - textWidth) / 2 + offsetX;
                int y = 380 + i * 40;
                DrawStringToHandle(x, y, buffer, GetColor(255, 255, 255), m_fontJP);
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
    if (m_fontDigital != -1 &&
        m_fontJP != -1) {
        DeleteFontToHandle(m_fontDigital);
        DeleteFontToHandle(m_fontJP);
        m_fontDigital = -1;
        m_fontJP = -1;

    }

    if (m_seTypeSound != -1) {
        DeleteSoundMem(m_seTypeSound);
        m_seTypeSound = -1;
    }

}

eSceneType StartScene::GetNowSceneType() const { return eSceneType::eStart; }
