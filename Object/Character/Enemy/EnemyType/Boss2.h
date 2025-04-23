#pragma once
#include "../EnemyBase.h"

const int MAX_ATTACK_PATTRN = 8;

class Boss2 : public EnemyBase
{
private:
	int attack_pattrn = 0;
	int prev_attack_pattrn = -1; // �O�t���[���̃p�^�[���L�^�p
	bool generate = false;
	bool generate2 = false;
	bool is_alive = true;
	bool is_collision;

	float move_time = 0.0f; // �����p�̎��ԕϐ�
	Vector2D base_position; // �O���̒��S�ʒu
	Vector2D generate_base_position; // �O���̒��S�ʒu

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

public:
	int GetAttackPattrn() const;
	int GetIsAlive() const;

};

