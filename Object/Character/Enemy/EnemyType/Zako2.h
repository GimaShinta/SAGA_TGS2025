#pragma once
#include "../EnemyBase.h"
#include"../../Player/Player.h"

// �w�b�_�[�iZako2.h�j�ɒǉ�
enum class Zako2Pattern
{
	Idle,       // �������Ȃ�
	MoveStraight, // �^�������ړ�
	MoveZigzag, // �W�O�U�O�ړ�
	FollowPlayer, // �v���C���[�Ɍ������Ă���
};


class Zako2 :public EnemyBase
{
public:
	Zako2();
	~Zako2();
	class Player* player;


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

	void Shot(float delta_second) override;
	Zako2Pattern pattern = Zako2Pattern::MoveStraight;
	float pattern_timer = 0.0f; // �s���؂�ւ��p�̃^�C�}�[

};

