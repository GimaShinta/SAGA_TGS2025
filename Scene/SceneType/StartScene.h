#pragma once
#include "../SceneBase.h"
#include<stdio.h>



class StartScene :  public SceneBase
{
private:
	enum class Step {
		WaitForInput,
		IntroText,
		GoToTitle
	};

	Step m_step;
	float m_elapsedTime;
	int m_displayCharCount;
	static const int LINE_COUNT = 2;
	const char* m_introText[LINE_COUNT]; // 2行分の文字列

public:
	StartScene();
	virtual ~StartScene();

public:
	// 初期化処理
	virtual void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">１フレーム当たりの時間</param>
	/// <returns></returns>
	virtual eSceneType Update(float delta_second) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <returns></returns>
	virtual void Draw() override;

	// 終了時処理（使ったインスタンスの削除とか）
	virtual void Finalize() override;

	// 現在のシーンタイプ（オーバーライド必須）
	virtual eSceneType GetNowSceneType() const override;

};

