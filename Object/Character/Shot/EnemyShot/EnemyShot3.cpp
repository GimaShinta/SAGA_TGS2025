#include "EnemyShot3.h"
#include "../../../GameObjectManager.h"
#include "../../Player/Player.h"
#include "../../../../Utility/ProjectConfig.h"

EnemyShot3::EnemyShot3() : shot_timer(0.0f), player_target(false), player(nullptr)
{
}

EnemyShot3::~EnemyShot3()
{
}

// 初期化処理
void EnemyShot3::Initialize()
{
	z_layer = 4;
	box_size = 12;

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::eEnemyShot;
	// 当たる相手のオブジェクトタイプ
	collision.hit_object_type.push_back(eObjectType::ePlayer);

	// 動くかどうか（trueなら動く、falseなら止まる）
	is_mobility = true;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
void EnemyShot3::Update(float delta_second)
{
	move_time += delta_second;

	// 弾が画面外に出たら削除（移動方向によって判定を変える）
	if (location.y < -300.0f || // 上方向
		location.y > D_WIN_MAX_Y + 300.0f ||
		location.x > (D_WIN_MAX_X / 2) + 350.0f ||
		location.x < (D_WIN_MAX_X / 2) - 350.0f) // 下方向
	{
		is_destroy = true;
	}

	// 定位置に着いたらストップ
	if (velocity.x > 0 && player_target == false)
	{
		if (location.x >= purpose_location.x)
		{
			velocity.x = 0;
			velocity.y = -240;
		}
	}
	else if (velocity.x < 0 && player_target == false)
	{
		if (location.x <= purpose_location.x)
		{
			velocity.x = 0;
			velocity.y = -240;
		}
	}
	// 定位置に着いたらストップ
	else if (velocity.x == 0 && player_target == false)
	{
		if (location.y <= purpose_location.y)
		{
			velocity.y = 0;
		}
	}

	// ストップしていれば一秒ずつ段階を踏んで弾を打つ
	if (velocity.x == 0 && velocity.y == 0)
	{
		shot_timer += delta_second;

		if (shot_timer >= 1.0f * step_shot)
		{
			player_target = true;
		}
	}

	// 一段階目の攻撃
	if (player_target == true && step_shot == 1)
	{
		if (!has_fired_step1)
		{
			//テキの位置からプレイヤーへのベクトルを求める
			Vector2D b = player->GetLocation() - location;
			float c = sqrt(pow(b.x, 2) + pow(b.y, 2));
			//プレイヤーに向かって弾を打つ
			velocity = (Vector2D(b.x / c, b.y / c));
			velocity *= 500;
			has_fired_step1 = true; // 1回だけ方向を決める
		}
	}
	// 二段階目の攻撃
	else if (player_target == true && step_shot == 2)
	{
		if (!has_fired_step2)
		{
			if (location.y <= player->GetLocation().y)
			{
				velocity = Vector2D(0.0f, 600.0f);
			}
			else
			{
				velocity = Vector2D(0.0f, -600.0f);
			}

			has_fired_step2 = true; // 1回だけ方向を決める
		}
	}
	// 三段階目の攻撃
	else if (player_target == true && step_shot == 3)
	{
		// プレイヤーとのベクトル
		Vector2D to_player = player->GetLocation() - location;
		float distance = sqrt(pow(to_player.x, 2) + pow(to_player.y, 2));

		// 追尾モードがONなら
		if (is_homing == true)
		{
			// 追尾を終了する距離（例：100px以内）
			if (distance < 150.0f)
			{
				is_homing = false; // 追尾終了！
				// 追尾していた最後の方向で velocity を固定
				// 何もしなければ今の velocity のまま直進
			}
			else
			{
				// プレイヤー方向に向かってベクトルを更新
				Vector2D direction = to_player / distance;
				velocity = direction * 500.0f;
			}
		}
	}

	location += velocity * delta_second;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void EnemyShot3::Draw(const Vector2D& screen_offset) const
{
	float angle = move_time * 10.0f; // 回転速度
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

// 終了時処理
void EnemyShot3::Finalize()
{
}

void EnemyShot3::SetPlayerLocation(const Vector2D& p_location)
{
	player_location = p_location;
}

void EnemyShot3::SetPurposeLocation(const Vector2D& p_location)
{
	purpose_location = p_location;
}

void EnemyShot3::SetPlayer(Player* p_player)
{
	player = p_player;
}

void EnemyShot3::SetStepShot(int step)
{
	step_shot = step;
}
