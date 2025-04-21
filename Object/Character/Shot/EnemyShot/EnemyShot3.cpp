#include "EnemyShot3.h"
#include "../../../GameObjectManager.h"
#include "../../Player/Player.h"
#include "../../../../Utility/ProjectConfig.h"

EnemyShot3::EnemyShot3() : shot_timer(0.0f), player_target(false), player(nullptr)
{
}

EnemyShot3::~EnemyShot3()
{
}

// ����������
void EnemyShot3::Initialize()
{
	z_layer = 4;
	box_size = 12;

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eEnemyShot;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::ePlayer);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void EnemyShot3::Update(float delta_second)
{
	move_time += delta_second;

	// �e����ʊO�ɏo����폜�i�ړ������ɂ���Ĕ����ς���j
	if (location.y < -300.0f || // �����
		location.y > D_WIN_MAX_Y + 300.0f ||
		location.x > (D_WIN_MAX_X / 2) + 350.0f ||
		location.x < (D_WIN_MAX_X / 2) - 350.0f) // ������
	{
		is_destroy = true;
	}

	// ��ʒu�ɒ�������X�g�b�v
	if (velocity.x > 0 && player_target == false)
	{
		if (location.x >= purpose_location.x)
		{
			velocity.x = 0;
			velocity.y = -240;
		}
	}
	else if (velocity.x < 0 && player_target == false)
	{
		if (location.x <= purpose_location.x)
		{
			velocity.x = 0;
			velocity.y = -240;
		}
	}
	// ��ʒu�ɒ�������X�g�b�v
	else if (velocity.x == 0 && player_target == false)
	{
		if (location.y <= purpose_location.y)
		{
			velocity.y = 0;
		}
	}

	// �X�g�b�v���Ă���Έ�b���i�K�𓥂�Œe��ł�
	if (velocity.x == 0 && velocity.y == 0)
	{
		shot_timer += delta_second;

		if (shot_timer >= 1.0f * step_shot)
		{
			player_target = true;
		}
	}

	// ��i�K�ڂ̍U��
	if (player_target == true && step_shot == 1)
	{
		if (!has_fired_step1)
		{
			//�e�L�̈ʒu����v���C���[�ւ̃x�N�g�������߂�
			Vector2D b = player->GetLocation() - location;
			float c = sqrt(pow(b.x, 2) + pow(b.y, 2));
			//�v���C���[�Ɍ������Ēe��ł�
			velocity = (Vector2D(b.x / c, b.y / c));
			velocity *= 500;
			has_fired_step1 = true; // 1�񂾂����������߂�
		}
	}
	// ��i�K�ڂ̍U��
	else if (player_target == true && step_shot == 2)
	{
		if (!has_fired_step2)
		{
			if (location.y <= player->GetLocation().y)
			{
				velocity = Vector2D(0.0f, 600.0f);
			}
			else
			{
				velocity = Vector2D(0.0f, -600.0f);
			}

			has_fired_step2 = true; // 1�񂾂����������߂�
		}
	}
	// �O�i�K�ڂ̍U��
	else if (player_target == true && step_shot == 3)
	{
		// �v���C���[�Ƃ̃x�N�g��
		Vector2D to_player = player->GetLocation() - location;
		float distance = sqrt(pow(to_player.x, 2) + pow(to_player.y, 2));

		// �ǔ����[�h��ON�Ȃ�
		if (is_homing == true)
		{
			// �ǔ����I�����鋗���i��F100px�ȓ��j
			if (distance < 150.0f)
			{
				is_homing = false; // �ǔ��I���I
				// �ǔ����Ă����Ō�̕����� velocity ���Œ�
				// �������Ȃ���΍��� velocity �̂܂ܒ��i
			}
			else
			{
				// �v���C���[�����Ɍ������ăx�N�g�����X�V
				Vector2D direction = to_player / distance;
				velocity = direction * 500.0f;
			}
		}
	}

	location += velocity * delta_second;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void EnemyShot3::Draw(const Vector2D& screen_offset) const
{
	float angle = move_time * 10.0f; // ��]���x
	float hw = box_size.x;
	float hh = box_size.y;

	float cosA = cos(angle);
	float sinA = sin(angle);

	Vector2D p1(-hw, -hh);
	Vector2D p2(hw, -hh);
	Vector2D p3(hw, hh);
	Vector2D p4(-hw, hh);

	auto rotate = [&](Vector2D p) {
		return Vector2D(
			location.x + p.x * cosA - p.y * sinA,
			location.y + p.x * sinA + p.y * cosA
		);
		};

	p1 = rotate(p1);
	p2 = rotate(p2);
	p3 = rotate(p3);
	p4 = rotate(p4);

	int color = GetColor(255, 255, 0);
	DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color, TRUE);
	DrawTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, color, TRUE);
}

// �I��������
void EnemyShot3::Finalize()
{
}

void EnemyShot3::SetPlayerLocation(const Vector2D& p_location)
{
	player_location = p_location;
}

void EnemyShot3::SetPurposeLocation(const Vector2D& p_location)
{
	purpose_location = p_location;
}

void EnemyShot3::SetPlayer(Player* p_player)
{
	player = p_player;
}

void EnemyShot3::SetStepShot(int step)
{
	step_shot = step;
}
