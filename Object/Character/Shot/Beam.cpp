#include "Beam.h"
#include "../../GameObjectManager.h"
#include "../../../Utility/ProjectConfig.h"
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
	z_layer = 4;
	box_size = Vector2D(36.0f, 800.0f);

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eBeam;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::eEnemy);
	collision.hit_object_type.push_back(eObjectType::eBoss);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void Beam::Update(float delta_second)
{
	if (player != nullptr)
	{
		location.x = player->GetLocation().x;
		if (Beam_flip == false)
		{
			location.y = (player->GetLocation().y - D_OBJECT_SIZE) - box_size.y;
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

	location += velocity * delta_second;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Beam::Draw(const Vector2D& screen_offset) const
{
	// �e��`�悷��
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(255, 15, 192), TRUE);
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
}

void Beam::SetBeamFlip(bool flip)
{
	Beam_flip = flip;
}

void Beam::SetPlayer(Player* p_player)
{
	player = p_player;
}
