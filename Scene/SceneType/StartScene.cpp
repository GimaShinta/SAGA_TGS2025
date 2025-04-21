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
	// 入力クラスのインスタンスを取得
	InputManager* input = Singleton<InputManager>::GetInstance();

	// Aボタンでゲームメインへ遷移
	if (input->GetKeyDown(KEY_INPUT_SPACE) ||
		input->GetButtonDown(XINPUT_BUTTON_A))
	{
		return eSceneType::eTitle;
	}

	// 現在のシーンを維持する
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
