#include "Zako2.h"
#include"../../Player/Player.h"

Zako2::Zako2()
{
}

Zako2::~Zako2()
{
}

// ����������
void Zako2::Initialize()
{
	enemy_type = ENE_ZAKO2;
	z_layer = 2;
	box_size = 12;
	hp = 30;

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eEnemy;
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
void Zako2::Update(float delta_second)
{


	pattern_timer += delta_second;

	// �s���p�^�[���ɉ����ċ�����ς���
	switch (pattern)
	{
		case Zako2Pattern::Idle:
			velocity = { 0, 0 };
			break;

		case Zako2Pattern::MoveStraight:
			velocity = { 0, 50 }; // �������ɒ��i
			break;

		case Zako2Pattern::MoveZigzag:
			velocity.x = sinf(pattern_timer * 3.0f) * 30; // ���ɗh���
			velocity.y = 50;
			break;

		case Zako2Pattern::FollowPlayer:
			// ��: �v���C���[�̈ʒu�֌������i�v���C���[�ʒu�͉���j
			Vector2D dir = player->GetLocation() - location;
			dir.Normalize();
			velocity = dir * 60;
			break;
	}

	location += velocity * delta_second;

	// �e��ł�
	Shot(delta_second);

	// HP�`�F�b�N
	if (hp <= 0)
	{
		is_destroy = true;
	}

	__super::Update(delta_second);
}


/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Zako2::Draw(const Vector2D& screen_offset) const
{
	// �G���Q��`�悷��
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(0, 0, 255), TRUE);

	// �̗͂̕\��
	DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
}

// �I��������
void Zako2::Finalize()
{
}

void Zako2::Shot(float delta_second)
{
	shot_timer += delta_second;

	// �ܕb�Ɉ��ł�
	if (shot_timer >= 5.0f)
	{
		is_shot = true;
		shot_timer = 0;
	}
}
