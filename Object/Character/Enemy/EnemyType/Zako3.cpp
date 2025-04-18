#include "Zako3.h"

Zako3::Zako3()
{
}

Zako3::~Zako3()
{
}

// ����������
void Zako3::Initialize()
{
	enemy_type = ENE_ZAKO3;
	z_layer = 2;
	velocity.y = 100 + rand() % 20;
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
void Zako3::Update(float delta_second)
{
	location += velocity * delta_second;

	// �̗͂��Ȃ��Ȃ�����폜
	if (hp <= 0)
	{
		is_destroy = true;
	}

	// �e�N���X�̍X�V�������Ăяo��
	__super::Update(delta_second);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Zako3::Draw(const Vector2D& screen_offset) const
{
	// �G���R��`�悷��
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);

	// �̗͂̕\��
	DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
}

// �I��������
void Zako3::Finalize()
{
}

void Zako3::Shot(float delta_second)
{
}

void Zako3::SetFlip(bool flip)
{
	zako3_flip = flip;
}
