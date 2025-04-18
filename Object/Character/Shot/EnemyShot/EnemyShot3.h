#pragma once
#include "EnemyShotBase.h"
class EnemyShot3 :
	public EnemyShotBase
{
private:
	class Player* player;
	float shot_timer;
	float shot_speed = 400;
	bool player_target;
	Vector2D player_location;
	Vector2D purpose_location;
	int step_shot;
	bool is_homing = true;
	bool has_fired_step2 = false;

public:
	EnemyShot3();
	~EnemyShot3();

public:
	// 初期化処理
	virtual void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delata_second">１フレーム当たりの時間</param>
	virtual void Update(float delta_second) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="screen_offset"></param>
	virtual void Draw(const Vector2D& screen_offset)const override;

	// 終了時処理
	virtual void Finalize() override;

	void SetPlayerLocation(const Vector2D& p_location);
	void SetPurposeLocation(const Vector2D& p_location);
	void SetPlayer(class Player* p_player);
	void SetStepShot(int step);
};

