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

    // �^�C�g���p�t�H���g�i48px�A�����j�쐬
    m_titleFontHandle = CreateFontToHandle("���C���I", 48, 6);
}


eSceneType TitleScene::Update(float delta_second)
{
    InputManager* input = Singleton<InputManager>::GetInstance();

    // �㉺�L�[ or �R���g���[���\���őI��
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

    // ����i�X�y�[�X or A�{�^���j
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
    // �w�i�O���b�h�i�T�C���g�Ńp���X�ω��j
    int grid_size = 40;
    int pulseColor = 50 + (int)(sinf(GetNowCount() / 60.0f) * 50);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, pulseColor, 100));
    for (int y = 0; y < D_WIN_MAX_Y; y += grid_size)
        DrawLine(0, y, D_WIN_MAX_X, y, GetColor(0, pulseColor, 100));
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // �^�C�g�����S�`��i�����E�O���[���o�j
    const char* title = "== DIGITAL NEXUS ==";
    int titleWidth = GetDrawStringWidthToHandle(title, strlen(title), m_titleFontHandle);
    int titleX = (D_WIN_MAX_X - titleWidth) / 2;
    int glow = 150 + (int)(sinf(GetNowCount() / 50.0f) * 105);
    DrawStringToHandle(titleX, 100, title, GetColor(0, glow, 255), m_titleFontHandle);

    // ���j���[�`��i���������E�I�𒆓_�Ł{�h�炬�j
    const char* menuItems[] = {
        "�Q�[���X�^�[�g",
        "�����L���O",
        "�N���W�b�g",
        "�G���h"
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
            DrawString(x - 30 + offsetX, y, "��", GetColor(255, 255, blink));
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
    // �t�H���g�n���h������i���\�[�X���[�N�΍�j
    DeleteFontToHandle(m_titleFontHandle);
}

eSceneType TitleScene::GetNowSceneType() const
{
    return eSceneType::eTitle;
}
