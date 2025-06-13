#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"

const int FPS = 60; // フレームレート（1秒あたりの更新回数）
const int STAGE_DISTANCE = FPS * 10; // ステージの全長（10秒分）

class Stage1 : public StageBase
{
private:
	// 各種攻撃・敵キャラクターのポインタ定義
	class Shot* shot;               // プレイヤーのショット
	class Beam* beam;               // プレイヤーのビーム攻撃
	class Zako* zako;             // 雑魚敵その1
	class Zako2* zako2;             // 雑魚敵その2
	class Zako4* zako4;             // 雑魚敵その4（1回のみ出現）
	class Stage1Boss* boss1;
	class Boss* boss;               // ボスキャラ1
	class Boss2* boss2;             // ボスキャラ2
	class EnemyShot3* e_shot3;      // 敵のショットタイプ3
	class EnemyShot2* e_shot2;      // 敵のショットタイプ2
	class EnemyShot1* e_shot1;      // 敵のショットタイプ1

	std::vector<EnemyBase*> enemy_list; // ステージ上に存在する敵のリスト
	StageBase* current_stage;           // 現在のステージ（ステージ遷移用）

	float enemy_spawn_timer = 0.0f;     // 敵出現タイマー
	float distance_timer = 0.0f;        // ステージの進行タイマー
	bool zako4_spawned = false;         // Zako4がすでに出現したかどうか

private:
	int distance = 0;          // ステージ終端までの残り距離
	int boss_idx = 0;          // ボスの出現制御インデックス
	int stage = 1;             // 現在のステージ番号
	int score = 0;             // プレイヤーのスコア
	int hisco = 10000;         // ハイスコア
	int no_damage = 0;         // プレイヤー無敵時間管理用フラグ
	int weapon_lev = 1;        // 武器のレベル（弾の数に影響）
	int timer = 0;             // 時間経過用汎用タイマー
	int reach_count = 0;       // ステージ到達判定用カウンター
	int brend = 0;             // ブレンドエフェクト用タイマー
	float warning_timer = 0;   // 警告表示のタイマー
	bool is_warning = false;   // 警告表示中かどうか
	bool is_brend = false;     // ブレンド演出中かどうか
	int bs_attack_pattrn = 0;  // ボスの攻撃パターン制御用インデックス
	float shot_timer;          // ショット発射の間隔制御タイマー

	float stage_timer = 0.0f;  // ステージの経過時間（秒単位）

	float scene_timer = 0.0f;  // 演出や状態遷移用タイマー
	bool boss_spawned = false;
	

	


	static const int TILE_COLS = 8;
	static const int TILE_ROWS = 6;

	bool tile_destroy_flags[TILE_ROWS][TILE_COLS] = { false };
	float tile_destroy_timer = 0.0f;


public:
	// コンストラクタ・デストラクタ
	Stage1(Player* player);     // プレイヤー情報を引数に取るステージ初期化
	~Stage1();                  // デストラクタ

	// 基本的なステージ処理群
	void Initialize() override;              // ステージ初期化処理
	void Finalize() override;                // ステージ終了処理
	void Update(float delta) override;       // 毎フレームの更新処理
	void Draw() override;                    // 描画処理
	bool IsFinished() override;              // ステージが終了したかどうか
	bool IsClear() override;                 // ステージクリア判定
	bool IsOver() override;                  // ゲームオーバー判定
	StageBase* GetNextStage(Player* player) override; // 次のステージを取得



private:
	// ステージ内部処理
	void DisplayWarning(float delta_second); // 警告演出処理
	void EnemyAppearance(float delta);       // 敵の出現処理
	void PlayerShot();

private:
	bool finished = false;                   // ステージ終了フラグ

private:
	int bg_image;	//背景画像
	mutable float grid_scroll_back = 0.0f;
	mutable float grid_scroll_mid = 0.0f;
	bool is_boss_dying = false;
	float boss_death_timer = 0.0f;
	float screen_shake_power = 0.0f;


	int font_digital;

	float scroll_y = 0.0f;

	struct StarParticle {
		Vector2D pos;
		Vector2D velocity;
		float alpha;
		float length;   // 尾の長さ
		float life;
	};

	mutable std::vector<StarParticle> star_particles;


	void DrawScrollBackground() const override; // 背景のスクロール描画

	mutable std::vector<std::pair<Vector2D, float>> ripple_list; // 位置と時間

};
