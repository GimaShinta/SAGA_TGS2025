#pragma once
#include "../CharacterBase.h"
#include "../../../Object/GameObjectManager.h"
#include <unordered_set>

const int ENEMY_MAX = 100; // �G�@�̐��̍ő�l

enum
{
	ENE_NONE,
	ENE_ZAKO1,
	ENE_ZAKO2,
	ENE_ZAKO3,
	ENE_BOSS,
	ENE_BOSS2,
};

class EnemyBase : public CharacterBase
{
protected:
	float hp;
	bool on_hit;
	int enemy_type;

protected:
	bool is_shot;
	float shot_timer;
	float delta = 0.0f;
	float beam_damage_timer = 0.0f;

public:
	EnemyBase();
	~EnemyBase();

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

public:
	/// <summary>
	/// �q�b�g������
	/// </summary>
	/// <param name="hit_object">������������</param>
	virtual void OnHitCollision(GameObjectBase* hit_object) override;

protected:
	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="delta_second">�P�t���[��������̎���</param>
	virtual void Movement(float delta_second) override;

	virtual void Shot(float delta_second);

public:
	bool GetIsShot() const;
	int GetEnemyType() const;
	void SetIsShot();

};

