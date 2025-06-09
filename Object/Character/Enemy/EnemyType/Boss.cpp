#include "Boss.h"
#include "../../../../Utility/ProjectConfig.h"

Boss::Boss()
{
}

Boss::~Boss()
{
}

// 初期化処理
void Boss::Initialize()
{
	enemy_type = ENE_BOSS;
	z_layer = 1;
	velocity.y = -350;
	box_size = 30;
	hp = 10;

	base_position = Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) - 200); // ∞軌道の中心位置
	generate_base_position = Vector2D(D_WIN_MAX_X / 2, -200); // 登場時の位置

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::eBoss;
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
void Boss::Update(float delta_second)
{
	// 攻撃パターンを設定して弾を打つ
	Shot(delta_second);

	// 移動処理
	Movement(delta_second);

	// 体力がなくなったら削除
	if (hp <= 0)
	{
		is_alive = false;
	}

	// 攻撃パターン変更時に時間リセット
	if (attack_pattrn != prev_attack_pattrn)
	{
		if (prev_attack_pattrn == 3)
		{
			move_time = 0.0f; // 軌道の先頭に戻す！
		}
		prev_attack_pattrn = attack_pattrn;
	}

	// 親クラスの更新処理を呼び出す
	__super::Update(delta_second);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void Boss::Draw(const Vector2D& screen_offset) const
{
	//// 回転角（秒数に応じて回す）
	//float angle = move_time * 1.5f; // 回転速度は調整OK（ラジアン）
	//float half_size = box_size.x;

	//float cosA = cos(angle);
	//float sinA = sin(angle);

	//// 中心からの相対座標を回転
	//Vector2D p1 = Vector2D(-half_size, -half_size);
	//Vector2D p2 = Vector2D(half_size, -half_size);
	//Vector2D p3 = Vector2D(half_size, half_size);
	//Vector2D p4 = Vector2D(-half_size, half_size);

	//auto rotate = [&](Vector2D p) {
	//	return Vector2D(
	//		location.x + p.x * cosA - p.y * sinA,
	//		location.y + p.x * sinA + p.y * cosA
	//	);
	//	};

	//// 各点を回転後の位置に
	//p1 = rotate(p1);
	//p2 = rotate(p2);
	//p3 = rotate(p3);
	//p4 = rotate(p4);

	//// 回転四角形を2つの三角形で描画
	//int color = GetColor(255, 0, 255);
	//DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color, TRUE);
	//DrawTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, color, TRUE);

	// 体力の表示（中心付近に）
	DrawFormatString(location.x - 15, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
}

// 終了時処理
void Boss::Finalize()
{
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">１フレーム当たりの時間</param>
void Boss::Movement(float delta_second)
{
//	move_time += delta_second;
//
//	float float_offset = 8.0f * sin(move_time * 2.5f); // 浮遊感
//
//	// 攻撃パターンごとの動き分岐
//	if (attack_pattrn == 1 ||
//		attack_pattrn == 2)
//	{
//#if 0
//		float a = 250.0f; // 横幅
//		float b = 50.0f;  // 縦幅
//		float t = move_time * 1.0f; // ゆっくりめに変更！
//
//		location.x = base_position.x + a * sin(t);
//		location.y = base_position.y + b * sin(t) * cos(t);
//#else
//		float a = 250.0f; // 横幅
//		float b = 50.0f;  // 縦幅
//		float t = move_time * 1.0f;
//
//		float float_offset = 8.0f * sin(move_time * 2.5f); // 浮遊感のオフセット
//
//		location.x = base_position.x + a * sin(t);
//		location.y = base_position.y + b * sin(t) * cos(t) + float_offset; // Yに浮遊追加
//#endif
//	}
//	else if (attack_pattrn == 3)
//	{
//		//// 攻撃パターン3のときは停止
//		//location = base_position;
//		//velocity = 0;
//		//location += velocity * delta_second;
//
//		// 浮遊だけの動きにする（velocityは使わない）
//		location.y = base_position.y + float_offset;
//
//		// 横方向のゆるいスイングを追加したい場合
//		location.x = base_position.x + 10.0f * sin(move_time);
//
//	}
//	else
//	{
//		if (generate == false)
//		{
//			velocity.y = -500;
//			if (location.y < generate_base_position.y)
//			{
//				generate = true;
//			}
//		}
//		else
//		{
//			//location.x = generate_base_position.x;
//			box_size = 60;
//			velocity.y = 80;
//
//			// 横方向のゆるいスイング
//			float swing_offset = 2.0f * sin(move_time * 5.0f);
//			location.x = generate_base_position.x + swing_offset;
//
//			if (location.y > base_position.y)
//			{
//				generate_time += delta_second;
//				move_time = 0.0f;
//				velocity.y = 0;
//				collision.is_blocking = true;
//
//				// 横方向のゆるいスイング
//				float swing_offset = sin(move_time * 2.0f);
//				location.x = generate_base_position.x + swing_offset;
//				//// 浮遊だけの動きにする（velocityは使わない）
//				//location.y = base_position.y + float_offset;
//			}
//		}
//		location += velocity * delta_second;
//	}


}

void Boss::Shot(float delta_second)
{
	shot_timer += delta_second;

	// 五秒経過したら攻撃パターンを変更して弾を発射
	if (shot_timer >= 5.0f && generate_time >= 2.0f)
	{
		attack_pattrn = 1 + rand() % 3;
		is_shot = true;
		shot_timer = 0;
		generate_time = 10.0f;
	}
}

int Boss::GetAttackPattrn() const
{
	return attack_pattrn;
}

int Boss::GetIsAlive() const
{
	return is_alive;
}
