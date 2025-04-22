#include "ResultScene.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

// 初期化処理
void ResultScene::Initialize()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">１フレーム当たりの時間</param>
/// <returns></returns>
eSceneType ResultScene::Update(float delta_second)
{
	return GetNowSceneType();
}

/// <summary>
/// 描画処理
/// </summary>
/// <returns></returns>
void ResultScene::Draw()
{
	DrawString(0, 0, "Result", GetColor(255, 255, 255));
}

// 終了時処理（使ったインスタンスの削除とか）
void ResultScene::Finalize()
{
}

/// <summary>
/// 現在のシーン情報
/// </summary>
/// <returns>現在はリザルトシーンです</returns>
eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::eResult;
}
