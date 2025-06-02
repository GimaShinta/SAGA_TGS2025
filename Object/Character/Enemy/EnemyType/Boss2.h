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
	int prev_attack_pattrn = -1; // �O�t���[���̃p�^�[���L�^�p

	std::vector<int> attack_pattrn_num;
	int attack_count = 0;

	bool generate = false;
	bool generate2 = false;
	bool is_alive = true;
	bool is_collision;

	float move_time = 0.0f; // �����p�̎��ԕϐ�
	Vector2D base_position; // �O���̒��S�ʒu
	Vector2D generate_base_position; // �O���̒��S�ʒu

	bool swinging_right = true;      // ���݉E�Ɍ������Ă邩�ǂ���
	float swing_timer = 0.0f;        // �Î~���̃^�C�}�[
	bool is_pausing = false;         // �Î~�����ǂ���

	float image_size = 1.0f;

	int boss2_image[7] = { 0 };

	// ���i�̈ʒu�i���E���ꂼ��2���j
	Vector2D part_positions[6]; // 0:����, 1:����O, 2:�E��O, 3:�E��

	// �N���X Boss2 �Ɉȉ��̕ϐ���ǉ�

	bool has_initialized_swing_x = false;
	float swing_center_x = 0.0f;
	int swing_direction = 1;           // 1 = �E, -1 = ��
	float swing_wait_timer = 0.0f;     // ��~���̃^�C�}�[

	class EnemyShot3* e_shot3;
	class EnemyShot4* e_shot4;
	class EnemyShot5* e_shot5;

public:
	Boss2();
	~Boss2();

public:
	// ����������
	void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delata_second">�P�t���[��������̎���</param>
	void Update(float delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="screen_offset"></param>
	void Draw(const Vector2D& screen_offset)const override;

	// �I��������
	void Finalize() override;

protected:
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="delta_second">�P�t���[��������̎���</param>
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

