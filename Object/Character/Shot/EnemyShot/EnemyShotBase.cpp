#include "EnemyShotBase.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"

EnemyShotBase::EnemyShotBase()
{
}

EnemyShotBase::~EnemyShotBase()
{
}

// 初期化処理
void EnemyShotBase::Initialize()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
void EnemyShotBase::Update(float delta_second)
{
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void EnemyShotBase::Draw(const Vector2D& screen_offset) const
{
	// 弾の回転角（進行方向によって回転速度変えてもOK）
	float angle = move_time * 10.0f; // 速めに回転させて弾っぽく
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

	int color = GetColor(0, 255, 255);
	DrawTriangle(p1.x, p2.y, p2.x, p2.y, p3.x, p3.y, color, TRUE);
	DrawTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, color, TRUE);
}

// 終了時処理
void EnemyShotBase::Finalize()
{
}

/// <summary>
/// ヒット時処理
/// </summary>
/// <param name="hit_object">当たった相手</param>
void EnemyShotBase::OnHitCollision(GameObjectBase* hit_object)
{
	// 当たった相手が弾だったら
	if (hit_object->GetCollision().object_type == eObjectType::ePlayer)
	{
		is_destroy = true;
	}
}
