#include "HelpScene.h"

HelpScene::HelpScene()
{
}

HelpScene::~HelpScene()
{
}

// 初期化処理
void HelpScene::Initialize()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">１フレーム当たりの時間</param>
/// <returns></returns>
eSceneType HelpScene::Update(float delta_second)
{
	return GetNowSceneType();
}

/// <summary>
/// 描画処理
/// </summary>
/// <returns></returns>
void HelpScene::Draw()
{
}

// 終了時処理（使ったインスタンスの削除とか）
void HelpScene::Finalize()
{
}

/// <summary>
/// 現在のシーン情報
/// </summary>
/// <returns>現在はリザルトシーンです</returns>
eSceneType HelpScene::GetNowSceneType() const
{
	return eSceneType::eHelp;
}
