#include "Shot.h"
#include "../../GameObjectManager.h"
#include "../../../Utility/ProjectConfig.h"

Shot::Shot()
{
}

Shot::~Shot()
{
}

// ����������
void Shot::Initialize()
{
	z_layer = 4;
	velocity.y = -1000.0f;
	box_size = Vector2D(3.0f, 14.0f);

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eShot;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::eEnemy);
	collision.hit_object_type.push_back(eObjectType::eBoss);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	se = rm->GetSounds("Resource/sound/se/shot/shot_01.mp3");
	ChangeVolumeSoundMem(255 * 30 / 100, se);
	PlaySoundMem(se, DX_PLAYTYPE_BACK);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void Shot::Update(float delta_second)
{
	// �e����ʊO�ɏo����폜�i�ړ������ɂ���Ĕ����ς���j
	if ((velocity.y < 0 && location.y < -300.0f) || // �����
		(velocity.y > 0 && location.y > D_WIN_MAX_Y + 300.0f)) // ������
	{
		is_destroy = true; 
	}
	
	if (shot_flip == false)
	{
		velocity.y = -2000.0f;
	}
	else
	{
		velocity.y = 2000.0f;
	}

	location += velocity * delta_second;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Shot::Draw(const Vector2D& screen_offset) const
{
	// �e��`�悷��
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 255), TRUE);
}

// �I��������
void Shot::Finalize()
{
}

/// <summary>
/// �q�b�g������
/// </summary>
/// <param name="hit_object">������������</param>
void Shot::OnHitCollision(GameObjectBase* hit_object)
{
	// �����������肪�e��������
	if (hit_object->GetCollision().object_type == eObjectType::eEnemy)
	{
		is_destroy = true;
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBoss)
	{
		is_destroy = true;
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBoss2)
	{
		is_destroy = true;
	}
}

void Shot::SetShotFlip(bool flip)
{
	shot_flip = flip;
}
