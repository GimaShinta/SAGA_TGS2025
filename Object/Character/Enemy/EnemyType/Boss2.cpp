#include "Boss2.h"
#include "../../../../Utility/ProjectConfig.h"

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
	z_layer = 1;
	box_size = 30;
	hp = 100;

	// �U���p�^�[���̐ݒ�
	attack_pattrn_num = { 0 };

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eBoss2;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::eShot);
	collision.hit_object_type.push_back(eObjectType::eBeam);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;

	// �퓬���̒��S���W
	base_position = Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) - 200);

	// �o�ꎞ�̒��S���W
	generate_base_position = Vector2D(D_WIN_MAX_X / 2 + 150, D_WIN_MAX_Y + 200);

	// �������W�͓o�ꎞ�̂��̂��g�p
	location = generate_base_position;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	images = rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_b.png", 12, 12, 1, 24, 24);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void Boss2::Update(float delta_second)
{
	// �A�j���[�V����
	GameObjectBase::AnimationControl(delta_second, images, { 0, 1, 2, 3, 4, 5, 4, 3 ,2 ,1 }, 100.0f);
	// �U���p�^�[����ݒ肵�Ēe��ł�
	Shot(delta_second);

	// �ړ�����
	Movement(delta_second);

	// �̗͂��Ȃ��Ȃ�����폜
	if (hp <= 0)
	{
		is_alive = false;
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

	// �e�N���X�̍X�V�������Ăяo��
	__super::Update(delta_second);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Boss2::Draw(const Vector2D& screen_offset) const
{
	// �G���P��`�悷��
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);

	// �̗͂̕\��
	DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);

	DrawRotaGraph(location.x, location.y, 10.0f, 0.0f, image, TRUE);

}

// �I��������
void Boss2::Finalize()
{
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss2::Movement(float delta_second)
{
	move_time += delta_second;

	const float float_amplitude_y = 30.0f;
	const float float_speed_y = 1.5f;
	const float float_amplitude_x = 20.0f;
	const float float_speed_x = 1.5f;
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
			box_size = 60;
		}
		else
		{
			generate2 = true;

			const float swing_range = 100.0f;
			const float move_speed = 100.0f;
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
	const int shot_interval = 0.1f;

	// ���Ԍo�߂�����U���p�^�[����ύX���Ēe�𔭎�
	if (generate2 == true && is_shot == false)
	{
		shot_timer += delta_second;
		if (shot_timer >= shot_interval)
		{
#if 1

	#if 0
			// ����̍U���̂݌J��Ԃ�
			attack_pattrn = 5;
	#else
			// HP����������U���p�^�[����ύX�i�I�[�o�[�t���[�h�~�ɍ��킹�ă��Z�b�g�j
			if (hp <= 50 && attack_pattrn_num != std::vector<int>{7, 8})
			{
				attack_pattrn_num = { 7, 8 };
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
}

int Boss2::GetAttackPattrn() const
{
	return attack_pattrn;
}

int Boss2::GetIsAlive() const
{
	return is_alive;
}
