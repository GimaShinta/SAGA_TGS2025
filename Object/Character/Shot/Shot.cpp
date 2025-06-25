#include "Shot.h"
#include "../../GameObjectManager.h"
#include "../../../Utility/ProjectConfig.h"
#include "../../../Utility/AnimationManager.h"
#include"../../../Object/Character/Enemy/EnemyType/Stage2BOSS.h"

Shot::Shot()
{
}

Shot::~Shot()
{
}

// 初期化処理
void Shot::Initialize()
{
	z_layer = 3;
	velocity.y = -1000.0f;
	box_size = Vector2D(5.0f, 14.0f);

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::eShot;
	// 当たる相手のオブジェクトタイプ
	collision.hit_object_type.push_back(eObjectType::eEnemy);
	collision.hit_object_type.push_back(eObjectType::eBoss);
	collision.hit_object_type.push_back(eObjectType::eEnemyPart);

	// 動くかどうか（trueなら動く、falseなら止まる）
	is_mobility = true;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	se = rm->GetSounds("Resource/sound/se/shot/shot_02.mp3");
	image = rm->GetImages("Resource/Image/Object/Player/Shot/anime_effect16.png", 6, 6, 1, 8, 88)[0];

	//AnimationManager* am = Singleton<AnimationManager>::GetInstance();
	//int anim_id = 0;
	//anim_id = am->PlayerAnimation(EffectName::eExprotion, location, 0.05f, false);
	//// アニメーションの追加設定
	//am->SetAlpha(anim_id, 122);       // 半透明
	//am->SetScale(anim_id, 0.1f);      // 1.5倍拡大
	hit_se = rm->GetSounds("Resource/sound/se/se_effect/reaction.mp3");
	ChangeVolumeSoundMem(255 * 110 / 100, hit_se);
	ChangeVolumeSoundMem(255 * 60 / 100, se);
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
	//// 弾を描画する
	//DrawBox(location.x - box_size.x, location.y - box_size.y,
	//	location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 255), TRUE);

	DrawRotaGraph(location.x, location.y, 1.1f, 0.0f, image, TRUE);

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
	AnimationManager* am = Singleton<AnimationManager>::GetInstance();
	int anim_id = 0;
	// 当たった相手が弾だったら
	if (hit_object->GetCollision().object_type == eObjectType::eEnemy)
	{
		float random_x = static_cast<float>(GetRand(20));
		if (GetRand(2) == 1)
		{
			PlaySoundMem(hit_se, DX_PLAYTYPE_BACK);
			random_x *= -1;
		}
		float random_y = static_cast<float>(GetRand(15));

		anim_id = am->PlayerAnimation(EffectName::eExprotion2, Vector2D(location.x - random_x, location.y - random_y), 0.01f, false);
		// アニメーションの追加設定
		am->SetAlpha(anim_id, 255);       // 半透明
		am->SetScale(anim_id, 0.2f);      // 1.5倍拡大

		is_destroy = true;
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBoss)
	{
		is_destroy = true;
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBoss2 ||
		hit_object->GetCollision().object_type == eObjectType::eBoss3)
	{
		PlaySoundMem(hit_se, DX_PLAYTYPE_BACK);
		float random_x = static_cast<float>(GetRand(100));

		anim_id = am->PlayerAnimation(EffectName::eExprotion2, Vector2D(location.x, location.y - random_x), 0.01f, false);
		// アニメーションの追加設定
		am->SetAlpha(anim_id, 255);       // 半透明
		am->SetScale(anim_id, 0.2f);      // 1.5倍拡大

		is_destroy = true;
	}

	if (hit_object->GetCollision().object_type == eObjectType::eEnemyPart)
	{
		anim_id = am->PlayerAnimation(EffectName::eExprotion2, location, 0.01f, false);
		am->SetAlpha(anim_id, 255);
		am->SetScale(anim_id, 0.2f);
		is_destroy = true;
	}
	if (hit_object->GetCollision().object_type == eObjectType::eBoss2)
	{
		Stage2Boss* boss = dynamic_cast<Stage2Boss*>(hit_object);
		if (boss)
		{
			boss->TakeDamage(10); // ダメージを与える
		}

		is_destroy = true;
	}

	
}

void Shot::SetShotFlip(bool flip)
{
	shot_flip = flip;
}
