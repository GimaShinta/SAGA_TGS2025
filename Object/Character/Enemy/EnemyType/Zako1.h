#pragma once
#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

enum class Zako1Pattern
{
	MoveStraight,
	MoveZigzag,
	FollowPlayer,
	BackAndForth,
	LateralSweep,
	DiagonalMove,
	Hovering,
	Kamikaze 
};



class Zako1 :public EnemyBase
{
public:
	Zako1();
	~Zako1();


public:
	void Initialize() override;
	void Update(float delta_second) override;
	void Draw(const Vector2D& screen_offset)const override;
	void Finalize() override;

protected:
	void Shot(float delta_second) override;

private:
	Zako1Pattern pattern = Zako1Pattern::MoveStraight;
	float pattern_timer = 0.0f;
	Vector2D start_location;   // �����ʒu
	bool is_returning = false; // �߂��Ԃ��ǂ���
	bool on_hit;

private:
	Vector2D follow_velocity;
	float spawn_delay_timer; // �o����̑ҋ@����

	void ChangePatternRandomly();


public:
	/// <summary>
	/// �q�b�g������
	/// </summary>
	/// <param name="hit_object">������������</param>
	void OnHitCollision(GameObjectBase* hit_object) override;
	void SetPattern(Zako1Pattern new_pattern);

};

