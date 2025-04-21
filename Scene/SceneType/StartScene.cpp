#include "StartScene.h"

StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

void StartScene::Initialize()
{
}

eSceneType StartScene::Update(float delta_second)
{
	// ���̓N���X�̃C���X�^���X���擾
	InputManager* input = Singleton<InputManager>::GetInstance();

	// A�{�^���ŃQ�[�����C���֑J��
	if (input->GetKeyDown(KEY_INPUT_SPACE) ||
		input->GetButtonDown(XINPUT_BUTTON_A))
	{
		return eSceneType::eTitle;
	}

	// ���݂̃V�[�����ێ�����
	return GetNowSceneType();
}

void StartScene::Draw()
{
	DrawString(0, 0, "Start", GetColor(255, 255, 255));
}

void StartScene::Finalize()
{
}

eSceneType StartScene::GetNowSceneType() const
{
	return eSceneType::eStart;
}
