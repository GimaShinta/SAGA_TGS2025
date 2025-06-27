#include "CreditScene.h"
#include "DxLib.h"

CreditScene::CreditScene()
    : scrollY(0.0f), scrollSpeed(30.0f)
{
    creditLines = {
        "CREDITS",
        "",
        "CharacterDesign: Minakaze Dots",
        "Stage1.2 / BGM: STUDIO incho3",
        "Stage3 / BGM: Tak_mfk",
        "FinalBoss / BGM: maruya328",
        "SE: Audio Stock",
        "",
        "Special Thanks: You!",
        "",
        "Thank you for playing!",
        "",
        "Press A to return..."
    };
}
CreditScene::~CreditScene()
{
    // �K�v�Ȃ��������������ɏ���
}


void CreditScene::Initialize()
{
    scrollY = D_WIN_MAX_Y;
    font_credits = CreateFontToHandle("���C���I", 28, 6, DX_FONTTYPE_ANTIALIASING);

}

eSceneType CreditScene::Update(float delta)
{
    scrollY -= scrollSpeed * delta;

    InputManager* input = Singleton<InputManager>::GetInstance();
    if (input->GetKeyDown(KEY_INPUT_Z) || input->GetButtonDown(XINPUT_BUTTON_A) || scrollY < -static_cast<float>(creditLines.size() * 40))
    {
        return eSceneType::eTitle;
    }

    return eSceneType::eCredit;
}

void CreditScene::Draw()
{
    SetFontSize(28);
    int cx = D_WIN_MAX_X / 2;

   // extern int font_credits;  // �O���錾�i�ʃt�@�C���Ƀt�H���g�쐬������ꍇ�j

    for (size_t i = 0; i < creditLines.size(); ++i)
    {
        int y = static_cast<int>(scrollY + i * 40);
        int text_w = GetDrawStringWidthToHandle(creditLines[i], strlen(creditLines[i]), font_credits);
        DrawStringToHandle(cx - text_w / 2, y, creditLines[i], GetColor(255, 255, 255), font_credits);
    }

}

void CreditScene::Finalize()
{
    // ����͓��ɂȂ��i�K�v�ɉ����ă��\�[�X����j
}

eSceneType CreditScene::GetNowSceneType() const
{
    return eSceneType::eCredit;
}

