#include "TitleScene.h"
 
TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

// ����������
void TitleScene::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
/// <returns></returns>
eSceneType TitleScene::Update(float delta_second)
{
	// ���̓N���X�̃C���X�^���X���擾
	InputManager* input = Singleton<InputManager>::GetInstance();
	// A�{�^���ŃQ�[�����C���֑J��
	if (input->GetKeyDown(KEY_INPUT_SPACE) ||
		input->GetButtonDown(XINPUT_BUTTON_A))
	{
		return eSceneType::eGameMain;
	}

	// ���݂̃V�[�����ێ�����
	return GetNowSceneType();
}

/// <summary>
/// �`�揈��
/// </summary>
/// <returns></returns>
void TitleScene::Draw()
{
	DrawString(0, 0, "�^�C�g��", GetColor(255,255,255));
}

// �I���������i�g�����C���X�^���X�̍폜�Ƃ��j
void TitleScene::Finalize()
{
}

/// <summary>
/// ���݂̃V�[�����
/// </summary>
/// <returns>���݂̓��U���g�V�[���ł�</returns>
eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}
