#include "Zako2.h"

Zako2::Zako2()
{
}

Zako2::~Zako2()
{
}

// 初期化処理
void Zako2::Initialize()
{
	enemy_type = ENE_ZAKO2;
	z_layer = 2;
	box_size = 12;
	hp = 30;

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::eEnemy;
	// 当たる相手のオブジェクトタイプ
	collision.hit_object_type.push_back(eObjectType::eShot);
	collision.hit_object_type.push_back(eObjectType::eBeam);

	// 動くかどうか（trueなら動く、falseなら止まる）
	is_mobility = true;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
void Zako2::Update(float delta_second)
{
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
void Zako2::Draw(const Vector2D& screen_offset) const
{
	// 雑魚２を描画する
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(0, 0, 255), TRUE);

	// 体力の表示
	DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
}

// 終了時処理
void Zako2::Finalize()
{
}

void Zako2::Shot(float delta_second)
{
	shot_timer += delta_second;

	// 五秒に一回打つ
	if (shot_timer >= 5.0f)
	{
		is_shot = true;
		shot_timer = 0;
	}
}
