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

	// ビーム風の当たり判定（細長い長方形）
	box_size = Vector2D(4, 24);  // 横4 × 縦24

	collision.is_blocking = true;
	collision.object_type = eObjectType::eEnemyShot;
	collision.hit_object_type.push_back(eObjectType::ePlayer);

	is_mobility = true;
	life_time = 0.1f;  // 表示は0.1秒
}

void EnemyShot1::Update(float delta_second)
{
	move_time += delta_second;

	location += velocity * delta_second;

	// 画面外なら削除
	if (location.y < -300.0f || location.y > D_WIN_MAX_Y + 300.0f ||
		location.x > (D_WIN_MAX_X / 2) + 350.0f ||
		location.x < (D_WIN_MAX_X / 2) - 350.0f)
	{
		is_destroy = true;
	}
}

void EnemyShot1::Draw(const Vector2D& screen_offset) const
{
	
		// 全方向弾：白い球体
		DrawCircle(static_cast<int>(location.x), static_cast<int>(location.y), 6, GetColor(255, 255, 255), TRUE);
		return;

	Vector2D dir = velocity.Normalize();
	Vector2D end = location + dir * 600.0f;  // ★ ビームの長さ（調整可）

	int color = GetColor(255, 255, 0);
	DrawLineAA(location.x, location.y, end.x, end.y, color, 8.0f); // 太さ8で描画

}


void EnemyShot1::Finalize()
{}
