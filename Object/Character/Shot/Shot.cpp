#include "Shot.h"
#include "../../GameObjectManager.h"
#include "../../../Utility/ProjectConfig.h"
#include "../../../Utility/AnimationManager.h"

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
	box_size = Vector2D(5.0f, 14.0f);

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
	image = rm->GetImages("Resource/Image/Object/Player/Shot/anime_effect16.png", 6, 6, 1, 8, 88)[0];

	AnimationManager* am = Singleton<AnimationManager>::GetInstance();
	int anim_id = 0;
	anim_id = am->PlayerAnimation(EffectName::eExprotion, location, 0.05f, false);
	// �A�j���[�V�����̒ǉ��ݒ�
	am->SetAlpha(anim_id, 122);       // ������
	am->SetScale(anim_id, 0.1f);      // 1.5�{�g��

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
	//// �e��`�悷��
	//DrawBox(location.x - box_size.x, location.y - box_size.y,
	//	location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 255), TRUE);

	DrawRotaGraph(location.x, location.y, 1.1f, 0.0f, image, TRUE);

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
	AnimationManager* am = Singleton<AnimationManager>::GetInstance();
	int anim_id = 0;
	// �����������肪�e��������
	if (hit_object->GetCollision().object_type == eObjectType::eEnemy)
	{
		float random_x = static_cast<float>(GetRand(25));
		if (GetRand(2) == 1)
		{
			random_x *= -1;
		}

		anim_id = am->PlayerAnimation(EffectName::eExprotion2, Vector2D(location.x - random_x, location.y), 0.01f, false);
		// �A�j���[�V�����̒ǉ��ݒ�
		am->SetAlpha(anim_id, 255);       // ������
		am->SetScale(anim_id, 0.2f);      // 1.5�{�g��

		is_destroy = true;
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBoss)
	{
		is_destroy = true;
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBoss2)
	{
		float random_x = static_cast<float>(GetRand(100));

		anim_id = am->PlayerAnimation(EffectName::eExprotion2, Vector2D(location.x, location.y - random_x), 0.01f, false);
		// �A�j���[�V�����̒ǉ��ݒ�
		am->SetAlpha(anim_id, 255);       // ������
		am->SetScale(anim_id, 0.2f);      // 1.5�{�g��

		is_destroy = true;
	}
}

void Shot::SetShotFlip(bool flip)
{
	shot_flip = flip;
}
