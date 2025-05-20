#include "../EnemyBeam1.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"
#include "../../Enemy/EnemyType/Boss2.h"

EnemyBeam1::EnemyBeam1()
{
}

EnemyBeam1::~EnemyBeam1()
{
}

// ����������
void EnemyBeam1::Initialize()
{
	z_layer = 4;
	box_size = Vector2D(48.0f, 800.0f);

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eEnemyBeam;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::ePlayer);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void EnemyBeam1::Update(float delta_second)
{
	if (boss2 != nullptr)
	{
		location.x = boss2->GetLocation().x;
		location.y = (boss2->GetLocation().y + D_OBJECT_SIZE) + box_size.y;
	}


	location += velocity * delta_second;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void EnemyBeam1::Draw(const Vector2D& screen_offset) const
{
	//�����蔻��̉���
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 255), TRUE);
}

// �I��������
void EnemyBeam1::Finalize()
{
}

void EnemyBeam1::OnHitCollision(GameObjectBase* hit_object)
{
}

void EnemyBeam1::SetBoss2(Boss2* p_boss)
{
	boss2 = p_boss;
}
