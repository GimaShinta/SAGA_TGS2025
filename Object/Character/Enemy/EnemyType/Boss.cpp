#include "Boss.h"
#include "../../../../Utility/ProjectConfig.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

// ����������
void Boss::Initialize()
{
	enemy_type = ENE_BOSS;
	z_layer = 1;
	velocity.y = -350;
	box_size = 30;
	hp = 10;

	base_position = Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) - 200); // ���O���̒��S�ʒu
	generate_base_position = Vector2D(D_WIN_MAX_X / 2, -200); // �o�ꎞ�̈ʒu

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eBoss;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::eShot);
	collision.hit_object_type.push_back(eObjectType::eBeam);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void Boss::Update(float delta_second)
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

	// �U���p�^�[���ύX���Ɏ��ԃ��Z�b�g
	if (attack_pattrn != prev_attack_pattrn)
	{
		if (prev_attack_pattrn == 3)
		{
			move_time = 0.0f; // �O���̐擪�ɖ߂��I
		}
		prev_attack_pattrn = attack_pattrn;
	}

	// �e�N���X�̍X�V�������Ăяo��
	__super::Update(delta_second);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Boss::Draw(const Vector2D& screen_offset) const
{
	//// ��]�p�i�b���ɉ����ĉ񂷁j
	//float angle = move_time * 1.5f; // ��]���x�͒���OK�i���W�A���j
	//float half_size = box_size.x;

	//float cosA = cos(angle);
	//float sinA = sin(angle);

	//// ���S����̑��΍��W����]
	//Vector2D p1 = Vector2D(-half_size, -half_size);
	//Vector2D p2 = Vector2D(half_size, -half_size);
	//Vector2D p3 = Vector2D(half_size, half_size);
	//Vector2D p4 = Vector2D(-half_size, half_size);

	//auto rotate = [&](Vector2D p) {
	//	return Vector2D(
	//		location.x + p.x * cosA - p.y * sinA,
	//		location.y + p.x * sinA + p.y * cosA
	//	);
	//	};

	//// �e�_����]��̈ʒu��
	//p1 = rotate(p1);
	//p2 = rotate(p2);
	//p3 = rotate(p3);
	//p4 = rotate(p4);

	//// ��]�l�p�`��2�̎O�p�`�ŕ`��
	//int color = GetColor(255, 0, 255);
	//DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color, TRUE);
	//DrawTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, color, TRUE);

	// �̗͂̕\���i���S�t�߂Ɂj
	DrawFormatString(location.x - 15, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
}

// �I��������
void Boss::Finalize()
{
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss::Movement(float delta_second)
{
//	move_time += delta_second;
//
//	float float_offset = 8.0f * sin(move_time * 2.5f); // ���V��
//
//	// �U���p�^�[�����Ƃ̓�������
//	if (attack_pattrn == 1 ||
//		attack_pattrn == 2)
//	{
//#if 0
//		float a = 250.0f; // ����
//		float b = 50.0f;  // �c��
//		float t = move_time * 1.0f; // �������߂ɕύX�I
//
//		location.x = base_position.x + a * sin(t);
//		location.y = base_position.y + b * sin(t) * cos(t);
//#else
//		float a = 250.0f; // ����
//		float b = 50.0f;  // �c��
//		float t = move_time * 1.0f;
//
//		float float_offset = 8.0f * sin(move_time * 2.5f); // ���V���̃I�t�Z�b�g
//
//		location.x = base_position.x + a * sin(t);
//		location.y = base_position.y + b * sin(t) * cos(t) + float_offset; // Y�ɕ��V�ǉ�
//#endif
//	}
//	else if (attack_pattrn == 3)
//	{
//		//// �U���p�^�[��3�̂Ƃ��͒�~
//		//location = base_position;
//		//velocity = 0;
//		//location += velocity * delta_second;
//
//		// ���V�����̓����ɂ���ivelocity�͎g��Ȃ��j
//		location.y = base_position.y + float_offset;
//
//		// �������̂�邢�X�C���O��ǉ��������ꍇ
//		location.x = base_position.x + 10.0f * sin(move_time);
//
//	}
//	else
//	{
//		if (generate == false)
//		{
//			velocity.y = -500;
//			if (location.y < generate_base_position.y)
//			{
//				generate = true;
//			}
//		}
//		else
//		{
//			//location.x = generate_base_position.x;
//			box_size = 60;
//			velocity.y = 80;
//
//			// �������̂�邢�X�C���O
//			float swing_offset = 2.0f * sin(move_time * 5.0f);
//			location.x = generate_base_position.x + swing_offset;
//
//			if (location.y > base_position.y)
//			{
//				generate_time += delta_second;
//				move_time = 0.0f;
//				velocity.y = 0;
//				collision.is_blocking = true;
//
//				// �������̂�邢�X�C���O
//				float swing_offset = sin(move_time * 2.0f);
//				location.x = generate_base_position.x + swing_offset;
//				//// ���V�����̓����ɂ���ivelocity�͎g��Ȃ��j
//				//location.y = base_position.y + float_offset;
//			}
//		}
//		location += velocity * delta_second;
//	}


}

void Boss::Shot(float delta_second)
{
	shot_timer += delta_second;

	// �ܕb�o�߂�����U���p�^�[����ύX���Ēe�𔭎�
	if (shot_timer >= 5.0f && generate_time >= 2.0f)
	{
		attack_pattrn = 1 + rand() % 3;
		is_shot = true;
		shot_timer = 0;
		generate_time = 10.0f;
	}
}

int Boss::GetAttackPattrn() const
{
	return attack_pattrn;
}

int Boss::GetIsAlive() const
{
	return is_alive;
}
