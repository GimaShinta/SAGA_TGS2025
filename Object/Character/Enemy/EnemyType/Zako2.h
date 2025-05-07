#pragma once
#include "../EnemyBase.h"
#include"../../Player/Player.h"

// ヘッダー（Zako2.h）に追加
enum class Zako2Pattern
{
	Idle,       // 何もしない
	MoveStraight, // 真っすぐ移動
	MoveZigzag, // ジグザグ移動
	FollowPlayer, // プレイヤーに向かってくる
};


class Zako2 :public EnemyBase
{
public:
	Zako2();
	~Zako2();
	class Player* player;


public:
	// 初期化処理
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delata_second">１フレーム当たりの時間</param>
	void Update(float delta_second) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="screen_offset"></param>
	void Draw(const Vector2D& screen_offset)const override;

	// 終了時処理
	void Finalize() override;

protected:

	void Shot(float delta_second) override;
	Zako2Pattern pattern = Zako2Pattern::MoveStraight;
	float pattern_timer = 0.0f; // 行動切り替え用のタイマー

};

