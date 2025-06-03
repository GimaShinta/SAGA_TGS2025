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

private:
	int font_digital;
	int font_orbitron;

private:
	int current_bgm_handle = -1;
	int stage_bgm1 = -1; // ステージ1,2の共通BGM
	int stage_bgm3 = -1; // ステージ3用BGM


	// ↓ スコアログ処理用構造体と変数
	struct ScoreLog 
	{
		std::string text;
		float timer;
		float y_offset;
	};

	std::vector<ScoreLog> score_logs;
	float previous_score_count = 0.0f;

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
};
