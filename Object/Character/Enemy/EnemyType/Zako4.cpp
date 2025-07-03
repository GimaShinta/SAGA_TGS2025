#include "Zako4.h"

Zako4::Zako4()
{}

Zako4::~Zako4()
{}

// 初期化処理
void Zako4::Initialize()
{
	enemy_type = ENE_ZAKO4;
	z_layer = 3;
	velocity.x = 60;
	box_size = Vector2D(32, 32); // サイズを32x32に拡大
	hp = 500;

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	collision.object_type = eObjectType::eEnemy;
	collision.hit_object_type.push_back(eObjectType::eShot);
	collision.hit_object_type.push_back(eObjectType::eBeam);

	is_mobility = true;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">1フレーム当たりの時間</param>
void Zako4::Update(float delta_second)
{
	// 移動処理
	location += velocity * delta_second;

	// 弾を打つ
	Shot(delta_second);

	// 体力がなくなったら削除
	if (hp <= 0)
	{
		is_destroy = true;
	}

	// 親クラスの更新処理を呼び出す
	__super::Update(delta_second);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void Zako4::Draw(const Vector2D& screen_offset) const
{
	// 雑魚4を描画する（緑色のボックス）
	DrawBox(location.x - box_size.x, location.y - box_size.y,
			location.x + box_size.x, location.y + box_size.y,
			GetColor(0, 255, 0), TRUE);

	// 体力の表示（少し上にずらす）
	DrawFormatString(location.x - 16, location.y, GetColor(0, 0, 0), "%.0f", hp);
}

// 終了時処理
void Zako4::Finalize()
{}

// 弾を打つ
void Zako4::Shot(float delta_second)
{
	shot_timer += delta_second;

	// 5秒に一度、移動方向を反転（※実際の弾生成処理は外部管理を想定？）
	if (shot_timer >= 5.0f)
	{
		velocity.y *= -1;
		is_shot = true;
		shot_timer = 0;
	}
}
