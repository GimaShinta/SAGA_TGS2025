#include "Boss2.h"
#include "../../../../Utility/ProjectConfig.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../Shot/EnemyBeam1.h"

// ���`���
float Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

Boss2::Boss2()
{
}

Boss2::~Boss2()
{
}

// ����������
void Boss2::Initialize()
{
	enemy_type = ENE_BOSS2;
	z_layer = 3;
	box_size = 30;
	hp = 30000;

	// �U���p�^�[���̐ݒ�
	attack_pattrn_num = { 7, 12, 5, 12, 6, 12, 10, 12 };


	// �����蔻��̃I�u�W�F�N�g�ݒ�
	//collision.is_blocking = true;
	collision.is_blocking = false;
	collision.object_type = eObjectType::eNone;
	collision.hit_object_type.clear();

	//// �����̃I�u�W�F�N�g�^�C�v
	//collision.object_type = eObjectType::eBoss2;
	//// �����鑊��̃I�u�W�F�N�g�^�C�v
	//collision.hit_object_type.push_back(eObjectType::eShot);
	//collision.hit_object_type.push_back(eObjectType::eBeam);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;

	// �퓬���̒��S���W
	base_position = Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) - 100);

	// �o�ꎞ�̒��S���W
	generate_base_position = Vector2D(D_WIN_MAX_X / 2 + 150, D_WIN_MAX_Y + 200);

	// �������W�͓o�ꎞ�̂��̂��g�p
	location = generate_base_position;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	boss2_image[0] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_02/boss03_a1.png")[0];
	boss2_image[1] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_02/boss03_a2.png")[0];
	boss2_image[2] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_02/boss03_b1.png")[0];
	boss2_image[3] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_02/boss03_b2.png")[0];
	boss2_image[4] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_02/boss03_b3.png")[0];
	boss2_image[5] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_02/boss03_b4.png")[0];
	boss2_image[6] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_02/boss03_b5.png")[0];

	boss2_anim = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_02/anime01.png", 6, 6, 1, 80, 40);
	boss2_image[7] = boss2_anim[0];

	boss2_jet = rm->GetImages("Resource/Image/Effect/exhaust_03_spritesheet.png", 24, 8, 3, 128, 200);
	jet = boss2_jet[0];


	// ������Əd���Ȃ�
	//se[0] = rm->GetSounds("Resource/sound/se/battle/bakuhatu_b.mp3");
	//se[1] = rm->GetSounds("Resource/sound/se/boss_se/boss_kill.mp3");
	//se[2] = rm->GetSounds("Resource/sound/se/boss_se/bakuhatu_end.mp3");
	//ChangeVolumeSoundMem(255 * 100 / 100, se[0]);
	//ChangeVolumeSoundMem(255 * 100 / 100, se[1]);
	//ChangeVolumeSoundMem(255 * 100 / 100, se[2]);

	// �ŏ��͖{�̂̈ʒu�ɌŒ�
	for (int i = 0; i < 6; ++i)
	{
		part_positions[i] = location;
	}

	for (int i = 0; i < 5; ++i) {
		ripples[i].active = false;
		ripples[i].timer = 0.0f;
	}

	// ��F�U���p�^�[�����Ƃɍ��E�̔g��ʒu���`
	ripple_positions[0] = { Vector2D(-160,  100), Vector2D(160,  100) };
	ripple_positions[7] = { Vector2D(-100,  70), Vector2D(100,  70) };
	ripple_positions[5] = { Vector2D(-100,  70), Vector2D(100,  70) };
	ripple_positions[6] = { Vector2D(-100,  70), Vector2D(100,  70) };
	ripple_positions[10] = { Vector2D(-100,  70), Vector2D(100,  70) };
	ripple_positions[12] = { Vector2D(-160,  100), Vector2D(160,  100) };


	//		if (attack_pattrn != 12)
//		{
//			ripples[i].pos = Vector2D(location.x - 160.0f, location.y + 100.0f); // ���C��
//		}
//		else
//		{
//			ripples[i].pos = Vector2D(location.x - 100.0f, location.y + 40.0f); // ���C��
//		}

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void Boss2::Update(float delta_second)
{
	// �A�j���[�V����
	GameObjectBase::AnimationControl(image);

	// �ړ�����
	Movement(delta_second);

	// �̗͂��Ȃ��Ȃ�����폜
	if (hp <= 0 && !is_crashing) {
		is_crashing = true;
		velocity = Vector2D(0, 0); // �����͎蓮����
		return; // ���̃t���[���ňȍ~�̏����͍s��Ȃ�
	}

	// ���i�̑��΃I�t�Z�b�g�i���E��2���j
	Vector2D offsets_2[6] = {
		Vector2D(-180, 0), // ����
		Vector2D(-100,  0), // ����O
		Vector2D(100,  0), // �E��O
		Vector2D(180, 0),  // �E��

		Vector2D(-100, -100),  // �C
		Vector2D(100, -100)  // �C
	};


	if (is_crashing) {
		// �Փˏ����͖�����
		collision.object_type = eObjectType::eNone;
		collision.hit_object_type.clear();

		// �^�C�}�[���Z
		crash_timer += delta_second;

		const float crash_duration = 10.0f;
		float t = Clamp(crash_timer / crash_duration, 0.0f, 1.0f);
		float eased_t = 1.0f - pow(1.0f - t, 3);
		image_size = Lerp(2.0f, 1.0f, eased_t); // �X�P�[���k��

		// ===== �������I�Ȉړ� =====
		crash_velocity.y += gravity * delta_second;   // �d�͂ŗ�������
		base_position += crash_velocity * delta_second;

		// ===== �U�����o =====
		float shake_amplitude = 20.0f * t;
		float shake = sin(crash_timer * 10.0f) * shake_amplitude;
		base_position.y += shake * delta_second;

		// ===== ���i���k�߂Ė{�̂Ɋ񂹂� =====
		for (int i = 0; i < 6; ++i) {
			float offset_ratio = Lerp(1.0f, 0.5f, eased_t);
			Vector2D adjusted_offset = offsets_2[i] * offset_ratio;
			part_positions[i] = location + adjusted_offset;
		}

		// ===== ���������J�n�i���񂾂��j=====
		if (!explosions_started) {
			explosions_started = true;
			explosion_index = 0;
			explosion_timer = 0.0f;
			AnimationManager::GetInstance()->PlaySE(SE_NAME::Bakuhatu);
			AnimationManager::GetInstance()->PlaySE(SE_NAME::Kill);
			//PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
			//PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
			// ����̔����𑦎�����
			float offset_x = static_cast<float>(GetRand(200) - 100);
			float offset_y = static_cast<float>(GetRand(200) - 100);
			Vector2D random_pos = location + Vector2D(offset_x, offset_y);
			float scale = 0.3f + (GetRand(200) / 200.0f); // 0.5 �` 1.5

			int id = AnimationManager::GetInstance()->PlayerAnimation(
				EffectName::eExprotion2,
				random_pos,
				0.05f,
				false
			);
			AnimationManager::GetInstance()->SetScale(id, scale);

			explosion_index++; // �ŏ��̔����J�E���g
		}

		// ===== ���ԍ��Ŕ����𔭐� =====
		if (explosions_started) {
			explosion_timer += delta_second;

			// ���������i�ő吔���Ԋu�j
			if (explosion_index < max_explosions && explosion_timer >= explosion_interval) {
				explosion_timer = 0.0f;

				float offset_x = static_cast<float>(GetRand(200) - 100);
				float offset_y = static_cast<float>(GetRand(200) - 100);
				Vector2D random_pos = location + Vector2D(offset_x, offset_y);
				float scale = 0.3f + (GetRand(200) / 200.0f); // 0.5 �` 1.5
				//PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
				AnimationManager::GetInstance()->PlaySE(SE_NAME::Bakuhatu);
				int id = AnimationManager::GetInstance()->PlayerAnimation(
					EffectName::eExprotion2,
					random_pos,
					0.05f,
					false
				);
				AnimationManager::GetInstance()->SetScale(id, scale);

				explosion_index++;
			}

			// �S����������ɑ唚�����폜
			if (explosion_index >= max_explosions) {
				//PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
				//PlaySoundMem(se[2], DX_PLAYTYPE_BACK);
				AnimationManager::GetInstance()->PlaySE(SE_NAME::Kill);
				AnimationManager::GetInstance()->PlaySE(SE_NAME::Bakuhatu_End);

				int id = AnimationManager::GetInstance()->PlayerAnimation(
					EffectName::eExprotion2,
					location,
					0.07f,
					false
				);
				AnimationManager::GetInstance()->SetScale(id, 3.5f);

				id = AnimationManager::GetInstance()->PlayerAnimation(
					EffectName::eExprotion2,
					location,
					0.07f,
					false
				);
				AnimationManager::GetInstance()->SetScale(id, 2.5f);

				is_alive = false;
			}
		}

		return;
	}

	// �U���p�^�[����ݒ肵�Ēe��ł�
	Shot(delta_second);

	if (is_drive == true)
	{
		ripple_positions[7] = { Vector2D(-160,  100), Vector2D(160,  100) };
		ripple_positions[5] = { Vector2D(-160,  100), Vector2D(160,  100) };

	}


	damage_timer += delta_second;

	if (generate2 == true)
	{
		if (damage_timer >= 0.05f)
		{
			damage_timer = 0.0f;
			hp -= 5;
		}

	}

	//// �U���p�^�[���ύX���Ɏ��ԃ��Z�b�g
	//if (attack_pattrn != prev_attack_pattrn)
	//{
	//	if (prev_attack_pattrn == 3)
	//	{
	//		move_time = 0.0f; // �O���̐擪�ɖ߂��I
	//	}
	//	prev_attack_pattrn = attack_pattrn;
	//}

	// ���i��x��ĒǏ]�����鏈��
	const float follow_speed = 5.0f; // �Ǐ]���x�i�傫���Ƒ������Ă���j

	// ���i�̑��΃I�t�Z�b�g�i���E��2���j
	Vector2D offsets[6] = {
		Vector2D(110, 0), // ����
		Vector2D(60,  0), // ����O
		Vector2D(-60,  0), // �E��O
		Vector2D(-110, 0),  // �E��

		Vector2D(60, 30),  // �C
		Vector2D(-60, 30)  // �C
	};


	// �Ǐ]�̑���
	float individual_follow_speeds[6] = {
	12.0f,  // �����i�������j
	12.0f,  // ����O
	12.0f,  // �E��O
	12.0f,   // �E���i�������j

	10.0f,   // �C�i�������j
	10.0f   // �C�i�������j
	};

	// �Ǐ]�̑���
	float individual_follow_speeds_2[6] = {
	100.0f,  // �����i�������j
	100.0f,  // ����O
	100.0f,  // �E��O
	100.0f,   // �E���i�������j

	100.0f,   // �C�i�������j
	100.0f   // �C�i�������j
	};

	// �Ǐ]����
	for (int i = 0; i < 6; ++i)
	{
		Vector2D target;
		if (generate == false)
		{
			target = location + offsets[i];
			part_positions[i] += (target - part_positions[i]) * individual_follow_speeds_2[i] * delta_second;
			angle = 3.14 / 1.0f;

		}
		else
		{
			target = location + offsets_2[i];
			part_positions[i] += (target - part_positions[i]) * individual_follow_speeds[i] * delta_second;
			angle = 0.0f;
		}
	}


	std::vector<int> animation_num = { 0, 1, 2, 3, 4, 5 };
	//�t���[�����[�g�Ŏ��Ԃ��v��
	animation_time += delta_second;
	//8�b�o������摜��؂�ւ���
	if (animation_time >= anim_speed)
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
		boss2_image[7] = boss2_anim[animation_num[animation_count]];
	}

	if (attack_pattrn == 12)
	{
		anim_speed = 0.01;
	}
	else
	{
		anim_speed = 0.1f;
	}

	std::vector<int> jet_num = { 2, 3, 4, 8, 9, 10, 11, 12, 16, 17, 16, 12, 11, 10, 9, 8, 4, 3, 2 };
	//�t���[�����[�g�Ŏ��Ԃ��v��
	jet_timer += delta_second;
	//8�b�o������摜��؂�ւ���
	if (jet_timer >= 0.01f)
	{
		//�v�����Ԃ̏�����
		jet_timer = 0.0f;
		//���Ԍo�߃J�E���g�̑���
		jet_count++;
		//�J�E���g���A�j���[�V�����摜�̗v�f���ȏ�ɂȂ�����
		if (jet_count >= jet_num.size())
		{
			//�J�E���g�̏�����
			jet_count = 0;
		}
		// �A�j���[�V���������Ԃɑ�������
		jet = boss2_jet[jet_num[jet_count]];
	}

	if (generate) {
		show_hpbar = true;

		if (hpbar_fade_timer < 1.0f) {
			hpbar_fade_timer += delta_second;
			if (hpbar_fade_timer > 1.0f) hpbar_fade_timer = 1.0f;
		}
	}


	for (int i = 0; i < 5; ++i) {
		if (ripples[i].active) {
			ripples[i].timer += delta_second;
			if (ripples[i].timer >= 0.5f) { // 0.5�b�ŏ���
				ripples[i].active = false;
			}
		}
	}
	// �e�N���X�̍X�V�������Ăяo��
	__super::Update(delta_second);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Boss2::Draw(const Vector2D& screen_offset) const
{
	//// �G���P��`�悷��
	//DrawBox(location.x - box_size.x, location.y - box_size.y,
	//	location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);

	//DrawBoss2(location);

	if (!show_hpbar) {
		DrawBoss2(location);
		return;
	}

	// �{�X�{�̕`��
	DrawBoss2(location);

	// �t�F�[�h�{�X�P�[�����i0.0?1.0�j
	const float t = hpbar_fade_timer;
	float scale = t;
	if (scale < 0.0f) scale = 0.0f;
	if (scale > 1.0f) scale = 1.0f;
	const int alpha = static_cast<int>(scale * 255);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// �X�P�[�����O�t���`��
	const float max_hp = 30000.0f;
	const float bar_width_full = 650.0f;
	const float bar_height = 8.0f;
	const float x_center = D_WIN_MAX_X / 2;
	const float y = 30.0f;

	// ���ۂ̕`�敝�i�k�����g��j
	const float current_bar_width = bar_width_full * scale;
	const float x = x_center - current_bar_width / 2;

	float hp_ratio = (float)hp / (float)max_hp;
	if (hp_ratio < 0.0f) hp_ratio = 0.0f;
	if (hp_ratio > 1.0f) hp_ratio = 1.0f;



	//float hp_ratio = Clamp(hp / max_hp, 0.0f, 1.0f);
	const float filled_width = current_bar_width * hp_ratio;

	// �F�i��F50%�ȉ��ŔZ���I�����W�j
	unsigned int bar_color = (hp_ratio > 0.5f)
		? GetColor(255, 0, 0)
		: GetColor(255, 100, 0);

	// �w�i
	DrawBox(x - 2, y - 2, x + current_bar_width + 2, y + bar_height + 2, GetColor(0, 0, 0), TRUE);

	// HP�o�[�{��
	DrawBox(x, y, x + filled_width, y + bar_height, bar_color, TRUE);

	// ���g
	DrawBox(x, y, x + current_bar_width, y + bar_height, GetColor(255, 255, 255), FALSE);

	// ���ڂ݁i���`�Ԉʒu�j
	float notch_x = x + current_bar_width / 3;
	DrawBox(notch_x - 1, y - 2, notch_x + 1, y + bar_height + 2, GetColor(255, 255, 0), TRUE);

	// HP���l
	//DrawFormatString(x + current_bar_width + 10, y - 2, GetColor(255, 255, 255), "HP: %.0f", hp);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

// �I��������
void Boss2::Finalize()
{
}

void Boss2::OnHitCollision(GameObjectBase* hit_object)
{
	if (hit_object->GetCollision().object_type == eObjectType::eShot)
	{
		if (generate2 == true)
		{
			if (on_hit == false)
			{
				if (is_weakness == true)
				{
					hp -= 50;
				}
				else
				{
					hp -= 10;
				}
				on_hit = true;

				if (GetRand(70) == 1)
				{
					DropItems();
				}
			}
			else
			{
				on_hit = false;
			}
		}
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBeam)
	{
		if (generate2 == true)
		{
			beam_damage_timer += delta;

			if (beam_damage_timer >= 0.05f)
			{
				if (is_weakness == true)
				{
					hp -= 50;
				}
				else
				{
					hp -= 10;
				}
				beam_damage_timer = 0;

				if (GetRand(70) == 1)
				{
					DropItems();
				}
			}
		}
	}
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss2::Movement(float delta_second)
{
	move_time += delta_second;

	const float float_amplitude_y = 15.0f;
	const float float_speed_y = 2.7f;
	const float float_amplitude_x = 25.0f;
	const float float_speed_x = 2.7f;
	Vector2D float_offset;
	float_offset.y = sinf(move_time * float_speed_y) * float_amplitude_y;
	float_offset.x = cosf(move_time * float_speed_x) * float_amplitude_x;

	if (!generate)
	{
		velocity.y = -900.0f;
		base_position.x = generate_base_position.x;

		if (location.y < -400)
		{
			generate = true;
		}
	}
	else
	{
		float distance_y = base_position.y - location.y;
		float base_x = D_WIN_MAX_X / 2;
		base_position.x = generate_base_position.x - 150.0f;

		if (fabs(distance_y) > 1.0f)
		{
			const float max_speed = 200.0f;
			velocity.y = distance_y;
			if (velocity.y > max_speed) velocity.y = max_speed;
			if (velocity.y < -max_speed) velocity.y = -max_speed;

			static bool check = false;
			if (check == false)
			{
				box_size = Vector2D(200, 70);
				check = true;
			}
			else
			{
				box_size = Vector2D(100, 120);
				check = false;
			}

			collision.object_type = eObjectType::eBoss2;
			collision.hit_object_type = { eObjectType::eShot, eObjectType::eBeam };

			//box_size = Vector2D(190, 80);
			image_size = 2.0f;
		}
		else
		{

			generate2 = true;

			const float swing_range = 100.0f;
			const float move_speed = 75.0f;
			// ��������
			const float deceleration_distance = 50.0f;
			const float min_speed = 10.0f;
			const float wait_time_max = 0.2f; // �܂�Ԃ����̒�~����

			if (!has_initialized_swing_x)
			{
				swing_center_x = base_x;
				has_initialized_swing_x = true;
				swing_direction = 1;
				swing_wait_timer = 0.0f;
			}

			float left_x = swing_center_x - swing_range;
			float right_x = swing_center_x + swing_range;

			// ��~���̏ꍇ
			if (swing_wait_timer > 0.0f)
			{
				swing_wait_timer -= delta_second;
				if (swing_wait_timer < 0.0f) swing_wait_timer = 0.0f;
				velocity.x = 0.0f;
			}
			else
			{
				float distance_to_edge = (swing_direction == 1)
					? right_x - base_position.x
					: base_position.x - left_x;

				// �[�ɓ��B
				if (distance_to_edge <= 0.0f)
				{
					swing_direction *= -1;
					swing_wait_timer = wait_time_max;
					velocity.x = 0.0f;
				}
				else
				{
					float t = Clamp(distance_to_edge / deceleration_distance, 0.0f, 1.0f);
					float eased = t * t * (3 - 2 * t); // easeInOut���
					float current_speed = Max(move_speed * eased, min_speed);
					velocity.x = current_speed * swing_direction;
				}
			}

			velocity.y = 0.0f;
			location.y = base_position.y;
		}
	}

	generate_base_position += velocity * delta_second;
	base_position.x += velocity.x * delta_second;

	location.y = generate_base_position.y + float_offset.y;
	location.x = base_position.x + float_offset.x;
}

// �U��
void Boss2::Shot(float delta_second)
{
	// ���̍U���p�^�[���ύX�܂ł̎���
	const int shot_interval = 1.0f;

	// ���Ԍo�߂�����U���p�^�[����ύX���Ēe�𔭎�
	if (generate2 == true && is_shot == false)
	{
		auto it = ripple_positions.find(attack_pattrn);
		if (it != ripple_positions.end()) {
			const Vector2D& left_offset = it->second.first;
			const Vector2D& right_offset = it->second.second;

			for (int i = 0; i < 5; ++i) {
				if (!ripples[i].active) {
					ripples[i].active = true;
					ripples[i].timer = 0.0f;
					ripples[i].pos = location + left_offset;
					AnimationManager::GetInstance()->PlaySE(SE_NAME::Hamon);
					AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::Hamon, 100);
					break;
				}
			}

			for (int i = 0; i < 5; ++i) {
				if (!ripples[i].active) {
					ripples[i].active = true;
					ripples[i].timer = 0.0f;
					ripples[i].pos = location + right_offset;
					break;
				}
			}
		}




		//// �g���2�i���E�j����������
		//for (int i = 0; i < 5; ++i)
		//{
		//	if (!ripples[i].active)
		//	{
		//		ripples[i].active = true;
		//		ripples[i].timer = 0.0f;

		//		// �����Ŕg��̉����Đ�����
		//		AnimationManager::GetInstance()->PlaySE(SE_NAME::Hamon); // ������SE��
		//		AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::Hamon, 50);
		//		if (attack_pattrn != 12)
		//		{
		//			ripples[i].pos = Vector2D(location.x - 160.0f, location.y + 100.0f); // ���C��
		//		}
		//		else
		//		{
		//			ripples[i].pos = Vector2D(location.x - 100.0f, location.y + 40.0f); // ���C��
		//		}
		//		break;
		//	}
		//}
		//for (int i = 0; i < 5; ++i)
		//{
		//	if (!ripples[i].active)
		//	{
		//		ripples[i].active = true;
		//		ripples[i].timer = 0.0f;
		//		if (attack_pattrn != 12)
		//		{
		//			ripples[i].pos = Vector2D(location.x + 160.0f, location.y + 100.0f); // ���C��
		//		}
		//		else
		//		{
		//			ripples[i].pos = Vector2D(location.x + 100.0f, location.y + 40.0f); // ���C��
		//		}
		//		break;
		//	}
		//}

		shot_timer += delta_second;
		if (shot_timer >= shot_interval)
		{
#if 1

	#if 0
			// ����̍U���̂݌J��Ԃ�
			attack_pattrn = 5;
	#else
			// HP����������U���p�^�[����ύX�i�I�[�o�[�t���[�h�~�ɍ��킹�ă��Z�b�g�j
			if (hp <= 10000 && attack_pattrn_num != std::vector<int>{7, 5})
			{
				is_drive = true;
				attack_pattrn_num = { 7, 5 };
				attack_count = 0; // ���S�Ƀ��Z�b�g
			}

			// ���߂�ꂽ�U���p�^�[���̂݌J��Ԃ�
			if (!attack_pattrn_num.empty())
			{
				// �O�̂��ߔz��T�C�Y�`�F�b�N�����ށi�O�����j
				if (attack_count >= attack_pattrn_num.size())
				{
					attack_count = 0;
				}
				attack_pattrn = attack_pattrn_num[attack_count];
				attack_count++;
		}
			else
			{
				// �\�������F�U���p�^�[�����X�g����̎��͈��S�ȃf�t�H���g��
				attack_pattrn = 0;
			}
#endif

#else
			// ���S�����_��
			attack_pattrn = 4 + rand() % MAX_ATTACK_PATTRN;
#endif
			is_shot = true;
			shot_timer = 0;
		}
	}

	// �U��
	Attack(delta_second);
}

void Boss2::DrawBoss2(const Vector2D position) const
{
	// �{��
	DrawRotaGraph(position.x, position.y, image_size, angle, boss2_image[2], TRUE);

	// ���i
	DrawRotaGraph(part_positions[1].x, part_positions[1].y, image_size, angle, boss2_image[3], TRUE); // ����O
	DrawRotaGraph(part_positions[0].x, part_positions[0].y, image_size, angle, boss2_image[5], TRUE); // ����
	if (is_crashing == false)
	{
		if (generate == false)
		{
			DrawRotaGraph(position.x - 70, position.y + 150, image_size, 0.0f, jet, TRUE);
			DrawRotaGraph(position.x + 70, position.y + 150, image_size, 0.0f, jet, TRUE);

		}
		else
		{
			DrawRotaGraph(position.x - 110, position.y - 250, image_size, 3.14 / 1.0f, jet, TRUE);
			DrawRotaGraph(position.x + 110, position.y - 250, image_size, 3.14 / 1.0f, jet, TRUE);
		}

	}
	DrawRotaGraph(part_positions[2].x, part_positions[2].y, image_size, angle, boss2_image[4], TRUE); // �E��O
	DrawRotaGraph(part_positions[3].x, part_positions[3].y, image_size, angle, boss2_image[6], TRUE); // �E��
	DrawRotaGraph(part_positions[4].x, part_positions[4].y, image_size, angle, boss2_image[1], TRUE);
	DrawRotaGraph(part_positions[5].x, part_positions[5].y, image_size, angle, boss2_image[1], TRUE);


	if (is_crashing == false)
	{
		if (generate == false)
		{
			DrawRotaGraph(part_positions[4].x, part_positions[4].y - 50.0f, image_size, angle, boss2_image[7], TRUE);
			DrawRotaGraph(part_positions[5].x, part_positions[5].y - 50.0f, image_size, angle, boss2_image[7], TRUE);
		}
		else
		{
			DrawRotaGraph(part_positions[4].x, part_positions[4].y + 110.0f, image_size, angle, boss2_image[7], TRUE);
			DrawRotaGraph(part_positions[5].x, part_positions[5].y + 110.0f, image_size, angle, boss2_image[7], TRUE);
		}

	}
	//DrawBox(location.x - box_size.x, location.y - box_size.y,
	//	location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);



	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); // �A���t�@�u�����hON

	if (is_crashing == false)
	{
		for (int i = 0; i < 5; ++i) {
			if (!ripples[i].active) continue;

			float t = ripples[i].timer / 0.5f; // 0.0�`1.0
			if (t > 1.0f) t = 1.0f;            // ���S�̂���Clamp����Ȃ炱����

			// Lerp(0.0f, 80.0f, t) �� 0.0f + (80.0f - 0.0f) * t
			float radius = 80.0f * t;

			// Lerp(255.0f, 200.0f, t) �� 255.0f + (200.0f - 255.0f) * t = 255 - 55 * t
			int alpha = static_cast<int>(255.0f - 55.0f * t);

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

			if (is_drive == false)
			{

				DrawCircle(static_cast<int>(ripples[i].pos.x),
					static_cast<int>(ripples[i].pos.y),
					static_cast<int>(radius),
					GetColor(150, 255, 150),
					FALSE);
			}
			else
			{
				DrawCircle(static_cast<int>(ripples[i].pos.x),
					static_cast<int>(ripples[i].pos.y),
					static_cast<int>(radius),
					GetColor(255, 0, 0),
					FALSE);

			}

		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �u�����h������
}

int Boss2::GetAttackPattrn() const
{
	return attack_pattrn;
}

int Boss2::GetIsAlive() const
{
	return is_alive;
}

bool Boss2::GetGenerate() const
{
	return generate;
}

bool Boss2::GetIsCrashing() const
{
	return is_crashing;
}

// �{�X�Q�̍U��
void Boss2::Attack(float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	if (is_shot == true)
	{
		Vector2D e_location = location;

		switch (attack_pattrn)
		{
		case 1:
			for (int i = 0; i < 3; i++)
			{
				e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 30) + (30 * i), e_location.y + D_OBJECT_SIZE));
				e_shot4->SetVelocity(Vector2D(0, 120));
			}
			is_shot = false;
			break;
		case 2:
			// �c�Q����
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 60) + (30 * j), e_location.y));
					if (i == 0)
					{
						e_shot4->SetVelocity(Vector2D(0, 120));
					}
					else if (i == 1)
					{
						e_shot4->SetVelocity(Vector2D(0, -120));
					}
				}
			}
			// ���Q����
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(e_location.x, (e_location.y - 60) + (30 * j)));
					if (i == 0)
					{
						e_shot4->SetVelocity(Vector2D(-120, 0));
					}
					else if (i == 1)
					{
						e_shot4->SetVelocity(Vector2D(120, 0));
					}
				}
			}
			is_shot = false;
			break;
		case 3:
			// �c�ɂR�悱�ɂT�����i�Е��j
			for (int j = 1; j < 4; j++)
			{
				for (int i = 0; i < 10; i++)
				{
					e_shot3 = objm->CreateObject<EnemyShot3>(location);
					e_shot3->SetPlayer(player);
					e_shot3->SetPlayerLocation(player->GetLocation());
					// +�P���Ȃ�������O�b�Ŕ��˂����
					e_shot3->SetStepShot(j);
					if (i < 5)
					{
						e_shot3->SetVelocity(Vector2D(240, 0));
						e_shot3->SetPurposeLocation(Vector2D((e_location.x + 150) + (30 * i), (e_location.y + 40) - (50 * j)));
					}
					else
					{
						e_shot3->SetVelocity(Vector2D(-240, 0));
						e_shot3->SetPurposeLocation(Vector2D(e_location.x - (30 * i), (e_location.y + 40) - (50 * j)));
					}
				}
			}
			is_shot = false;
			break;
		case 4:
			/// <summary>
			/// �U���p�^�[���S�i�ԉ΁j
			/// </summary>
			/// <param name="bullet_num">�e�̐�</param>
			/// <param name="speed">�e�̑��x</param>
			/// <param name="spiral_interval">���ˊԊu</param>
			/// <param name="spiral_duration_limit">�U�����鎞��</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn4_2(6, 300.0f, 1.0f, 5.0f, location, delta_second);
			break;
		case 5:
#if 0
			static float spiral_timer = 0.0f;            // �e���˂̃C���^�[�o���Ǘ�
			static float spiral_angle = 0.0f;            // ��]�p�x
			static float spiral_total_time = 0.0f;       // �p�^�[��5�ɓ����Ă���̗݌v����
			const float spiral_interval = 0.1f;
			const float spiral_duration_limit = 3.0f;    // �ő唭�ˎ���

			// 3�b�o�߂����牽�����Ȃ�
			if (spiral_total_time >= spiral_duration_limit) return;

			spiral_timer += delta_second;
			spiral_total_time += delta_second;

			if (spiral_timer >= spiral_interval)
			{
				spiral_timer = 0.0f;

				float rad = spiral_angle * DX_PI / 180.0f;
				float speed = 160.0f;
				Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

				GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
				EnemyShot4* shot = objm->CreateObject<EnemyShot4>(boss2->GetLocation());
				shot->SetVelocity(velocity);

				spiral_angle += 15.0f;
				if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
			}
#else

			/// <summary>
			/// �U���p�^�[���T�i�Q�����j
			/// </summary>
			/// <param name="spiral_interval">���ˊԊu</param>
			/// <param name="spiral_duration_limit">�U�����鎞��</param>
			/// <param name="spiral_speed">�e�̑��x</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn5_2(0.05f, 5.0f, 300.0f, location, delta_second);
			//Pattrn5(0.08f, 5.0f, 300.0f, location, delta_second);
#endif
			break;
		case 6:
			/// <summary>
			/// �U���p�^�[���U�i�o���o����^�j
			/// </summary>
			/// <param name="fan_angle_range">��̊p�x�i60�Ȃ�}30���j</param>
			/// <param name="bullet_speed">�e�̑��x</param>
			/// <param name="fan_interval">���ˊԊu</param>
			/// <param name="fan_duration_limit">�U�����鎞��</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn6_2(100.0f, 300.0f, 0.15f, 10.0f, location, delta_second);

			break;
		case 7:
			/// <summary>
			/// �U���p�^�[���V�i�i�K��`�j
			/// </summary>
			/// <param name="fan_angle_range">��̊p�x�͈́i60�Ȃ�}30���j</param>
			/// <param name="bullet_speed">�e�̑��x</param>
			/// <param name="fan_interval">���˂̊Ԋu</param>
			/// <param name="fan_duration_limit">�U������</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn7_2(150.0f, 500.0f, 0.2f, 5.0f, Vector2D(location.x, location.y + 100.0f), delta_second);

			break;
		case 8:
			/// <summary>
			/// �U���p�^�[���W�i�Ђ�Q�j
			/// </summary>
			/// <param name="wave_interval">���ˊԊu</param>
			/// <param name="wave_duration_limit">���Ԏ��Ԃ̏��</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn8(0.1f, 1.0f, Vector2D(location.x, location.y + 150.0f), delta_second);

			break;
		case 9:
			/// <summary>
			/// �U���p�^�[���X�i�{�X���S��]�e�j
			/// </summary>
			/// <param name="shot_count">�e�̐�</param>
			/// <param name="radius">�{�X����̋���</param>
			/// <param name="angular_speed">��]���x</param>
			/// <param name="bullet_speed">�e��</param>
			/// <param name="rotation_timer">��]�̎��Ԍv��</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn9_2(27, 250.0f, 90.0f, 0.0f, location, delta_second);

			/// <summary>
			/// �U���p�^�[��11�i��{�r�[���j
			/// </summary>
			/// <param name="offsets_x">�{�X����̋���(��)</param>
			Pattrn11(0.0f);

			break;
		case 10:
			/// <summary>
			/// �U���p�^�[���P�O�i�x�N�g�����S��]�e�j
			/// </summary>
			/// <param name="shot_count">�e�̐�</param>
			/// <param name="radius">�{�X����̋���</param>
			/// <param name="angular_speed">��]���x</param>
			/// <param name="bullet_speed">�e��</param>
			/// <param name="rotation_timer">��]�̎��Ԍv��</param>
			/// <param name="center_location">��������ꏊ</param>
			/// <param name="generate_location">��������ꏊ</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn10_2(8, 120.0f, 550.0f, 450.0f, 5.0f, location, delta_second);

			break;
		case 11:
			/// <summary>
			/// �U���p�^�[��11�i��{�r�[���j
			/// </summary>
			/// <param name="offsets_x">�{�X����̋���(��)</param>
			Pattrn11(0.0f);

			break;
		case 12:
			/// <summary>
			/// �U���p�^�[��12�i��{�r�[���j
			/// </summary>
			Pattrn12();

			break;
		case 13:
			Pattrn13(delta_second);

			break;
		default:
			break;
		}
	}
}

/// <summary>
/// �U���p�^�[���S
/// </summary>
/// <param name="bullet_num">�e�̐�</param>
/// <param name="speed">�e�̑��x</param>
/// <param name="spiral_interval">���ˊԊu</param>
/// <param name="spiral_duration_limit">�U�����鎞��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss2::Pattrn4(int bullet_num, float speed, float spiral_interval, float spiral_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float spiral_timer = 0.0f;
	static float spiral_total_time = 0.0f;

	spiral_timer += delta_second;
	spiral_total_time += delta_second;

	if (spiral_timer >= spiral_interval)
	{
		spiral_timer = 0.0f;

		for (int i = 0; i < bullet_num; i++)
		{
			float angle = (360.0f / bullet_num) * i;
			float rad = angle * DX_PI / 180.0f;

			Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

			EnemyShot4* e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
			e_shot4->SetVelocity(velocity);
			AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
			AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

		}
	}

	// ���Ԑ����𒴂�����I���i���˂��Ȃ��j
	if (spiral_total_time >= spiral_duration_limit)
	{
		spiral_total_time = 0.0f;
		is_shot = false;
	}
}

void Boss2::Pattrn4_2(int bullet_num, float speed, float spiral_interval, float spiral_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float spiral_timer = 0.0f;
	static float spiral_total_time = 0.0f;

	spiral_timer += delta_second;
	spiral_total_time += delta_second;

	if (spiral_timer >= spiral_interval)
	{
		spiral_timer = 0.0f;

		for (int i = 0; i < bullet_num; i++)
		{
			float angle = (360.0f / bullet_num) * i;
			float rad = angle * DX_PI / 180.0f;

			Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

			EnemyShot4* e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x - 170.0f, generate_location.y + 65.0f));
			e_shot4->SetVelocity(velocity);
			AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
			AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);


		}

		for (int i = 0; i < bullet_num; i++)
		{
			float angle = (360.0f / bullet_num) * i;
			float rad = angle * DX_PI / 180.0f;

			Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

			EnemyShot4* e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x + 170.0f, generate_location.y + 65.0f));
			e_shot4->SetVelocity(velocity);
		}
	}

	// ���Ԑ����𒴂�����I���i���˂��Ȃ��j
	if (spiral_total_time >= spiral_duration_limit)
	{
		spiral_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���T
/// </summary>
/// <param name="spiral_interval">���ˊԊu</param>
/// <param name="spiral_duration_limit">�U�����鎞��</param>
/// <param name="spiral_speed">�e�̑��x</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss2::Pattrn5(float spiral_interval, float spiral_duration_limit, float spiral_speed, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float spiral_timer = 0.0f;
	static float spiral_angle = 0.0f;
	static float spiral_total_time = 0.0f;

	spiral_timer += delta_second;
	spiral_total_time += delta_second;

	if (spiral_timer >= spiral_interval)
	{
		spiral_timer = 0.0f;

#if 0
		// �㉺������ɏo��
		for (int dir = 0; dir < 2; dir++)
		{
			float base_angle = (dir == 0) ? 90.0f : 270.0f;
			float current_angle = base_angle + spiral_angle;

			float rad = current_angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

			EnemyShot4* shot = objm->CreateObject<EnemyShot4>(generate_location);
			shot->SetVelocity(velocity);
			shot->SetAttackPattrn(2);
		}
#else
		// �����
		float base_angle = 90.0f; // �����
		float current_angle = base_angle + spiral_angle;

		float rad = current_angle * DX_PI / 180.0f;
		Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

		EnemyShot4* shot = objm->CreateObject<EnemyShot4>(generate_location);
		shot->SetVelocity(velocity);
		AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
		AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

#endif


		spiral_angle += 40.0f;
		if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
	}

	// ���Ԑ����𒴂�����I���i���˂��Ȃ��j
	if (spiral_total_time >= spiral_duration_limit)
	{
		spiral_total_time = 0.0f;
		is_shot = false;
	}
}

void Boss2::Pattrn5_2(float spiral_interval, float spiral_duration_limit, float spiral_speed, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float spiral_timer = 0.0f;
	static float spiral_angle = 0.0f;
	static float spiral_total_time = 0.0f;

	spiral_timer += delta_second;
	spiral_total_time += delta_second;

	if (spiral_timer >= spiral_interval)
	{
		spiral_timer = 0.0f;

#if 0
		for (int dir = 0; dir < 2; dir++)
		{
			float base_angle = (dir == 0) ? 90.0f : 270.0f;
			float current_angle = base_angle + spiral_angle;

			float rad = current_angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

			EnemyShot4* shot = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x + 170.0f, generate_location.y + 65.0f));
			shot->SetVelocity(velocity);
			shot->SetAttackPattrn(2);
		}

		for (int dir = 0; dir < 2; dir++)
		{
			float base_angle = (dir == 0) ? 90.0f : 270.0f;
			float current_angle = base_angle - spiral_angle; // �� �t��]�ɂ���

			float rad = current_angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

			EnemyShot4* shot = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x - 170.0f, generate_location.y + 65.0f));
			shot->SetVelocity(velocity);
			shot->SetAttackPattrn(2);
		}
#else

		// �E���i���v���A������̂݁j
		{
			float base_angle = 90.0f;
			// �}�C�i�X�ɂ������]�����]����
			float current_angle = base_angle + spiral_angle;

			float rad = current_angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

			EnemyShot4* shot = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x + 170.0f, generate_location.y + 65.0f));
			shot->SetVelocity(velocity);
			AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
			AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

		}

		// �����i�����v���A������̂݁j
		{
			float base_angle = 90.0f;
			float current_angle = base_angle + spiral_angle;

			float rad = current_angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

			EnemyShot4* shot = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x - 170.0f, generate_location.y + 65.0f));
			shot->SetVelocity(velocity);
		}


#endif


		spiral_angle += 40.0f;
		if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
	}

	// ���Ԑ����𒴂�����I���i���˂��Ȃ��j
	if (spiral_total_time >= spiral_duration_limit)
	{
		spiral_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���U
/// </summary>
/// <param name="fan_angle_range">��̊p�x�i60�Ȃ�}30���j</param>
/// <param name="bullet_speed">�e�̑��x</param>
/// <param name="fan_interval">���ˊԊu</param>
/// <param name="fan_duration_limit">�U�����鎞��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss2::Pattrn6(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	// �U�������p static �ϐ�
	static float fan_timer = 0.0f;
	static float fan_total_time = 0.0f;

	// �U���p�^�[�����ύX���ꂽ�烊�Z�b�g������ �� boss2���ŏ�Ԏ�������̂�����

	fan_timer += delta_second;
	fan_total_time += delta_second;

	// ���Ԋu���Ƃɔ���
	if (fan_timer >= fan_interval)
	{
		fan_timer = 0.0f;

		// �����_���Ȋp�x�i���S�}60���j
		float base_angle = 90.0f; // ���������S
		float random_angle = base_angle - fan_angle_range / 2.0f + (rand() % (int)fan_angle_range);

		float rad = random_angle * DX_PI / 180.0f;
		Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

		e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
		e_shot4->SetVelocity(velocity);
		AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
		AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);


		
	}

	// ���Ԑ����𒴂�����I���i���˂��Ȃ��j
	if (fan_total_time >= fan_duration_limit)
	{
		fan_total_time = 0.0f;
		is_shot = false;
	}
}

void Boss2::Pattrn6_2(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	// �U�������p static �ϐ�
	static float fan_timer = 0.0f;
	static float fan_total_time = 0.0f;

	// �U���p�^�[�����ύX���ꂽ�烊�Z�b�g������ �� boss2���ŏ�Ԏ�������̂�����

	fan_timer += delta_second;
	fan_total_time += delta_second;

	// ���Ԋu���Ƃɔ���
	if (fan_timer >= fan_interval)
	{
		fan_timer = 0.0f;

		// �����_���Ȋp�x�i���S�}60���j
		float base_angle = 90.0f; // ���������S
		float random_angle = base_angle - fan_angle_range / 2.0f + (rand() % (int)fan_angle_range);

		float rad = random_angle * DX_PI / 180.0f;
		Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

		e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x + 170.0f, generate_location.y + 65.0f));
		e_shot4->SetVelocity(velocity);
		e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x - 170.0f, generate_location.y + 65.0f));
		e_shot4->SetVelocity(velocity);
		AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
		AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

	}

	// ���Ԑ����𒴂�����I���i���˂��Ȃ��j
	if (fan_total_time >= fan_duration_limit)
	{
		fan_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���V
/// </summary>
/// <param name="fan_angle_range">��̊p�x�͈́i60�Ȃ�}30���j</param>
/// <param name="bullet_speed">�e�̑��x</param>
/// <param name="fan_interval">���˂̊Ԋu</param>
/// <param name="fan_duration_limit">�U������</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss2::Pattrn7(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float fan_timer = 0.0f;
	static float fan_total_time = 0.0f;

	fan_timer += delta_second;
	fan_total_time += delta_second;

	if (fan_timer >= fan_interval)
	{
		fan_timer = 0.0f;

		int bullet_count = 4;
		float base_angle = 90.0f; // ���S������

		for (int i = 0; i < bullet_count; ++i)
		{
			float angle = base_angle - fan_angle_range / 2.0f +
				(fan_angle_range / (bullet_count - 1)) * i;

			float rad = angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

			e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
			e_shot4->SetVelocity(velocity);
			AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
			AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

		}
	}

	// ��莞�Ԍo�߂�����I��
	if (fan_total_time >= fan_duration_limit)
	{
		fan_total_time = 0.0f;
		is_shot = false;
	}
}

void Boss2::Pattrn7_2(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float fan_timer = 0.0f;
	static float fan_total_time = 0.0f;

	fan_timer += delta_second;
	fan_total_time += delta_second;

	if (fan_timer >= fan_interval)
	{
		fan_timer = 0.0f;

		int bullet_count = 6;
		float base_angle = 90.0f; // ���S������

		for (int i = 0; i < bullet_count; ++i)
		{
			float angle = base_angle - fan_angle_range / 2.0f +
				(fan_angle_range / (bullet_count - 1)) * i;

			float rad = angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

			e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x + 170.0f, generate_location.y - 10.0f));
			e_shot4->SetVelocity(velocity);
			AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
			AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

		}

		for (int i = 0; i < bullet_count; ++i)
		{
			float angle = base_angle - fan_angle_range / 2.0f +
				(fan_angle_range / (bullet_count - 1)) * i;

			float rad = angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

			e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(generate_location.x - 170.0f, generate_location.y - 10.0f));
			e_shot4->SetVelocity(velocity);
		}
	}

	// ��莞�Ԍo�߂�����I��
	if (fan_total_time >= fan_duration_limit)
	{
		fan_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���W
/// </summary>
/// <param name="wave_interval">���ˊԊu</param>
/// <param name="wave_duration_limit">���Ԏ��Ԃ̏��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss2::Pattrn8(float wave_interval, float wave_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

#if 1
	static float wave_timer = 0.0f;
	static float wave_total_time = 0.0f;

	wave_timer += delta_second;
	wave_total_time += delta_second;

	if (wave_timer >= wave_interval)
	{
		wave_timer = 0.0f;
		Vector2D e_lo = generate_location;

		// �E��
		e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x + 50, e_lo.y));
		e_shot5->SetWaveReflected(false);
		e_shot5->SetVelocity(Vector2D(0, 200));
		e_shot5->SetWaveParameters(600.0f, 0.7f);

		// ����
		e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x - 50, e_lo.y));
		e_shot5->SetWaveReflected(true);
		e_shot5->SetVelocity(Vector2D(0, 200));
		e_shot5->SetWaveParameters(600.0f, 0.7f);
		AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
		AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

	}

	// ��莞�Ԍo�߂�����I��
	if (wave_total_time >= wave_duration_limit)
	{
		wave_total_time = 0.0f;
		is_shot = false;
	}
#else

	int num_shots = 20;
	float spread_speed = 150.0f;
	Vector2D origin = e_location; // �e�̏������ˈʒu�i�{�X�̈ʒu�j
	Vector2D boss_center = boss2->GetLocation(); // �z�����ݒ��S�I

	for (int i = 0; i < num_shots; ++i)
	{
		float angle = 360.0f / num_shots * i;
		float rad = angle * DX_PI / 180.0f;

		e_shot5 = objm->CreateObject<EnemyShot5>(origin);
		e_shot5->SetVelocity(Vector2D(cosf(rad), sinf(rad)) * spread_speed);
		e_shot5->SetSuckCenter(boss_center); // �z�����ݐ���Z�b�g�I
	}
#endif

}

/// <summary>
/// �U���p�^�[���X
/// </summary>
/// <param name="shot_count">�e�̐�</param>
/// <param name="radius">�{�X����̋���</param>
/// <param name="angular_speed">��]���x</param>
/// <param name="bullet_speed">�e��</param>
/// <param name="rotation_timer">��]�̎��Ԍv��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss2::Pattrn9(int shot_count, float radius, float angular_speed, float bullet_speed, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static std::vector<float> angles;    // �e�e�̊p�x�Ǘ�
	static std::vector<EnemyShot4*> rotating_shots;  // �e�̃|�C���^��ێ�

	static float rotation_timer = 0.0f;

	rotation_timer += delta_second;

	// ����������
	if (angles.empty())
	{
		rotating_shots.clear();
		// �e�𐶐����ď����p�x��ݒ�
		for (int i = 0; i < shot_count; ++i)
		{
			float angle = 360.0f / shot_count * i;  // �e�̊p�x�ݒ�
			angles.push_back(angle);

			// �{�X�̎��͂ɒe��z�u
			e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
			AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
			AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

			if (e_shot4)
			{
				rotating_shots.push_back(e_shot4);  // �������������ꂽ��e��ۑ�
				e_shot4->SetVelocity(Vector2D(0, 0)); // ��]�����Ȃ̂Œe����0

			}
		}
	}

	// �e�e�̈ʒu���X�V���ĉ�]������
	for (int i = 0; i < shot_count; ++i)
	{
		angles[i] += angular_speed * delta_second; // �p�x�X�V
		if (angles[i] >= 360.0f) angles[i] -= 360.0f; // �p�x��0?360�x�ɒ���

		float rad = angles[i] * DX_PI / 180.0f;  // ���W�A���ɕϊ�

		// �{�X�𒆐S�ɔ��a `radius` �̉~�O����`��
		Vector2D new_pos = generate_location + Vector2D(cos(rad) * radius, sin(rad) * radius);

		// �e�̈ʒu���X�V
		if (rotating_shots[i])  // �e�����������݂��邩�m�F
		{
			rotating_shots[i]->SetLocation(new_pos);

			// �e�̑��x�̓[���i��]�̂݁j
			rotating_shots[i]->SetVelocity(Vector2D(0, 0));
		}
	}

	// �U���̏I������i��莞�Ԍo�ߌ�j
	static float duration_timer = 0.0f;
	duration_timer += delta_second;
	if (duration_timer >= 5.0f)  // 5�b��ɏI��
	{
		for (int i = 0; i < shot_count; ++i)
		{
			if (rotating_shots[i])
			{
				rotating_shots[i]->SetDestroy();  // �e���폜
			}

		}
		angles.clear();  // �p�x�����N���A
		rotating_shots.clear();  // �e�̃��X�g���N���A
		duration_timer = 0.0f;
		is_shot = false;
	}
}

void Boss2::Pattrn9_2(int shot_count, float radius, float angular_speed, float bullet_speed, const Vector2D& generate_location, float delta_second)
{
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static std::vector<float> angles_left;
	static std::vector<float> angles_right;
	static std::vector<EnemyShot4*> shots_left;
	static std::vector<EnemyShot4*> shots_right;

	static float rotation_timer = 0.0f;
	rotation_timer += delta_second;

	const float offset_x = 180.0f; // �{�X�̒��S���獶�E�ɂ��炷����
	Vector2D left_center = generate_location + Vector2D(-offset_x, 0);
	Vector2D right_center = generate_location + Vector2D(+offset_x, 0);

	// ������
	if (angles_left.empty() && angles_right.empty())
	{
		shots_left.clear();
		shots_right.clear();

		for (int i = 0; i < shot_count; ++i)
		{
			float angle = 360.0f / shot_count * i;
			EnemyShot4* e_shot4 = nullptr;

			if (i % 2 == 0)
			{
				angles_left.push_back(angle);
				e_shot4 = objm->CreateObject<EnemyShot4>(left_center);
				AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
				AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

				if (e_shot4)
				{
					shots_left.push_back(e_shot4);
					e_shot4->SetVelocity(Vector2D(0, 0));
					//e_shot4->SetAttackPattrn(1);
				}
			}
			else
			{
				angles_right.push_back(angle);
				e_shot4 = objm->CreateObject<EnemyShot4>(right_center);
				if (e_shot4)
				{
					shots_right.push_back(e_shot4);
					e_shot4->SetVelocity(Vector2D(0, 0));
					//e_shot4->SetAttackPattrn(1);
				}
			}
		}
	}

	// �����̒e���X�V
	for (size_t i = 0; i < angles_left.size(); ++i)
	{
		angles_left[i] += angular_speed * delta_second;
		if (angles_left[i] >= 360.0f) angles_left[i] -= 360.0f;

		float rad = angles_left[i] * DX_PI / 180.0f;
		Vector2D new_pos = left_center + Vector2D(cos(rad) * radius, sin(rad) * radius);

		if (i < shots_left.size() && shots_left[i])
		{
			shots_left[i]->SetLocation(new_pos);
			shots_left[i]->SetVelocity(Vector2D(0, 0));
		}
	}

	// �E���̒e���X�V
	for (size_t i = 0; i < angles_right.size(); ++i)
	{
		angles_right[i] += angular_speed * delta_second;
		if (angles_right[i] >= 360.0f) angles_right[i] -= 360.0f;

		float rad = angles_right[i] * DX_PI / 180.0f;
		Vector2D new_pos = right_center + Vector2D(cos(rad) * radius, sin(rad) * radius);

		if (i < shots_right.size() && shots_right[i])
		{
			shots_right[i]->SetLocation(new_pos);
			shots_right[i]->SetVelocity(Vector2D(0, 0));
		}
	}

	// �U���I�������i5�b�o�߂ō폜�j
	static float duration_timer = 0.0f;
	duration_timer += delta_second;
	if (duration_timer >= 5.0f)
	{
		for (auto& s : shots_left)
			if (s) s->SetDestroy();
		for (auto& s : shots_right)
			if (s) s->SetDestroy();

		angles_left.clear();
		angles_right.clear();
		shots_left.clear();
		shots_right.clear();
		duration_timer = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���P�O
/// </summary>
/// <param name="shot_count">�e�̐�</param>
/// <param name="radius">�{�X����̋���</param>
/// <param name="angular_speed">��]���x</param>
/// <param name="bullet_speed">�e��</param>
/// <param name="rotation_timer">��]�̎��Ԍv��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss2::Pattrn10(int shot_count, float radius, float angular_speed, float center_speed, float duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float duration_timer = 0.0f;
	static Vector2D center_pos;  // �����Ȃ����S���W
	static Vector2D center_velocity;
	static std::vector<float> angles;
	static std::vector<EnemyShot4*> rotating_shots;
	static bool initialized = false;

	// ���Ԍo��
	duration_timer += delta_second;

	if (duration_timer < duration_limit)
	{
		if (!initialized) // ����������
		{
			// �{�X�̈ʒu���������S�Ƃ��āA�^���Ɉړ�������
			center_pos = generate_location;
			center_velocity = Vector2D(0.0f, center_speed); // �^������

			angles.clear();
			rotating_shots.clear();
			for (int i = 0; i < shot_count; ++i)
			{
				float angle = 360.0f / shot_count * i;
				angles.push_back(angle);

				EnemyShot4* shot = objm->CreateObject<EnemyShot4>(center_pos);
				AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
				AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

				if (shot)
				{
					shot->SetVelocity(Vector2D(0, 0));
					rotating_shots.push_back(shot);
				}
			}

			initialized = true;
		}

		// �����Ȃ����S�̈ړ�
		center_pos += center_velocity * delta_second;

		// ��]�e�̈ʒu�X�V
		for (int i = 0; i < shot_count; ++i)
		{
			angles[i] += angular_speed * delta_second;
			if (angles[i] >= 360.0f) angles[i] -= 360.0f;

			float rad = angles[i] * DX_PI / 180.0f;
			Vector2D offset(cos(rad) * radius, sin(rad) * radius);

			if (rotating_shots[i])
			{
				rotating_shots[i]->SetLocation(center_pos + offset);
			}
		}
	}
	else
	{
		// �U���I����̏���
		duration_timer = 0.0f;
		is_shot = false;
		initialized = false;  // �������t���O�����Z�b�g
	}
}

void Boss2::Pattrn10_2(int shot_count, float radius, float angular_speed, float center_speed, float duration_limit, const Vector2D& center_location, float delta_second)
{
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float duration_timer = 0.0f;
	static Vector2D center_pos_L, center_pos_R;
	static Vector2D center_velocity;
	static std::vector<RotatingShotData> shot_data_L, shot_data_R;
	static bool initialized = false;
	static float arrival_phase_timer = 0.0f;
	static bool all_arrived = false;


	duration_timer += delta_second;

	if (duration_timer < duration_limit)
	{
		if (!initialized)
		{
			const float offset_x = 170.0f;

			center_pos_L = center_location + Vector2D(-offset_x, 50);
			center_pos_R = center_location + Vector2D(+offset_x, 50);
			center_velocity = Vector2D(0.0f, center_speed);

			shot_data_L.clear();
			shot_data_R.clear();

			for (int i = 0; i < shot_count; ++i)
			{
				float angle = 360.0f / shot_count * i;
				float rad = angle * DX_PI / 180.0f;
				Vector2D offset(cos(rad) * radius, sin(rad) * radius);

				// �����̒e
				EnemyShot4* shot_L = objm->CreateObject<EnemyShot4>(center_pos_L);
				AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot);
				AnimationManager::GetInstance()->ChangeSEVolume(SE_NAME::EnemyShot, 50);

				if (shot_L)
				{
					shot_L->SetVelocity(Vector2D(0, 0));
					shot_data_L.push_back({ shot_L, angle, offset, 0.0f, RotatingShotData::State::MoveToCircle });
				}

				// �E���̒e
				EnemyShot4* shot_R = objm->CreateObject<EnemyShot4>(center_pos_R);
				if (shot_R)
				{
					shot_R->SetVelocity(Vector2D(0, 0));
					shot_data_R.push_back({ shot_R, angle, offset, 0.0f, RotatingShotData::State::MoveToCircle });
				}
			}

			initialized = true;
		}

		// ���S�ړ�
		if (all_arrived)
		{
			center_pos_L += center_velocity * delta_second;
			center_pos_R += center_velocity * delta_second;
		}

		auto update_shots = [&](std::vector<RotatingShotData>& shot_data, const Vector2D& center_pos)
			{
				for (auto& data : shot_data)
				{
					if (!data.shot) continue;

					switch (data.state)
					{
					case RotatingShotData::State::MoveToCircle:
					{
						data.arrival_timer += delta_second;
						float t = Min(data.arrival_timer / 1.0f, 1.0f);
						float eased = 1.0f - pow(1.0f - t, 2.0f); // easeOut

						Vector2D pos = center_pos + data.target_offset * eased;
						data.shot->SetLocation(pos);

						if (t >= 1.0f)
							data.state = RotatingShotData::State::Rotate;

						bool all_ready = true;
						for (auto& d : shot_data_L) if (d.state != RotatingShotData::State::Rotate) all_ready = false;
						for (auto& d : shot_data_R) if (d.state != RotatingShotData::State::Rotate) all_ready = false;

						if (all_ready) all_arrived = true;
						break;
					}
					case RotatingShotData::State::Rotate:
					{
						data.angle_deg += angular_speed * delta_second;
						if (data.angle_deg >= 360.0f) data.angle_deg -= 360.0f;

						float rad = data.angle_deg * DX_PI / 180.0f;
						Vector2D offset(cos(rad) * radius, sin(rad) * radius);
						data.shot->SetLocation(center_pos + offset);
						break;
					}
					}
				}
			};

		// ���E�̒e�X�V
		update_shots(shot_data_L, center_pos_L);
		update_shots(shot_data_R, center_pos_R);
	}
	else
	{
		// �U���I������
		for (auto& d : shot_data_L) if (d.shot) d.shot->SetDestroy();
		for (auto& d : shot_data_R) if (d.shot) d.shot->SetDestroy();

		shot_data_L.clear();
		shot_data_R.clear();
		initialized = false;
		all_arrived = false;              // �� ���ꂪ�厖
		duration_timer = 0.0f;
		arrival_phase_timer = 0.0f;       // �� �g���Ă���Ȃ�
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[��11�i��{�r�[���j
/// </summary>
/// <param name="offsets_x">�{�X����̋���</param>
void Boss2::Pattrn11(float offsets_x)
{
	static bool                         beam_on = false;          // ���ܔ��˒����H
	static EnemyBeam1* b;                     // ���˒��r�[��

	if (!beam_on)
	{
		GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

		b = objm->CreateObject<EnemyBeam1>(
			Vector2D(
				location.x + offsets_x,
				(location.y + 150.0f) - box_size.y
			));
		b->SetBoss2(this);
		beam_on = true;
	}

	if (b != nullptr)
	{
		// ���ꂼ�� +100 / -100 �̃I�t�Z�b�g�ŒǏ]
		b->SetLocation(
			Vector2D(
				location.x + offsets_x,
				(location.y + 150.0f) + b->GetBoxSize().y
			));
	}

	if (b != nullptr && b->is_destroy)   // 5 �b��Ȃǂ� true �ɂȂ�z��
	{
		b->SetDestroy();                 // �K�v�Ȃ疾���폜
		is_shot = false;
		beam_on = false;
	}
}

/// <summary>
/// �U���p�^�[��12�i�r�[����{�j
/// </summary>
void Boss2::Pattrn12()
{
	static bool                         beam_on = false;          // ���ܔ��˒����H
	static std::vector<EnemyBeam1*>     beams;                     // ���˒��r�[��
	static const float                  OFFSETS_X[2] = { +100.f, -100.f }; // +100 / -100 �̕���

	if (!beam_on)
	{
		PlaySoundMem(se_beam, DX_PLAYTYPE_BACK);
		GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

		for (int i = 0; i < 2; ++i)
		{
			EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
				Vector2D(
					location.x + OFFSETS_X[i],
					(location.y + 50.0f) - box_size.y
				));
			b->SetBoss2(this);
			beams.push_back(b);
		}
		beam_on = true;
	}

	for (size_t i = 0; i < beams.size(); ++i)
	{
		if (beams[i] == nullptr) continue;   // �O�̂���

		// ���ꂼ�� +100 / -100 �̃I�t�Z�b�g�ŒǏ]
		beams[i]->SetLocation(
			Vector2D(
				location.x + OFFSETS_X[i],
				(location.y + 50.0f) + beams[i]->GetBoxSize().y
			));
	}

	for (auto it = beams.begin(); it != beams.end(); )
	{
		EnemyBeam1* b = *it;

		if (b != nullptr && b->is_destroy)   // 5 �b��Ȃǂ� true �ɂȂ�z��
		{
			b->SetDestroy();                 // �K�v�Ȃ疾���폜
			it = beams.erase(it);            // vector ����O��
		}
		else
		{
			++it;
		}
	}

	// �S���������玟�̍U��������
	if (beam_on && beams.empty())
	{
		is_shot = false;
		beam_on = false;
	}
}

/// <summary>
/// �U���p�^�[��13�i�r�[���̒i�K�U���j�o�O����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void Boss2::Pattrn13(float delta_second)
{
	static int   step = 0;
	static float timer = 0.0f;
	static std::vector<EnemyBeam1*> beams;

	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	timer += delta_second; // ��FPS�Ȃ� delta_second ���g��

	switch (step)
	{
	case 0: // ��1�i�K�F������1�{
	{
		EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
			Vector2D(location.x, location.y - D_OBJECT_SIZE)
		);
		b->SetBoss2(this);
		beams.push_back(b);
		step = 1;
		timer = 0.0f;
	}
	break;

	case 1: // 2�b��ɑ�2�i�K�F2�{�i���E�j
		if (timer >= 2.0f)
		{
			float offsets[] = { +100.0f, -100.0f };
			for (float ox : offsets)
			{
				EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
					Vector2D(location.x + ox, location.y - D_OBJECT_SIZE)
				);
				b->SetBoss2(this);
				beams.push_back(b);
			}
			step = 2;
			timer = 0.0f;
		}
		break;

	case 2: // 2�b��ɑ�3�i�K�F3�{�i�S�́j
		if (timer >= 2.0f)
		{
			float offsets[] = { +200.0f, 0.0f, -200.0f };
			for (float ox : offsets)
			{
				EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
					Vector2D(location.x + ox, location.y - D_OBJECT_SIZE)
				);
				b->SetBoss2(this);
				beams.push_back(b);
			}
			step = 3;
		}
		break;

	case 3: // �r�[���ʒu�Ǐ]�ƏI������
		for (size_t i = 0; i < beams.size(); ++i)
		{
			if (beams[i] == nullptr) continue;

			float offset = 0.0f;
			switch (beams.size())
			{
			case 1: offset = 0.0f; break;
			case 3: offset = (i == 0) ? +100.0f : (i == 1) ? -100.0f : 0.0f; break;
			case 6: offset = (i == 0) ? +0.0f : (i == 1) ? +100.0f : (i == 2) ? -100.0f :
				(i == 3) ? +200.0f : (i == 4) ? 0.0f : -200.0f;
				break;
			}

			beams[i]->SetLocation(
				Vector2D(
					location.x + offset,
					(location.y - D_OBJECT_SIZE) + beams[i]->GetBoxSize().y
				)
			);
		}

		// �r�[�������ׂď�������I��
		for (auto it = beams.begin(); it != beams.end(); )
		{
			EnemyBeam1* b = *it;
			if (b != nullptr && b->is_destroy)
			{
				b->SetDestroy();
				it = beams.erase(it);
			}
			else ++it;
		}

		if (beams.empty())
		{
			is_shot = false;
			step = 0;
			timer = 0.0f;
		}
		break;
	}
}
