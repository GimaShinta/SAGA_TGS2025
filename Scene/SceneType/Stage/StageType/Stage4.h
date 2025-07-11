#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"

class Stage4 :
    public StageBase
{
private:
	class Shot* shot;
	class Beam* beam;
//	class Zako2* zako2;
//	class Boss* boss;
	class Boss3* boss3;
//	class EnemyShot5* e_shot5;
//	class EnemyShot4* e_shot4;
//	class EnemyShot3* e_shot3;
//	class EnemyShot2* e_shot2;
//	class EnemyShot1* e_shot1;
	std::vector<EnemyBase*> enemy_list;
//	StageBase* current_stage;
	int anim_id = 0;
//	std::vector<int> image_handles;
//
private:
	int distance = 0;   // ステージ終端までの距離
//	int boss_idx = 0;    // ボスを代入した配列のインデックス
	int stage = 1;      // ステージ
//	int score = 0;      // スコア
//	int hisco = 10000;  // ハイスコア
//	int no_damage = 0;   // 無敵状態
//	int weapon_lev = 1;   // 自機の武器のレベル（同時に発射される弾数）
//	int timer = 0;      // 時間の進行を管理
	int reach_count = 0;      // 時間の進行を管理
	int brend = 0;      // 時間の進行を管理
	float warning_timer = 0;      // 時間の進行を管理
	float gameover_timer = 0;      // 時間の進行を管理
	int transparent = 0;
	bool is_warning = false;
	bool is_brend = false;
//	int bs_attack_pattrn = 0;
//	float shot_timer;
//
//	float spiral_angle = 0.0f; // スパイラル角度（毎フレーム増加）
//	float spiral_interval = 0.1f; // 発射間隔（秒）
//	float spiral_timer = 0.0f; // 経過時間
//
	float stage_timer = 0.0f; // 経過時間（秒）
	float scene_timer = 0.0f;
//
//	float spiral_duration_timer = 0.0f;
//	bool is_spiral_active = false;
//
	int font_digital;
	int font_orbitron;
	int obi_handle;

	bool draw_animation_first = false;  // デフォルトはfalse
	bool boss3_spawned = false;
public:
	Stage4(Player* player);
	~Stage4();

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
	void EnemyAppearance();
	void EnemyShot(float delta_second);
	void UpdateGameStatus(float delta);
	void ResultDraw(float delta);  // ← 関数プロトタイプ追加
	bool result_fadeout_started = false;
	float result_fadeout_timer = 0.0f;
	bool result_ended = false;
	bool result_started = false;
	float result_timer = 0.0f;
	float total_score = 0.0f;
	bool result_displayed = false;
	float post_result_wait_timer = 0.0f;  // ←これを追加
	float delta_draw = 0.0f;
	float clear_wait_timer = 0.0f;
	bool scored = false;
private:
	bool finished = false;

	/*スクロール*/
	void DrawScrollBackground() const override;
};

