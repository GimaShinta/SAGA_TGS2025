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
	int stage_bgm4 = -1;
	int se_warning = -1;

	float stage3_bgm_volume = 255.0f;
	bool stage3_bgm_fadeout = false;
	bool stage4_bgm_pending = false;
	float se_warning_timer = 0.0f;


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

	// --- フィルター演出用 ---
	bool effect_shield_on = false;
	bool effect_shield_off = false;
	bool effect_powerup = false;
	float effect_timer = 0.0f;
	const float effect_duration = 0.4f;



	int vo = 70;


	bool black_fade_started = false;
	float black_fade_timer = 0.0f;
	float black_in_timer = 0.0f;
	float black_in_timer2 = 0.0f;
	int alpha = 0;
	// メンバ変数など
	float band_center_y = 360;          // 画面中央の基準位置
	float band_half_height = 0.0f;      // 帯の半分の高さ（0から最大まで変化）
	const float band_max_half_height = 60.0f; // 帯の最大半分の高さ（つまり高さ120）
	float band_expand_speed = 200.0f;   // 帯が広がる速度(px/s)
	int font_warning;
	float warning_text_x = 1280.0f; // 初期は右端外
	float warning_timer = 0.0f;
	float warning_scroll_speed = 700.0f; // px/秒
	float warning_duration = 11.0f;

	bool is_none = false;

	enum class WarningState {
		None,       // 無効
		Expanding,  // 帯が広がっていく
		Displaying, // 表示中（テキストスクロール）
		Shrinking   // 帯が縮む
	};

	WarningState warning_state = WarningState::None;

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

	void DrawUI();

	void InputSePlay();
};
