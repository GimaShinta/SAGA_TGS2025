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
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void Boss2::Update(float delta_second)
{
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

	// ���V���F�T�C���g�ɂ����ׂȕω�
	const float float_amplitude_y = 10.0f;
	const float float_speed_y = 2.0f;
	const float float_amplitude_x = 5.0f;
	const float float_speed_x = 1.5f;

	Vector2D float_offset;
	float_offset.y = sinf(move_time * float_speed_y) * float_amplitude_y;
	float_offset.x = cosf(move_time * float_speed_x) * float_amplitude_x;

	if (!generate)
	{
		// �o��O�̍����ړ�
		velocity.y = -700.0f;
		base_position.x = generate_base_position.x;

		// ��ʒu�ɓ���������o�ꂷ��悤�ɐݒ�
		if (location.y < -400)
		{
			generate = true;
		}
	}
	else
	{
		// ��ʒu�ւ̈ړ������i�ӂ���Ǝ~�܂�j
		float distance_y = base_position.y - location.y;
		base_position.x = generate_base_position.x - 150;
		generate_time += delta_second;

		if (fabs(distance_y) > 1.0f)
		{
			const float max_speed = 200.0f;
			velocity.y = distance_y;

			// �ő呬�x�𒴂��Ȃ��悤����
			if (velocity.y > max_speed) velocity.y = max_speed;
			if (velocity.y < -max_speed) velocity.y = -max_speed;

			box_size = 60;
		}
		else
		{
			velocity = 0;
			location = base_position; // �덷�C��
		}
	}

	// base_position ���X�V���A�ŏI�I�ȍ��W�ɕ��V�I�t�Z�b�g�����Z
	generate_base_position += velocity * delta_second;
	location.y = generate_base_position.y + float_offset.y;
	location.x = base_position.x + float_offset.x;
}

void Boss2::Shot(float delta_second)
{
	shot_timer += delta_second;

	// �ܕb�o�߂�����U���p�^�[����ύX���Ēe�𔭎�
	if (shot_timer >= 5.0f && generate_time >= 5.0f)
	{
		attack_pattrn = 3;
		//attack_pattrn = 1 + rand() % MAX_ATTACK_PATTRN;
		is_shot = true;
		shot_timer = 0;
		generate_time = 10.0f;
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
