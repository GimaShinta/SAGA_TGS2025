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

// 初期化処理
void Beam::Initialize()
{
	z_layer = 4;
	box_size = Vector2D(36.0f, 800.0f);

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::eBeam;
	// 当たる相手のオブジェクトタイプ
	collision.hit_object_type.push_back(eObjectType::eEnemy);
	collision.hit_object_type.push_back(eObjectType::eBoss);

	// 動くかどうか（trueなら動く、falseなら止まる）
	is_mobility = true;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
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
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void Beam::Draw(const Vector2D& screen_offset) const
{
	// 弾を描画する
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(255, 15, 192), TRUE);
}

// 終了時処理
void Beam::Finalize()
{
}

/// <summary>
/// ヒット時処理
/// </summary>
/// <param name="hit_object">当たった相手</param>
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
