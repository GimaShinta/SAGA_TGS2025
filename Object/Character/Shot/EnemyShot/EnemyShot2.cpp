#include "EnemyShot2.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"

EnemyShot2::EnemyShot2()
{
}

EnemyShot2::~EnemyShot2()
{
}

// ����������
void EnemyShot2::Initialize()
{
	z_layer = 4;
	box_size = 6;

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eEnemyShot;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::ePlayer);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	image = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Orange/standard/bullet73.png")[0];
	se = rm->GetSounds("Resource/sound/se/shot/shot_01.mp3");
	ChangeVolumeSoundMem(255 * 90 / 100, se);
	

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void EnemyShot2::Update(float delta_second)
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

	location += (velocity * 200) * delta_second;
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void EnemyShot2::Draw(const Vector2D& screen_offset) const
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
	//DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color, TRUE);
	//DrawTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, color, TRUE);

	PlaySoundMem(se, DX_PLAYTYPE_BACK);
	DrawRotaGraph(location.x, location.y, 1.9f, angle, image, TRUE);

}

// �I��������
void EnemyShot2::Finalize()
{
}