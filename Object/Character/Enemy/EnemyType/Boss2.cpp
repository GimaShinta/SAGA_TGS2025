#include "Boss2.h"
#include "../../../../Utility/ProjectConfig.h"

Boss2::Boss2()
{
}

Boss2::~Boss2()
{
}

// 初期化処理
void Boss2::Initialize()
{
	enemy_type = ENE_BOSS2;
	z_layer = 1;
	box_size = 30;
	hp = 100;

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::eBoss2;
	// 当たる相手のオブジェクトタイプ
	collision.hit_object_type.push_back(eObjectType::eShot);
	collision.hit_object_type.push_back(eObjectType::eBeam);

	// 動くかどうか（trueなら動く、falseなら止まる）
	is_mobility = true;

	// 戦闘中の中心座標
	base_position = Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) - 200);

	// 登場時の中心座標
	generate_base_position = Vector2D(D_WIN_MAX_X / 2 + 150, D_WIN_MAX_Y + 200);

	// 初期座標は登場時のものを使用
	location = generate_base_position;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
void Boss2::Update(float delta_second)
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

	//// 攻撃パターン変更時に時間リセット
	//if (attack_pattrn != prev_attack_pattrn)
	//{
	//	if (prev_attack_pattrn == 3)
	//	{
	//		move_time = 0.0f; // 軌道の先頭に戻す！
	//	}
	//	prev_attack_pattrn = attack_pattrn;
	//}

	// 親クラスの更新処理を呼び出す
	__super::Update(delta_second);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void Boss2::Draw(const Vector2D& screen_offset) const
{
	// 雑魚１を描画する
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);

	// 体力の表示
	DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
}

// 終了時処理
void Boss2::Finalize()
{
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">１フレーム当たりの時間</param>
void Boss2::Movement(float delta_second)
{
	move_time += delta_second;

	// 浮遊感
	const float float_amplitude_y = 10.0f;
	const float float_speed_y = 2.0f;
	const float float_amplitude_x = 5.0f;
	const float float_speed_x = 1.5f;
	Vector2D float_offset;
	float_offset.y = sinf(move_time * float_speed_y) * float_amplitude_y;
	float_offset.x = cosf(move_time * float_speed_x) * float_amplitude_x;

	// 登場前の通り過ぎる演出
	if (!generate)
	{
		// 高速移動
		velocity.y = -900.0f;
		// 横の位置を調整
		base_position.x = generate_base_position.x;

		// 画面外いったら登場したことにする
		if (location.y < -400)
		{
			generate = true;
		}
	}
	// 登場後
	else
	{
		// ｙの戦闘位置までの距離
		float distance_y = base_position.y - location.y;
		float base_x = D_WIN_MAX_X / 2;
		base_position.x = generate_base_position.x - 150.0f;

		// 登場後の降りてくる演出
		if (fabs(distance_y) > 1.0f)
		{
			const float max_speed = 200.0f;
			velocity.y = distance_y;

			// スピードの制限
			if (velocity.y > max_speed) velocity.y = max_speed;
			if (velocity.y < -max_speed) velocity.y = -max_speed;

			// サイズの変更
			box_size = 60;
		}
		// 登場後の動き（戦闘）
		else
		{
			// 弾を打つ準備がok
			generate2 = true;

			// 反復移動の設定
			const float swing_range = 150.0f;
			const float move_speed = 150.0f;
			const float wait_time_max = 0.6f;

			// 左右どこから移動するか初期動作の設定
			if (!has_initialized_swing_x)
			{
				// 左右移動の中心線を決定
				swing_center_x = base_x;
				// 初期移動の完了（もうこの条件は通らない）
				has_initialized_swing_x = true;
				swing_direction = 1; // 右から開始
				swing_wait_timer = 0.0f;
			}

			// 留まる位置を設定
			float left_x = (swing_center_x - swing_range);
			float right_x = (swing_center_x + swing_range);

			// 待機中かどうか
			if (swing_wait_timer > 0.0f)
			{
				swing_wait_timer -= delta_second;
				velocity.x = 0.0f;
			}
			else
			{
				// 端に達しているか判定（右に動く）
				if (swing_direction == 1 && base_position.x >= right_x)
				{
					swing_direction = -1;
					swing_wait_timer = wait_time_max;
					velocity.x = 0.0f;
				}
				// （左に動く）
				else if (swing_direction == -1 && base_position.x <= left_x)
				{
					swing_direction = 1;
					swing_wait_timer = wait_time_max;
					velocity.x = 0.0f;
				}
				else
				{
					// 通常移動
					velocity.x = move_speed * swing_direction;
				}
			}

			velocity.y = 0.0f;
			location.y = base_position.y;
		}
	}

	// y座標の更新
	generate_base_position += velocity * delta_second;
	// x座標の更新
	base_position.x += velocity.x * delta_second;

	// 位置更新
	location.y = generate_base_position.y + float_offset.y;
	location.x = base_position.x + float_offset.x;
}

// 攻撃
void Boss2::Shot(float delta_second)
{
	// 次の攻撃パターン変更までの時間
	const int shot_interval = 0.1f;

	// 時間経過したら攻撃パターンを変更して弾を発射
	if (generate2 == true && is_shot == false)
	{
		shot_timer += delta_second;
		if (shot_timer >= shot_interval)
		{
#if 1

	#if 0
			// 特定の攻撃のみ繰り返す
			attack_pattrn = 5;
	#else
			// HPが減ったら攻撃パターンを変更（オーバーフロー防止に合わせてリセット）
			if (hp <= 50 && attack_pattrn_num != std::vector<int>{7, 8})
			{
				attack_pattrn_num = { 7, 8 };
				attack_count = 0; // 安全にリセット
			}

			// 決められた攻撃パターンのみ繰り返す
			if (!attack_pattrn_num.empty())
			{
				// 念のため配列サイズチェックを挟む（念押し）
				if (attack_count >= attack_pattrn_num.size())
				{
					attack_count = 0;
				}
				attack_pattrn = attack_pattrn_num[attack_count];
				attack_count++;
		}
			else
			{
				// 予備処理：攻撃パターンリストが空の時は安全なデフォルトに
				attack_pattrn = 0;
			}
#endif

#else
			// 完全ランダム
			attack_pattrn = 4 + rand() % MAX_ATTACK_PATTRN;
#endif
			is_shot = true;
			shot_timer = 0;
		}
	}
}

int Boss2::GetAttackPattrn() const
{
	return attack_pattrn;
}

int Boss2::GetIsAlive() const
{
	return is_alive;
}
