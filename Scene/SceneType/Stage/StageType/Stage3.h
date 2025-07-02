#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"

const int FPS3 = 60; // フレームレート
const int STAGE3_DISTANCE = FPS3 * 5; // ステージの長さ

class Stage3 : public StageBase
{
private:
	class Shot* shot;
	class Beam* beam;
	class Zako2* zako2;
	class Boss* boss;
	class Boss2* boss2;
	class EnemyShot5* e_shot5;
	class EnemyShot4* e_shot4;
	class EnemyShot3* e_shot3;
	class EnemyShot2* e_shot2;
	class EnemyShot1* e_shot1;
	std::vector<EnemyBase*> enemy_list;
	StageBase* current_stage;
	int anim_id = 0;
	std::vector<int> image_handles;

private:
	int distance = 0;   // ステージ終端までの距離
	int boss_idx = 0;    // ボスを代入した配列のインデックス
	int stage = 1;      // ステージ
	int score = 0;      // スコア
	int hisco = 10000;  // ハイスコア
	int no_damage = 0;   // 無敵状態
	int weapon_lev = 1;   // 自機の武器のレベル（同時に発射される弾数）
	int timer = 0;      // 時間の進行を管理
	int reach_count = 0;      // 時間の進行を管理
	int brend = 0;      // 時間の進行を管理
	float warning_timer = 0;      // 時間の進行を管理
	float gameover_timer = 0;      // 時間の進行を管理
	int transparent = 0;
	bool is_warning = false;
	bool is_brend = false;
	int bs_attack_pattrn = 0;
	float shot_timer;

	float spiral_angle = 0.0f; // スパイラル角度（毎フレーム増加）
	float spiral_interval = 0.1f; // 発射間隔（秒）
	float spiral_timer = 0.0f; // 経過時間

	float stage_timer = 0.0f; // 経過時間（秒）
	float scene_timer = 0.0f;

	float spiral_duration_timer = 0.0f;
	bool is_spiral_active = false;

	int font_digital;
	int font_orbitron;
	int obi_handle;

	bool zako3_spawned = false;
	bool boss2_spawned = false;
	float enemy_spawn_timer = 0.0f;
	float boss2_spawn_timer = 0.0f;

	int enemy_group_index = 0; // 偶数: 左、奇数: 右

	float enemy_group_timer = 0.0f;
	bool is_spawning_group = false;

	bool draw_animation_first = false;  // デフォルトはfalse


	bool is_warning_finished = false;

	bool is_warning_active = false;
	float warning_duration = 10.0f;

	float warning_text_x = 1280.0f; // 初期は右端外
	float warning_scroll_speed = 700.0f; // px/秒


	// メンバ変数など
	float band_center_y = 360;          // 画面中央の基準位置
	float band_half_height = 0.0f;      // 帯の半分の高さ（0から最大まで変化）
	const float band_max_half_height = 60.0f; // 帯の最大半分の高さ（つまり高さ120）
	float band_expand_speed = 200.0f;   // 帯が広がる速度(px/s)
	int font_warning;


	enum class WarningState {
		None,       // 無効
		Expanding,  // 帯が広がっていく
		Displaying, // 表示中（テキストスクロール）
		Shrinking   // 帯が縮む
	};

	WarningState warning_state = WarningState::None;

	// Stage3.h に以下のメンバ変数を追加
	bool is_zako7_group_spawned = false;
public:
	Stage3(Player* player);
	~Stage3();

	void Initialize() override;
	void Finalize() override;
	void Update(float delta) override;
	void Draw() override;
	bool IsFinished() override;
	void SetFinished() override;
	bool IsClear() override;
	bool IsOver() override;
	StageBase* GetNextStage(Player* player) override;
	StageBase* GetNowStage() override;

private:
	void DisplayWarning(float delta_second);
	void PlayerShot();
	void EnemyAppearance(float delta);
	void EnemyShot(float delta_second);
	void UpdateGameStatus(float delta);

private:
	bool finished = false;

	/*スクロール*/
	void DrawScrollBackground() const override;
	void ScrollEffectUpdate(float delta);

	void HandleZako1_LR(float delta);
	void HandleZako1_Left(float delta);
	void HandleZako1_Right(float delta);
	void HandleZako1_Center(float delta);
	void HandleZako2_Upward(float delta);
	void SpawnBossAndItems();

	void DrawFrontGrid() const;

	float zako2_spawn_timer = 0.0f;

	void StartWarning();

	//グリッド
	mutable float scroll_back = 0.0f;
	mutable float scroll_front = 0.0f;
	mutable float time = 0.0f;


	struct StarParticle
	{
		Vector2D pos;
		Vector2D velocity;
		float alpha;
		float length;
		float life;
	};

	mutable std::vector<StarParticle> star_particles;


	void ResultDraw(float delta);  // ← 関数プロトタイプ追加

	float result_timer = 0.0f;
	float total_score = 0.0f;
	bool result_displayed = false;

	float clear_wait_timer = 0.0f;
	bool result_started = false;

	float glitch_timer = 0.0f;
	bool glitch_started = false;
	bool glitch_done = false;
	float post_result_wait_timer = 0.0f;  // ←これを追加
	float delta_draw = 0.0f;

	bool result_fadeout_started = false;
	float result_fadeout_timer = 0.0f;
	bool result_ended = false;

	const float black_fade_duration = 60.0f; // 60フレーム（＝1秒）程度

	public:
	int se_warning = -1;       // 警告SEのハンドル
	bool se_warning_played = false; // 一度だけ鳴らす用フラグ
	bool request_play_warning_se = false;  // 警告音を鳴らす要求
	bool request_play_stage4_bgm = false;

	// 左出現管理
	int spawn_index_left = 0;
	float spawn_delay_timer_left = 0.0f;
	bool spawning_zako_left = false;

	// 右出現管理
	int spawn_index_right = 0;
	float spawn_delay_timer_right = 0.0f;
	bool spawning_zako_right = false;

	template <typename T>
	T my_max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

	float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

};