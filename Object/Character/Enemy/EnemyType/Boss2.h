#pragma once
#include "../EnemyBase.h"
#include "../../Shot/EnemyShot/EnemyShot3.h"
#include "../../Shot/EnemyShot/EnemyShot4.h"
#include "../../Shot/EnemyShot/EnemyShot5.h"

const int MAX_ATTACK_PATTRN = 8;

class Boss2 : public EnemyBase
{
private:
	int attack_pattrn = 0;
	int prev_attack_pattrn = -1; // 前フレームのパターン記録用

	std::vector<int> attack_pattrn_num;
	int attack_count = 0;

	bool generate = false;
	bool generate2 = false;
	bool is_alive = true;
	bool is_collision;

	float move_time = 0.0f; // ∞字用の時間変数
	Vector2D base_position; // 軌道の中心位置
	Vector2D generate_base_position; // 軌道の中心位置

	bool swinging_right = true;      // 現在右に向かってるかどうか
	float swing_timer = 0.0f;        // 静止中のタイマー
	bool is_pausing = false;         // 静止中かどうか

	float image_size = 1.0f;

	int boss2_image[7] = { 0 };

	// 部品の位置（左右それぞれ2個ずつ）
	Vector2D part_positions[6]; // 0:左奥, 1:左手前, 2:右手前, 3:右奥

	// クラス Boss2 に以下の変数を追加

	bool has_initialized_swing_x = false;
	float swing_center_x = 0.0f;
	int swing_direction = 1;           // 1 = 右, -1 = 左
	float swing_wait_timer = 0.0f;     // 停止中のタイマー

	class EnemyShot3* e_shot3;
	class EnemyShot4* e_shot4;
	class EnemyShot5* e_shot5;

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

private:
	void DrawBoss2(const Vector2D position) const;

public:
	int GetAttackPattrn() const;
	int GetIsAlive() const;

	float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}
	float Max(float a, float b)
	{
		return (a > b) ? a : b;
	}

private:
	void Attack(float delta_second);

	void Pattrn4(int bullet_num, float speed, float spiral_interval, float spiral_duration_limit, const Vector2D& generate_location, float delta_second);
	void Pattrn5(float spiral_interval, float spiral_duration_limit, float spiral_speed, const Vector2D& generate_location, float delta_second);
	void Pattrn6(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second);
	void Pattrn7(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second);
	void Pattrn8(float wave_interval, float wave_duration_limit, const Vector2D& generate_location, float delta_second);
	void Pattrn9(int shot_count, float radius, float angular_speed, float bullet_speed, const Vector2D& generate_location, float delta_second);
	void Pattrn10(int shot_count, float radius, float angular_speed, float center_speed, float duration_limit, const Vector2D& center_location, float delta_second);
	void Pattrn11(float offsets_x);
	void Pattrn12();
	void Pattrn13(float delta_second);
};

