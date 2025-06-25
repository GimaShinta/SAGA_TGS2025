#pragma once
#include "../SceneBase.h"
class TitleScene : public SceneBase
{
private:
	int m_selectedIndex; // 選択中のインデックス
	int m_menuFontHandle;

	int m_logoHandle;
	float m_logoScale; // アニメ用スケール
	int m_logoAlpha;   // アニメ用透明度

	//ロゴの座標調整変数
	int m_logoX;
	int m_logoY;
	int m_logoW;
	int m_logoH;

	float m_logoAppearTimer = 0.0f;
	bool m_logoAppeared = false;

	int logo_se;

	int drawY;
	float m_cursorY; // カーソルY座標



	//選択時状態フラグ
	bool m_startTransitioning = false;  // 出撃演出中フラグ
	float m_transitionTimer = 0.0f;     // 出撃演出タイマー


	float m_menuScale = 1.0f;
	float m_menuAlpha = 255.0f;


	//パーティクルの演出用
	struct Particle {
		float x, y, vy;
		int alpha;
	};
	std::vector<Particle> m_particles;


public:
	TitleScene();
	virtual ~TitleScene();

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


private:
	//描画系関数
	void DrawBackgroundGrid();	//背景
	void DrawLogo();			//ロゴ
	void DrawParticles();		//パーティクル
	void DrawMenu();			//メニュー
};

