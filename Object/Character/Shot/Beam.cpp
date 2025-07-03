#include "Beam.h"
#include "../../GameObjectManager.h"
#include "../../../Utility/ProjectConfig.h"
#include "../../../Utility/AnimationManager.h"
#include "../Player/Player.h"

Beam::Beam() : player(nullptr)
{
}

Beam::~Beam()
{
}

// ����������
void Beam::Initialize()
{
	z_layer = 2;
	box_size = Vector2D(60.0f, 800.0f);

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eBeam;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::eEnemy);
	collision.hit_object_type.push_back(eObjectType::eBoss);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	beam_ts = rm->GetImages("Resource/Image/Object/Player/Beam/anime_sp_weapon03_1.png", 12, 2, 6, 88, 128);
	beam_t = beam_ts[10];
	beam_bs = rm->GetImages("Resource/Image/Object/Player/Beam/anime_sp_weapon03_2.png", 12, 2, 6, 88, 80);
	beam_b = beam_bs[10];

	beam_state = BeamState::Charging;
	charge_time = 0.0f;

	se = rm->GetSounds("Resource/sound/se/effect/audiostock_1244545.mp3");
	ChangeVolumeSoundMem(255 * 100 / 100, se);
	PlaySoundMem(se, DX_PLAYTYPE_BACK);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void Beam::Update(float delta_second)
{
	switch (beam_state)
	{
	case BeamState::Charging:
		charge_time += delta_second;

		if (beam_state == BeamState::Charging && player != nullptr && charge_on == false)
		{
			//AnimationManager* am = Singleton<AnimationManager>::GetInstance();
			//am->PlayerAnimation(EffectName::eExprotion2, player->GetLocation(), 0.1f, false);
			//charge_on = true;
		}

		if (charge_time >= charge_duration)
		{
			beam_state = BeamState::Firing;
		}
		return; // �`���[�W���̓r�[���̈ʒu�Ȃǂ��X�V���Ȃ�
	case BeamState::Firing:
		break; // �� �����̏�����
	default:
		return;
	}

	if (player != nullptr)
	{
		location.x = player->GetLocation().x;
		if (Beam_flip == false)
		{
			location.y = (player->GetLocation().y - (D_OBJECT_SIZE + 12.0f)) - box_size.y;
		}
		else
		{
			location.y = (player->GetLocation().y + D_OBJECT_SIZE) + box_size.y;
		}
	}

	destroy_time += delta_second;

	if (destroy_time >= 5.0f)
	{
		is_destroy = true;
	}

	std::vector<int> animation_num = { 10,11 };
	//�t���[�����[�g�Ŏ��Ԃ��v��
	animation_time += delta_second;
	//8�b�o������摜��؂�ւ���
	if (animation_time >= 0.03f)
	{
		//�v�����Ԃ̏�����
		animation_time = 0.0f;
		//���Ԍo�߃J�E���g�̑���
		animation_count++;
		//�J�E���g���A�j���[�V�����摜�̗v�f���ȏ�ɂȂ�����
		if (animation_count >= animation_num.size())
		{
			//�J�E���g�̏�����
			animation_count = 0;
		}
		// �A�j���[�V���������Ԃɑ�������
		beam_t = beam_ts[animation_num[animation_count]];
		beam_b = beam_bs[animation_num[animation_count]];
	}


	location += velocity * delta_second;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Beam::Draw(const Vector2D& screen_offset) const
{
	if (beam_state == BeamState::Charging)
	{
		// ��: �`���[�W�G�t�F�N�g��`��i���j
		DrawCircle(location.x, location.y, 50 + charge_time * 50, GetColor(255, 255, 100), TRUE);
		return;
	}

	if (beam_state != BeamState::Firing) return;

	//// �e��`�悷��
	//DrawBox(location.x - box_size.x, location.y - box_size.y,
	//	location.x + box_size.x, location.y + box_size.y, GetColor(255, 15, 192), TRUE);

	DrawRotaGraph(location.x, (location.y + box_size.y) - 600.0f, 1.5f, 0.0f, beam_t, TRUE); // �g�b�v
	DrawRotaGraph(location.x, (location.y + box_size.y) - 500.0f, 1.5f, 0.0f, beam_t, TRUE); // �g�b�v
	DrawRotaGraph(location.x, (location.y + box_size.y) - 400.0f, 1.5f, 0.0f, beam_t, TRUE); // �g�b�v
	DrawRotaGraph(location.x, (location.y + box_size.y) - 300.0f, 1.5f, 0.0f, beam_t, TRUE); // �g�b�v
	DrawRotaGraph(location.x, (location.y + box_size.y) - 200.0f, 1.5f, 0.0f, beam_t, TRUE); // �g�b�v
	DrawRotaGraph(location.x, (location.y + box_size.y) - 50.0f, 1.5f, 0.0f, beam_b, TRUE); // �{�g��

}

// �I��������
void Beam::Finalize()
{
}

/// <summary>
/// �q�b�g������
/// </summary>
/// <param name="hit_object">������������</param>
void Beam::OnHitCollision(GameObjectBase* hit_object)
{
	// 0.5�b�����Ȃ甚���������Ȃ�
	if (destroy_time - last_explosion_time < 0.1f) 
	{
		return;
	}
	last_explosion_time = destroy_time;

	if (hit_object->GetCollision().object_type == eObjectType::eBoss3)
	{
		AnimationManager* am = Singleton<AnimationManager>::GetInstance();
		int anim_id = 0;

		float random_x = static_cast<float>(GetRand(70));
		if (GetRand(2) == 1)
		{
			random_x *= -1;
		}
		Vector2D t_location = hit_object->GetLocation();

		float random_y = static_cast<float>(GetRand(100));

		anim_id = am->PlayerAnimation(EffectName::eExprotion2, Vector2D(location.x + random_x, (t_location.y + hit_object->GetBoxSize().y) - random_y), 0.03f, false);
		// �A�j���[�V�����̒ǉ��ݒ�
		am->SetAlpha(anim_id, 255);       // ������
		am->SetScale(anim_id, 0.8f);      // 1.5�{�g��
		am->SetZLayer(anim_id, 3);
	}
	else
	{

		AnimationManager* am = Singleton<AnimationManager>::GetInstance();
		int anim_id = 0;

		float random_x = static_cast<float>(GetRand(70));
		if (GetRand(2) == 1)
		{
			random_x *= -1;
		}
		Vector2D t_location = hit_object->GetLocation();

		float random_y = static_cast<float>(GetRand(150));

		anim_id = am->PlayerAnimation(EffectName::eExprotion2, Vector2D(location.x + random_x, (t_location.y + 48.0f) - random_y), 0.03f, false);
		// �A�j���[�V�����̒ǉ��ݒ�
		am->SetAlpha(anim_id, 255);       // ������
		am->SetScale(anim_id, 0.8f);      // 1.5�{�g��
		am->SetZLayer(anim_id, 3);
	}


}

void Beam::SetBeamFlip(bool flip)
{
	Beam_flip = flip;
}

void Beam::SetPlayer(Player* p_player)
{
	player = p_player;
}
