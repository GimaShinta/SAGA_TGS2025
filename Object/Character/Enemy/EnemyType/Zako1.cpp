#include "Zako1.h"
#include "../../../Item/Exp/Exp.h"
#include "../../../Character/Player/Player.h"

Zako1::Zako1()
{
}

Zako1::~Zako1()
{
}

// ����������
void Zako1::Initialize()
{
	enemy_type = ENE_ZAKO1;
	z_layer = 2;
	velocity.y = 60;
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
void Zako1::Update(float delta_second)
{
	// �ړ�����
	location += velocity * delta_second;

	// �e��ł�
	Shot(delta_second);

	// �̗͂��Ȃ��Ȃ�����폜
	if (hp <= 0)
	{
		is_destroy = true;

		ScoreData* score = Singleton<ScoreData>::GetInstance();
		score->SetScoreData(100);

		// �o���l�A�C�e���𐶐����ēo�^
		ItemExp* exp = Singleton<GameObjectManager>::GetInstance()->CreateObject<ItemExp>(this->location);
	}

	// �e�N���X�̍X�V�������Ăяo��
	__super::Update(delta_second);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Zako1::Draw(const Vector2D& screen_offset) const
{
	// �G���P��`�悷��
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 0), TRUE);

	// �̗͂̕\��
	DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
}

// �I��������
void Zako1::Finalize()
{
}

// �e��ł�
void Zako1::Shot(float delta_second)
{
	shot_timer += delta_second;

	// �ܕb�Ɉ��ł�
	if (shot_timer >= 5.0f)
	{
		velocity.y *= -1;
		is_shot = true;
		shot_timer = 0;
	}
}
