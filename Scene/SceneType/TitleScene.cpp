#include "TitleScene.h"
#include "DxLib.h"

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
}

eSceneType TitleScene::Update(float delta_second)
{
    InputManager* input = Singleton<InputManager>::GetInstance();

    // �㉺�L�[�őI��ύX
    if (input->GetKeyDown(KEY_INPUT_UP) || input->GetKeyDown(KEY_INPUT_W))
    {
        m_selectedIndex = (m_selectedIndex + 3) % 4; // ��ցi���[�v�j
    }
    if (input->GetKeyDown(KEY_INPUT_DOWN) || input->GetKeyDown(KEY_INPUT_S))
    {
        m_selectedIndex = (m_selectedIndex + 1) % 4; // ���ցi���[�v�j
    }

    // ����L�[
    if (input->GetKeyDown(KEY_INPUT_SPACE) || input->GetButtonDown(XINPUT_BUTTON_A))
    {
        switch (m_selectedIndex)
        {
        case 0: return eSceneType::eGameMain;
        case 1: return eSceneType::eResult;
        //case 2: return eSceneType::eCredit;
        case 3: return eSceneType::eXit;
        }
    }

    return GetNowSceneType();
}

void TitleScene::Draw()
{
    DrawString(500, 200, "== DIGITAL NEXUS ==", GetColor(0, 255, 255)); // �^�C�g��

    const char* menuItems[] = {
        "�Q�[���X�^�[�g",
        "�����L���O",
        "�N���W�b�g",
        "�G���h"
    };

    for (int i = 0; i < 4; ++i)
    {
        int y = 300 + i * 40;
        if (i == m_selectedIndex)
        {
            DrawString(480, y, "��", GetColor(255, 255, 0));
            DrawString(500, y, menuItems[i], GetColor(255, 255, 0)); // �I�𒆁F���F
        }
        else
        {
            DrawString(500, y, menuItems[i], GetColor(255, 255, 255));
        }
    }
}

void TitleScene::Finalize()
{
    // ���\�[�X�J���Ȃ��i�摜���g�p�j
}

eSceneType TitleScene::GetNowSceneType() const
{
    return eSceneType::eTitle;
}
