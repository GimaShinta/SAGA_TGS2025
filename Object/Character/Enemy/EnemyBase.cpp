#include "EnemyBase.h"
#include"../../../Utility/ScoreData.h"
#include "../../Item/Exp/Exp.h"
#include "../../Item/PowerUp/PowerUp.h"
#include "../../Item/Shield/Shield.h"

EnemyBase::EnemyBase() : hp(0.0f), on_hit(false), is_shot(false), shot_timer(0.0f), enemy_type(ENE_NONE)
{
}

EnemyBase::~EnemyBase()
{
}

// ����������
void EnemyBase::Initialize()
{
	EnemyBase::Initialize();
	move_pattern = rand() % 4;  // �����_����0?3�̃p�^�[��
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void EnemyBase::Update(float delta_second)
{
	delta = delta_second;

	// �e�N���X�̍X�V�������Ăяo��
	__super::Update(delta_second);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void EnemyBase::Draw(const Vector2D& screen_offset) const
{
	// �e�N���X�̕`�揈�����Ăяo��
	__super::Draw(screen_offset);
}

// �I��������
void EnemyBase::Finalize()
{
}

/// <summary>
/// �q�b�g������
/// </summary>
/// <param name="hit_object">������������</param>
void EnemyBase::OnHitCollision(GameObjectBase* hit_object)
{
	if (hit_object->GetCollision().object_type == eObjectType::eShot)
	{
		if (on_hit == false)
		{
			hp -= 10;
			on_hit = true;
		}
		else
		{
			on_hit = false;
		}
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBeam)
	{
		beam_damage_timer += delta;

		if (beam_damage_timer >= 0.05f)
		{
			hp -= 10;
			beam_damage_timer = 0;
		}
	}
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
void EnemyBase::Movement(float delta_second)
{
}

void EnemyBase::Shot(float delta_second)
{
}

bool EnemyBase::GetIsShot() const
{
	return this->is_shot;
}

int EnemyBase::GetEnemyType() const
{
	return this->enemy_type;
}

void EnemyBase::SetIsShot()
{
	this->is_shot = false;
}

void EnemyBase::SetPlayer(Player* p)
{
	player = p;
}




void EnemyBase::DropItems()
{
	auto& manager = *Singleton<GameObjectManager>::GetInstance();

	// 100%��Exp�̓h���b�v
	auto exp = manager.CreateObject<Exp>(location);
	exp->SetPlayer(player);

	// 1%��PowerUp�h���b�v
	if (rand() % 100 < 1) {
		manager.CreateObject<PowerUp>(location);
	}

	// 5%��Shield�h���b�v
	if (rand() % 100 < 3) {
		manager.CreateObject<Shield>(location);
	}
}
