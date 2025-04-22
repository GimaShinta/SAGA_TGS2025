#include "EnemyShot5.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"

EnemyShot5::EnemyShot5()
{
}

EnemyShot5::~EnemyShot5()
{
}

// ����������
void EnemyShot5::Initialize()
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

	wave_amplitude = 30.0f; // �����U�ꕝ
	wave_frequency = 4.0f;  // �������g��
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void EnemyShot5::Update(float delta_second)
{
	move_time += delta_second;

#if 0
	switch (shot_state)
	{
	case ShotState::SpreadOut:
		location += velocity * delta_second;
		if (move_time > 1.0f)
		{
			shot_state = ShotState::PullIn;
			move_time = 0.0f;

			// ���S�ɋz�����܂��isuck_center�Ɍ������j
			velocity = (suck_center - location).Normalize() * 100.0f;
		}
		break;

	case ShotState::PullIn:
		location += velocity * delta_second;

		// ���S�ɂقڋ߂Â����玟�̃X�e�[�g��
		if ((suck_center - location).Length() < 5.0f)
		{
			shot_state = ShotState::Explode;
			move_time = 0.0f;

			// �����_���ɒe����
			float angle = static_cast<float>(GetRand(359));
			velocity = Vector2D(cosf(angle * DX_PI / 180.0f), sinf(angle * DX_PI / 180.0f)) * 200.0f;
		}
		break;

	case ShotState::Explode:
		location += velocity * delta_second;
		break;
	}

#else
	base_velocity = velocity;
	// �g�̋O����������
	float offset = sinf(move_time * wave_frequency * 2 * DX_PI) * wave_amplitude;
	if (is_wave_reflected)
		offset *= -1.0f;
	Vector2D sideway = Vector2D(-base_velocity.y, base_velocity.x).Normalize(); // ���p����
	location += base_velocity * delta_second + sideway * offset * delta_second;


#endif

	// �e����ʊO�ɏo����폜�i�ړ������ɂ���Ĕ����ς���j
	if (location.y < -300.0f || // �����
		location.y > D_WIN_MAX_Y + 300.0f ||
		location.x > (D_WIN_MAX_X / 2) + 350.0f ||
		location.x < (D_WIN_MAX_X / 2) - 350.0f) // ������
	{
		is_destroy = true;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void EnemyShot5::Draw(const Vector2D& screen_offset) const
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
void EnemyShot5::Finalize()
{
}

void EnemyShot5::SetSuckCenter(const Vector2D& center)
{
	suck_center = center;
}

void EnemyShot5::SetWaveParameters(float amplitude, float frequency)
{
	wave_amplitude = amplitude;
	wave_frequency = frequency;
}

void EnemyShot5::SetWaveReflected(bool reflected)
{
	is_wave_reflected = reflected;
}
