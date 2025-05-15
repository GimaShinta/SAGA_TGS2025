#pragma once
#include "../CharacterBase.h"
#include <algorithm>

const int BULLET_MAX = 100; // 自機が発射する弾の最大数
const int PLAYER_SHIELD_MAX = 8; // 自機のシールドの最大値
const int WEAPON_LV_MAX = 10; // 武器レベルの最大値
const int PLAYER_SPEED_MAX = 350; // 自機の速さの最大値
const float SHOT_INTERVAL = 0.1f; // 0.25秒ごとに撃つ
const float BEAM_TIME = 5.0f; // 0.25秒ごとに撃つ
const float beam_duration = 5.0f; // 5秒間

enum
{
	LEVEL1 = 1,
	LEVEL2,
	LEVEL3
};

class Player : public CharacterBase
{
private:
	bool is_shot;
	int count_space;
	float shot_timer = 0.0f;
	int life;
	bool on_hit;
	bool beam_on = false;
	bool stop = false;
	bool is_damage;
	bool shot_flip = false;
	int reach_count = 0;
	int brend = 255;
	float beam_timer = 0;
	float damage_timer = 0;
	int powerd = 1;
	bool powerd_on = false;
	float powerd_time = 0.0f;
	bool is_alive = true;

private:
	float charge = 0.0f;
	const float charge_max = 100.0f;
	bool charge_ready = false;


public:
	Player();
	~Player();

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

public:
	/// <summary>
	/// ヒット時処理
	/// </summary>
	/// <param name="hit_object">当たった相手</param>
	void OnHitCollision(GameObjectBase* hit_object) override;

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">１フレーム当たりの時間</param>
	void Movement(float delta_second) override;

	void Shot(float delta_second);

	void Damage(float delta_second);

public:
	bool GetIsShot() const;
	bool GetIsAlive() const;
	bool GetBeamOn() const;
	bool GetShotFlip() const;
	void SetIsShot();
	void SetBeamOn();

public:
	void AddCharge(float value);
	bool CanUseSpecial() const;
	void UseSpecial();
	float GetChargeRate() const;

};

