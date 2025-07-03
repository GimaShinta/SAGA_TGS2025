#include "EnemyShot1.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"
#include <cmath>

EnemyShot1::EnemyShot1()
{}

EnemyShot1::~EnemyShot1()
{}

void EnemyShot1::Initialize()
{
	z_layer = 4;

	// �r�[�����̓����蔻��i�ג��������`�j
	box_size = Vector2D(4, 24);  // ��4 �~ �c24

	collision.is_blocking = true;
	collision.object_type = eObjectType::eEnemyShot;
	collision.hit_object_type.push_back(eObjectType::ePlayer);

	is_mobility = true;
	life_time = 0.1f;  // �\����0.1�b
}

void EnemyShot1::Update(float delta_second)
{
	move_time += delta_second;

	location += velocity * delta_second;

	// ��ʊO�Ȃ�폜
	if (location.y < -300.0f || location.y > D_WIN_MAX_Y + 300.0f ||
		location.x > (D_WIN_MAX_X / 2) + 350.0f ||
		location.x < (D_WIN_MAX_X / 2) - 350.0f)
	{
		is_destroy = true;
	}
}

void EnemyShot1::Draw(const Vector2D& screen_offset) const
{
	
		// �S�����e�F��������
		DrawCircle(static_cast<int>(location.x), static_cast<int>(location.y), 6, GetColor(255, 255, 255), TRUE);
		return;

	Vector2D dir = velocity.Normalize();
	Vector2D end = location + dir * 600.0f;  // �� �r�[���̒����i�����j

	int color = GetColor(255, 255, 0);
	DrawLineAA(location.x, location.y, end.x, end.y, color, 8.0f); // ����8�ŕ`��

}


void EnemyShot1::Finalize()
{}
