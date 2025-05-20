#pragma once
#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

/// <summary>
/// Zako1の行動パターンを定義する列挙型
/// </summary>
enum class Zako1Pattern
{
	MoveStraight, // 真っすぐ進む
	RightMove,    // 右に移動しながら進む
	LeftMove,      // 左に移動しながら進む
	ZIgzag,
	MoveAndStopShoot,   // ★追加
	MoveThenDiagonal    // ★追加
};

/// <summary>
/// 敵キャラクター「Zako1」のクラス。
/// EnemyBase を継承し、独自の移動・攻撃・当たり判定処理を実装する。
/// </summary>
class Zako1 : public EnemyBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Zako1();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Zako1();

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>
	/// <param name="delta_second">前フレームとの経過時間</param>
	void Update(float delta_second) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="screen_offset">画面スクロールのオフセット</param>
	void Draw(const Vector2D& screen_offset) const override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

protected:
	/// <summary>
	/// 弾を撃つ処理
	/// </summary>
	/// <param name="delta_second">前フレームとの経過時間</param>
	void Shot(float delta_second) override;

private:
	Zako1Pattern pattern = Zako1Pattern::MoveStraight;  // 現在の行動パターン
	float pattern_timer = 0.0f;                         // パターン切り替えのためのタイマー
	Vector2D start_location;                            // 出現時の初期位置
	bool is_returning = false;                          // 戻り動作中かどうかのフラグ
	bool on_hit;                                        // 被弾中かどうか
	bool has_shot = false;                              // 弾を撃ったかどうか
	float after_shot_timer = 0.0f; // 弾発射後の待機時間
	bool just_shot = false;                             // 弾を撃った直後の一時フラグ


	//std::vector<int> image_num;

	Vector2D follow_velocity;                           // プレイヤーを追従する際の速度
	float spawn_delay_timer;                            // 出現後の一定時間行動を停止するタイマー

	/// <summary>
	/// 行動パターンをランダムに変更する
	/// </summary>
	void ChangePatternRandomly();

public:
	/// <summary>
	/// 行動パターンを外部から設定する
	/// </summary>
	/// <param name="new_pattern">新しいパターン</param>
	void SetPattern(Zako1Pattern new_pattern);

};
