#include "Shot.h"
#include "../../GameObjectManager.h"
#include "../../../Utility/ProjectConfig.h"

Shot::Shot()
{
}

Shot::~Shot()
{
}

// 初期化処理
void Shot::Initialize()
{
	z_layer = 4;
	velocity.y = -1000.0f;
	box_size = Vector2D(3.0f, 14.0f);

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::eShot;
	// 当たる相手のオブジェクトタイプ
	collision.hit_object_type.push_back(eObjectType::eEnemy);
	collision.hit_object_type.push_back(eObjectType::eBoss);

	// 動くかどうか（trueなら動く、falseなら止まる）
	is_mobility = true;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	se = rm->GetSounds("Resource/sound/se/shot/shot_01.mp3");
	ChangeVolumeSoundMem(255 * 30 / 100, se);
	PlaySoundMem(se, DX_PLAYTYPE_BACK);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
void Shot::Update(float delta_second)
{
	// 弾が画面外に出たら削除（移動方向によって判定を変える）
	if ((velocity.y < 0 && location.y < -300.0f) || // 上方向
		(velocity.y > 0 && location.y > D_WIN_MAX_Y + 300.0f)) // 下方向
	{
		is_destroy = true; 
	}
	
	if (shot_flip == false)
	{
		velocity.y = -2000.0f;
	}
	else
	{
		velocity.y = 2000.0f;
	}

	location += velocity * delta_second;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void Shot::Draw(const Vector2D& screen_offset) const
{
	// 弾を描画する
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 255), TRUE);
}

// 終了時処理
void Shot::Finalize()
{
}

/// <summary>
/// ヒット時処理
/// </summary>
/// <param name="hit_object">当たった相手</param>
void Shot::OnHitCollision(GameObjectBase* hit_object)
{
	// 当たった相手が弾だったら
	if (hit_object->GetCollision().object_type == eObjectType::eEnemy)
	{
		is_destroy = true;
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBoss)
	{
		is_destroy = true;
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBoss2)
	{
		is_destroy = true;
	}
}

void Shot::SetShotFlip(bool flip)
{
	shot_flip = flip;
}
