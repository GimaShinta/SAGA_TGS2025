#pragma once
#include "../EnemyBase.h"

const int MAX_ATTACK_PATTRN = 8;

class Boss2 : public EnemyBase
{
private:
	int attack_pattrn = 0;
	int prev_attack_pattrn = -1; // 前フレームのパターン記録用
	bool generate = false;
	bool generate2 = false;
	bool is_alive = true;
	bool is_collision;

	float move_time = 0.0f; // ∞字用の時間変数
	Vector2D base_position; // 軌道の中心位置
	Vector2D generate_base_position; // 軌道の中心位置

public:
	Boss2();
	~Boss2();

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
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">１フレーム当たりの時間</param>
	void Movement(float delta_second) override;

	void Shot(float delta_second) override;

public:
	int GetAttackPattrn() const;
	int GetIsAlive() const;

};

