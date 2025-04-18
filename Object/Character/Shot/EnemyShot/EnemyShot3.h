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
	// ����������
	virtual void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delata_second">�P�t���[��������̎���</param>
	virtual void Update(float delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="screen_offset"></param>
	virtual void Draw(const Vector2D& screen_offset)const override;

	// �I��������
	virtual void Finalize() override;

	void SetPlayerLocation(const Vector2D& p_location);
	void SetPurposeLocation(const Vector2D& p_location);
	void SetPlayer(class Player* p_player);
	void SetStepShot(int step);
};

