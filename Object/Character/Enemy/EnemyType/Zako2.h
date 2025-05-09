#pragma once
#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

enum class Zako2Pattern
{
	Idle,
	MoveStraight,
	MoveZigzag,
	FollowPlayer,
	BackAndForth,
	LateralSweep,
	DiagonalMove,
	Hovering,
	Kamikaze // ← 追加！
};




class Zako2 :public EnemyBase
{
public:
	Zako2();
	~Zako2();


public:
	void Initialize() override;
	void Update(float delta_second) override;
	void Draw(const Vector2D& screen_offset)const override;
	void Finalize() override;

protected:
	void Shot(float delta_second) override;

private:
	Zako2Pattern pattern = Zako2Pattern::MoveStraight;
	float pattern_timer = 0.0f;
	Vector2D start_location;   // 初期位置
	bool is_returning = false; // 戻る状態かどうか
	bool on_hit;

private:
	Vector2D follow_velocity;
	float spawn_delay_timer; // 出現後の待機時間

	void ChangePatternRandomly(); 



};

