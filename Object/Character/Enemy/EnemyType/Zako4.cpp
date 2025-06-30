#include "Zako4.h"

Zako4::Zako4()
{}

Zako4::~Zako4()
{}

// ����������
void Zako4::Initialize()
{
	enemy_type = ENE_ZAKO4;
	z_layer = 3;
	velocity.x = 60;
	box_size = Vector2D(32, 32); // �T�C�Y��32x32�Ɋg��
	hp = 500;

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	collision.object_type = eObjectType::eEnemy;
	collision.hit_object_type.push_back(eObjectType::eShot);
	collision.hit_object_type.push_back(eObjectType::eBeam);

	is_mobility = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void Zako4::Update(float delta_second)
{
	// �ړ�����
	location += velocity * delta_second;

	// �e��ł�
	Shot(delta_second);

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
void Zako4::Draw(const Vector2D& screen_offset) const
{
	// �G��4��`�悷��i�ΐF�̃{�b�N�X�j
	DrawBox(location.x - box_size.x, location.y - box_size.y,
			location.x + box_size.x, location.y + box_size.y,
			GetColor(0, 255, 0), TRUE);

	// �̗͂̕\���i������ɂ��炷�j
	DrawFormatString(location.x - 16, location.y, GetColor(0, 0, 0), "%.0f", hp);
}

// �I��������
void Zako4::Finalize()
{}

// �e��ł�
void Zako4::Shot(float delta_second)
{
	shot_timer += delta_second;

	// 5�b�Ɉ�x�A�ړ������𔽓]�i�����ۂ̒e���������͊O���Ǘ���z��H�j
	if (shot_timer >= 5.0f)
	{
		velocity.y *= -1;
		is_shot = true;
		shot_timer = 0;
	}
}
