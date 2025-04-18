#include "TitleScene.h"
 
TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

// 初期化処理
void TitleScene::Initialize()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">１フレーム当たりの時間</param>
/// <returns></returns>
eSceneType TitleScene::Update(float delta_second)
{
	// 入力クラスのインスタンスを取得
	InputManager* input = Singleton<InputManager>::GetInstance();
	// Aボタンでゲームメインへ遷移
	if (input->GetKeyDown(KEY_INPUT_SPACE) ||
		input->GetButtonDown(XINPUT_BUTTON_A))
	{
		return eSceneType::eGameMain;
	}

	// 現在のシーンを維持する
	return GetNowSceneType();
}

/// <summary>
/// 描画処理
/// </summary>
/// <returns></returns>
void TitleScene::Draw()
{
	DrawString(0, 0, "タイトル", GetColor(255,255,255));
}

// 終了時処理（使ったインスタンスの削除とか）
void TitleScene::Finalize()
{
}

/// <summary>
/// 現在のシーン情報
/// </summary>
/// <returns>現在はリザルトシーンです</returns>
eSceneType TitleScene::GetNowSceneType() const
{
	return eSceneType::eTitle;
}
