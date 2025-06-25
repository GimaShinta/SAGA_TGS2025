#pragma once
#include "../SceneBase.h"
#include "../SceneType/Stage/StageBase.h"
#include <vector>
#include <string> 
#include "../../Object/Character/Enemy/EnemyBase.h"

class GameMainScene : public SceneBase
{
private:
	class Player* player;
	StageBase* current_stage;
	float next_scene_timer;
	bool isPaused = false;
private:
	int font_digital;
	int font_orbitron;
	int obi_handle; 

	float background_scroll_x = 0.0f;
private:
	int current_bgm_handle = -1;
	int stage_bgm1 = -1; // ステージ1,2の共通BGM
	int stage_bgm3 = -1; // ステージ3用BGM

	float pause_timer = 0.0f;
	int transparent = 0.0f;

	int se_charge;

	// ↓ スコアログ処理用構造体と変数
	struct ScoreLog 
	{
		std::string text;
		float timer;
		float y_offset;
	};

	std::vector<ScoreLog> score_logs;
	float previous_score_count = 0.0f;

	int m_selectedIndex = 0; // 選択中のインデックス
	bool m_startTransitioning = false;  // 出撃演出中フラグ
	float m_transitionTimer = 0.0f;     // 出撃演出タイマー
	int m_menuFontHandle;
	float line_effect_timer = 0.0f; // ライン演出用の独自タイマー

	int vo = 70;

public:
	GameMainScene();
	virtual ~GameMainScene();

	// 初期化処理
	virtual void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual eSceneType Update(float delta_second) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override;

	// 終了時処理
	virtual void Finalize() override;

	// 現在のシーンタイプ
	virtual eSceneType GetNowSceneType() const override;

	// スコアログ追加関数
	void AddScoreLog(const std::string& text);

	void InputSePlay();
};
