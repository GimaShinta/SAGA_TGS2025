#include "Boss3.h"
#include "../../../../Utility/ProjectConfig.h"
#include "../../Shot/EnemyBeam1.h"

Boss3::Boss3()
{
}

Boss3::~Boss3()
{
}

// 初期化処理
void Boss3::Initialize()
{
	enemy_type = ENE_BOSS3;
	z_layer = 1;
	box_size = 30;
	hp = 10000;

	// 攻撃パターンの設定
	attack_pattrn_num = { 11 };

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::eBoss3;
	// 当たる相手のオブジェクトタイプ
	collision.hit_object_type.push_back(eObjectType::eShot);
	collision.hit_object_type.push_back(eObjectType::eBeam);

	// 動くかどうか（trueなら動く、falseなら止まる）
	is_mobility = true;

	// 戦闘中の中心座標
	base_position = Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) - 225);

	// 登場時の中心座標
	generate_base_position = Vector2D(D_WIN_MAX_X / 2 + 150, 0 - 400);

	// 初期座標は登場時のものを使用
	location = generate_base_position;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	boss3_image[0] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_a2.png")[0];
	boss3_image[1] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_a3.png")[0];
	boss3_image[2] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_b1.png")[0];
	boss3_image[3] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_b2.png")[0];
	boss3_image[4] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_c1.png")[0];
	boss3_image[5] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_c2.png")[0];
	boss3_image[6] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_d1.png")[0];
	boss3_image[7] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_d2.png")[0];
	boss3_image[8] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_e1.png")[0];
	boss3_image[9] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_e2.png")[0];
	boss3_image[10] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_f.png")[0];

	//boss3_anim1 = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/anime01.png", 8, 8, 1, 32, 24);
	//boss3_image[10] = boss3_anim1[0];

	boss3_jet = rm->GetImages("Resource/Image/Effect/exhaust_03_spritesheet.png", 24, 8, 3, 128, 200);
	jet = boss3_jet[0];

	// 最初は本体の位置に固定
	for (int i = 0; i < 11; ++i)
	{
		part_positions[i] = location;
	}
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
void Boss3::Update(float delta_second)
{
	// アニメーション
	GameObjectBase::AnimationControl(image);
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

	// 部品を遅れて追従させる処理
	const float follow_speed = 5.0f; // 追従速度（大きいと早くついてくる）

	// 部品の相対オフセット（左右に2個ずつ）
	Vector2D offsets[11] = {
		Vector2D(110, 0), // 中央
		Vector2D(0,  -100.0f), // 顔
		Vector2D(-60,  0), // 右手前
		Vector2D(-110, 0),  // 右奥

		Vector2D(60, 30),  // 砲
		Vector2D(-60, 30),  // 砲
		Vector2D(-60, 30),  // 砲
		Vector2D(-60, 30),  // 砲
		Vector2D(-60, 30),  // 砲
		Vector2D(-60, 30)  // 砲
	};

	// 部品の相対オフセット（左右に2個ずつ）
	Vector2D offsets_2[11] = {
		Vector2D(0, -100), // 左奥
		Vector2D(0, 200), // 左手前
		Vector2D(-60,  0), // 右手前
		Vector2D(-120, 250),  // 右奥
		Vector2D(120, 250),  // 砲
		Vector2D(-300, 100),  // 砲
		Vector2D(300, 100),  // 砲
		Vector2D(-350, -70),  // 砲
		Vector2D(350, -70),  // 砲
		Vector2D(-60, 30)  // 砲
	};

	// 追従の速さ
	float individual_follow_speeds[11] = {
	100.0f,  // 左奥（ゆっくり）
	100.0f,  // 左手前
	100.0f,  // 右手前
	15.0f,   // 右奥（ゆっくり）

	15.0f,   // 砲（ゆっくり）
	10.0f,   // 砲（ゆっくり）
	10.0f ,  // 砲（ゆっくり）
	10.0f  , // 砲（ゆっくり）
	10.0f   ,// 砲（ゆっくり）
	100.0f   ,// 砲（ゆっくり）
	};

	// 追従の速さ
	float individual_follow_speeds_2[11] = {
	100.0f,  // 左奥（ゆっくり）
	100.0f,  // 左手前
	100.0f,  // 右手前
	100.0f,   // 右奥（ゆっくり）

	100.0f,   // 砲（ゆっくり）
	100.0f,   // 砲（ゆっくり）
	100.0f ,  // 砲（ゆっくり）
	100.0f  , // 砲（ゆっくり）
	100.0f   ,// 砲（ゆっくり）
	100.0f   ,// 砲（ゆっくり）
	};

	// 追従処理
	for (int i = 0; i < 11; ++i)
	{
		Vector2D target;
		if (generate == false)
		{
			target = location + offsets[i];
			part_positions[i] += (target - part_positions[i]) * individual_follow_speeds_2[i] * delta_second;
			angle = 3.14 / 1.0f;

		}
		else
		{
			target = location + offsets_2[i];
			part_positions[i] += (target - part_positions[i]) * individual_follow_speeds[i] * delta_second;
			angle = 0.0f;
		}
	}


	if (attack_pattrn == 12)
	{
		anim_speed = 0.01;
	}
	else
	{
		anim_speed = 0.1f;
	}


	// 親クラスの更新処理を呼び出す
	__super::Update(delta_second);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void Boss3::Draw(const Vector2D& screen_offset) const
{
	//// 雑魚１を描画する
	//DrawBox(location.x - box_size.x, location.y - box_size.y,
	//	location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);

	// 体力の表示
	DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);

	// ボスの描画
	DrawBoss3(Vector2D(location.x, location.y));
}

// 終了時処理
void Boss3::Finalize()
{
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">１フレーム当たりの時間</param>
void Boss3::Movement(float delta_second)
{
	move_time += delta_second;

	const float float_amplitude_y = 30.0f;
	const float float_speed_y = 1.5f;
	const float float_amplitude_x = 20.0f;
	const float float_speed_x = 1.5f;
	Vector2D float_offset;
	float_offset.y = sinf(move_time * float_speed_y) * float_amplitude_y;
	float_offset.x = cosf(move_time * float_speed_x) * float_amplitude_x;

	if (!generate)
	{
		generate = true;
		velocity.y = -900.0f;
		base_position.x = generate_base_position.x;

		if (location.y < -400)
		{
			generate = true;
		}
	}
	else
	{
		float distance_y = base_position.y - location.y;
		float base_x = D_WIN_MAX_X / 2;
		base_position.x = generate_base_position.x - 150.0f;

		if (fabs(distance_y) > 1.0f)
		{
			const float max_speed = 150.0f;
			velocity.y = distance_y;
			if (velocity.y > max_speed) velocity.y = max_speed;
			if (velocity.y < -max_speed) velocity.y = -max_speed;
			static bool check = false;


			static int chenge = 1;

			switch (chenge)
			{
			case 1:
				box_size = Vector2D(350, 150);
				chenge++;
				break;
			case 2:
				box_size = Vector2D(230, 250);
				chenge++;
				break;
			case 3:
				box_size = Vector2D(160, 350);
				chenge++;
				break;
			default:
				chenge = 1;
				break;
			}

			//if (check == false)
			//{
			//	box_size = Vector2D(350, 150);
			//	check = true;
			//}
			//else
			//{
			//	box_size = Vector2D(160, 350);
			//	check = false;
			//}
			image_size =3.5f;
		}
		else
		{
			generate2 = true;

		}
	}

	generate_base_position += velocity * delta_second;
	base_position.x += velocity.x * delta_second;

	location.y = generate_base_position.y + float_offset.y;
	location.x = base_position.x + float_offset.x;
}

// 攻撃
void Boss3::Shot(float delta_second)
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

	// 攻撃
	Attack(delta_second);
}

void Boss3::DrawBoss3(const Vector2D position) const
{
	DrawRotaGraph(part_positions[7].x, part_positions[7].y, image_size, angle, boss3_image[8], TRUE);
	DrawRotaGraph(part_positions[8].x, part_positions[8].y, image_size, angle, boss3_image[9], TRUE);
	// 本体
	DrawRotaGraph(position.x, position.y, image_size, angle, boss3_image[0], TRUE);
	// 部品
	DrawRotaGraph(part_positions[0].x, part_positions[0].y, image_size, angle, boss3_image[1], TRUE); // 左手前
	DrawRotaGraph(part_positions[1].x, part_positions[1].y, image_size, angle, boss3_image[2], TRUE); // 左奥
	//DrawRotaGraph(part_positions[1].x, part_positions[1].y, image_size, angle, boss3_image[3], TRUE); // 右手前
	DrawRotaGraph(part_positions[5].x, part_positions[5].y, image_size, angle, boss3_image[6], TRUE);
	DrawRotaGraph(part_positions[3].x, part_positions[3].y, image_size, angle, boss3_image[4], TRUE); // 右手前
	DrawRotaGraph(part_positions[4].x, part_positions[4].y, image_size, angle, boss3_image[5], TRUE); // 右奥
	DrawRotaGraph(part_positions[6].x, part_positions[6].y, image_size, angle, boss3_image[7], TRUE);
	//DrawRotaGraph(part_positions[1].x, part_positions[1].y, image_size, angle, boss3_image[10], TRUE);

	//if (generate == false)
	//{
	//	DrawRotaGraph(part_positions[4].x, part_positions[4].y - 50.0f, image_size, angle, boss3_image[7], TRUE);
	//	DrawRotaGraph(part_positions[5].x, part_positions[5].y - 50.0f, image_size, angle, boss3_image[7], TRUE);
	//}
	//else
	//{
	//	DrawRotaGraph(part_positions[4].x, part_positions[4].y + 110.0f, image_size, angle, boss3_image[7], TRUE);
	//	DrawRotaGraph(part_positions[5].x, part_positions[5].y + 110.0f, image_size, angle, boss3_image[7], TRUE);
	//}
	 
	//DrawBox(location.x - box_size.x, location.y - box_size.y,
	//	location.x + box_size.x, location.y + box_size.y,
	//	GetColor(255, 100, 0), TRUE);

	//DrawRotaGraph(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2, 2.0f, angle, jet, TRUE);

}

int Boss3::GetAttackPattrn() const
{
	return attack_pattrn;
}

int Boss3::GetIsAlive() const
{
	return is_alive;
}

// ボス２の攻撃
void Boss3::Attack(float delta_second)
{
	// オブジェクト管理クラスのインスタンスを取得
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	if (is_shot == true)
	{
		Vector2D e_location = location;

		switch (attack_pattrn)
		{
		case 1:
			for (int i = 0; i < 3; i++)
			{
				e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 30) + (30 * i), e_location.y + D_OBJECT_SIZE));
				e_shot4->SetVelocity(Vector2D(0, 120));
			}
			is_shot = false;
			break;
		case 2:
			// 縦２方向
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 60) + (30 * j), e_location.y));
					if (i == 0)
					{
						e_shot4->SetVelocity(Vector2D(0, 120));
					}
					else if (i == 1)
					{
						e_shot4->SetVelocity(Vector2D(0, -120));
					}
				}
			}
			// 横２方向
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(e_location.x, (e_location.y - 60) + (30 * j)));
					if (i == 0)
					{
						e_shot4->SetVelocity(Vector2D(-120, 0));
					}
					else if (i == 1)
					{
						e_shot4->SetVelocity(Vector2D(120, 0));
					}
				}
			}
			is_shot = false;
			break;
		case 3:
			// 縦に３よこに５つ生成（片方）
			for (int j = 1; j < 4; j++)
			{
				for (int i = 0; i < 10; i++)
				{
					e_shot3 = objm->CreateObject<EnemyShot3>(location);
					e_shot3->SetPlayer(player);
					e_shot3->SetPlayerLocation(player->GetLocation());
					// +１しなかったら０秒で発射される
					e_shot3->SetStepShot(j);
					if (i < 5)
					{
						e_shot3->SetVelocity(Vector2D(240, 0));
						e_shot3->SetPurposeLocation(Vector2D((e_location.x + 150) + (30 * i), (e_location.y + 40) - (50 * j)));
					}
					else
					{
						e_shot3->SetVelocity(Vector2D(-240, 0));
						e_shot3->SetPurposeLocation(Vector2D(e_location.x - (30 * i), (e_location.y + 40) - (50 * j)));
					}
				}
			}
			is_shot = false;
			break;
		case 4:
			/// <summary>
			/// 攻撃パターン４（花火）
			/// </summary>
			/// <param name="bullet_num">弾の数</param>
			/// <param name="speed">弾の速度</param>
			/// <param name="spiral_interval">発射間隔</param>
			/// <param name="spiral_duration_limit">攻撃する時間</param>
			/// <param name="generate_location">生成する位置</param>
			/// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
			Pattrn4(20, 300.0f, 1.0f, 5.0f, location, delta_second);
			break;
		case 5:
#if 0
			static float spiral_timer = 0.0f;            // 弾発射のインターバル管理
			static float spiral_angle = 0.0f;            // 回転角度
			static float spiral_total_time = 0.0f;       // パターン5に入ってからの累計時間
			const float spiral_interval = 0.1f;
			const float spiral_duration_limit = 3.0f;    // 最大発射時間

			// 3秒経過したら何もしない
			if (spiral_total_time >= spiral_duration_limit) return;

			spiral_timer += delta_second;
			spiral_total_time += delta_second;

			if (spiral_timer >= spiral_interval)
			{
				spiral_timer = 0.0f;

				float rad = spiral_angle * DX_PI / 180.0f;
				float speed = 160.0f;
				Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

				GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
				EnemyShot4* shot = objm->CreateObject<EnemyShot4>(Boss3->GetLocation());
				shot->SetVelocity(velocity);

				spiral_angle += 15.0f;
				if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
			}
#else

			/// <summary>
			/// 攻撃パターン５（渦巻き）
			/// </summary>
			/// <param name="spiral_interval">発射間隔</param>
			/// <param name="spiral_duration_limit">攻撃する時間</param>
			/// <param name="spiral_speed">弾の速度</param>
			/// <param name="generate_location">生成する位置</param>
			/// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
			Pattrn5(0.1f, 5.0f, 300.0f, location, delta_second);
#endif
			break;
		case 6:
			/// <summary>
			/// 攻撃パターン６（バラバラ扇型）
			/// </summary>
			/// <param name="fan_angle_range">扇の角度（60なら±30°）</param>
			/// <param name="bullet_speed">弾の速度</param>
			/// <param name="fan_interval">発射間隔</param>
			/// <param name="fan_duration_limit">攻撃する時間</param>
			/// <param name="generate_location">生成する位置</param>
			/// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
			Pattrn6(100.0f, 300.0f, 0.15f, 10.0f, location, delta_second);

			break;
		case 7:
			/// <summary>
			/// 攻撃パターン７（段階扇形）
			/// </summary>
			/// <param name="fan_angle_range">扇の角度範囲（60なら±30°）</param>
			/// <param name="bullet_speed">弾の速度</param>
			/// <param name="fan_interval">発射の間隔</param>
			/// <param name="fan_duration_limit">攻撃時間</param>
			/// <param name="generate_location">生成する位置</param>
			/// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
			Pattrn7(150.0f, 300.0f, 0.5f, 10.0f, Vector2D(location.x, location.y + 100.0f), delta_second);

			break;
		case 8:
			/// <summary>
			/// 攻撃パターン８（ひもQ）
			/// </summary>
			/// <param name="wave_interval">発射間隔</param>
			/// <param name="wave_duration_limit">発車時間の上限</param>
			/// <param name="generate_location">生成する位置</param>
			/// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
			Pattrn8(0.1f, 1.0f, Vector2D(location.x, location.y + 150.0f), delta_second);

			break;
		case 9:
			/// <summary>
			/// 攻撃パターン９（ボス中心回転弾）
			/// </summary>
			/// <param name="shot_count">弾の数</param>
			/// <param name="radius">ボスからの距離</param>
			/// <param name="angular_speed">回転速度</param>
			/// <param name="bullet_speed">弾速</param>
			/// <param name="rotation_timer">回転の時間計測</param>
			/// <param name="generate_location">生成する位置</param>
			/// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
			Pattrn9(27, 250.0f, 90.0f, 0.0f, location, delta_second);

			/// <summary>
			/// 攻撃パターン11（一本ビーム）
			/// </summary>
			/// <param name="offsets_x">ボスからの距離(横)</param>
			Pattrn11(0.0f);

			break;
		case 10:
			/// <summary>
			/// 攻撃パターン１０（ベクトル中心回転弾）
			/// </summary>
			/// <param name="shot_count">弾の数</param>
			/// <param name="radius">ボスからの距離</param>
			/// <param name="angular_speed">回転速度</param>
			/// <param name="bullet_speed">弾速</param>
			/// <param name="rotation_timer">回転の時間計測</param>
			/// <param name="center_location">生成する場所</param>
			/// <param name="generate_location">生成する場所</param>
			/// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
			Pattrn10(8, 120.0f, 350.0f, 300.0f, 5.0f, location, delta_second);

			break;
		case 11:
			/// <summary>
			/// 攻撃パターン11（一本ビーム）
			/// </summary>
			/// <param name="offsets_x">ボスからの距離(横)</param>
			Pattrn11(0.0f);

			break;
		case 12:
			/// <summary>
			/// 攻撃パターン12（二本ビーム）
			/// </summary>
			Pattrn12();

			break;
		case 13:
			Pattrn13(delta_second);

			break;
		default:
			break;
		}
	}
}

/// <summary>
/// 攻撃パターン４
/// </summary>
/// <param name="bullet_num">弾の数</param>
/// <param name="speed">弾の速度</param>
/// <param name="spiral_interval">発射間隔</param>
/// <param name="spiral_duration_limit">攻撃する時間</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Boss3::Pattrn4(int bullet_num, float speed, float spiral_interval, float spiral_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// オブジェクト管理クラスのインスタンスを取得
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float spiral_timer = 0.0f;
	static float spiral_total_time = 0.0f;

	spiral_timer += delta_second;
	spiral_total_time += delta_second;

	if (spiral_timer >= spiral_interval)
	{
		spiral_timer = 0.0f;

		for (int i = 0; i < bullet_num; i++)
		{
			float angle = (360.0f / bullet_num) * i;
			float rad = angle * DX_PI / 180.0f;

			Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

			EnemyShot4* e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
			e_shot4->SetVelocity(velocity);
			e_shot4->SetAttackPattrn(1);
		}
	}

	// 時間制限を超えたら終了（発射しない）
	if (spiral_total_time >= spiral_duration_limit)
	{
		spiral_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// 攻撃パターン５
/// </summary>
/// <param name="spiral_interval">発射間隔</param>
/// <param name="spiral_duration_limit">攻撃する時間</param>
/// <param name="spiral_speed">弾の速度</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Boss3::Pattrn5(float spiral_interval, float spiral_duration_limit, float spiral_speed, const Vector2D& generate_location, float delta_second)
{
	// オブジェクト管理クラスのインスタンスを取得
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float spiral_timer = 0.0f;
	static float spiral_angle = 0.0f;
	static float spiral_total_time = 0.0f;

	spiral_timer += delta_second;
	spiral_total_time += delta_second;

	if (spiral_timer >= spiral_interval)
	{
		spiral_timer = 0.0f;

		for (int dir = 0; dir < 2; dir++)
		{
			float base_angle = (dir == 0) ? 90.0f : 270.0f;
			float current_angle = base_angle + spiral_angle;

			float rad = current_angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

			EnemyShot4* shot = objm->CreateObject<EnemyShot4>(generate_location);
			shot->SetVelocity(velocity);
			shot->SetAttackPattrn(2);
		}

		spiral_angle += 40.0f;
		if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
	}

	// 時間制限を超えたら終了（発射しない）
	if (spiral_total_time >= spiral_duration_limit)
	{
		spiral_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// 攻撃パターン６
/// </summary>
/// <param name="fan_angle_range">扇の角度（60なら±30°）</param>
/// <param name="bullet_speed">弾の速度</param>
/// <param name="fan_interval">発射間隔</param>
/// <param name="fan_duration_limit">攻撃する時間</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Boss3::Pattrn6(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// オブジェクト管理クラスのインスタンスを取得
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	// 攻撃持続用 static 変数
	static float fan_timer = 0.0f;
	static float fan_total_time = 0.0f;

	// 攻撃パターンが変更されたらリセットしたい → Boss3側で状態持たせるのもあり

	fan_timer += delta_second;
	fan_total_time += delta_second;

	// 一定間隔ごとに発射
	if (fan_timer >= fan_interval)
	{
		fan_timer = 0.0f;

		// ランダムな角度（中心±60°）
		float base_angle = 90.0f; // 下方向中心
		float random_angle = base_angle - fan_angle_range / 2.0f + (rand() % (int)fan_angle_range);

		float rad = random_angle * DX_PI / 180.0f;
		Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

		e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
		e_shot4->SetVelocity(velocity);
		e_shot4->SetAttackPattrn(1);

	}

	// 時間制限を超えたら終了（発射しない）
	if (fan_total_time >= fan_duration_limit)
	{
		fan_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// 攻撃パターン７
/// </summary>
/// <param name="fan_angle_range">扇の角度範囲（60なら±30°）</param>
/// <param name="bullet_speed">弾の速度</param>
/// <param name="fan_interval">発射の間隔</param>
/// <param name="fan_duration_limit">攻撃時間</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Boss3::Pattrn7(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// オブジェクト管理クラスのインスタンスを取得
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float fan_timer = 0.0f;
	static float fan_total_time = 0.0f;

	fan_timer += delta_second;
	fan_total_time += delta_second;

	if (fan_timer >= fan_interval)
	{
		fan_timer = 0.0f;

		int bullet_count = 6;
		float base_angle = 90.0f; // 中心下方向

		for (int i = 0; i < bullet_count; ++i)
		{
			float angle = base_angle - fan_angle_range / 2.0f +
				(fan_angle_range / (bullet_count - 1)) * i;

			float rad = angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

			e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
			e_shot4->SetVelocity(velocity);
			e_shot4->SetAttackPattrn(2);
		}
	}

	// 一定時間経過したら終了
	if (fan_total_time >= fan_duration_limit)
	{
		fan_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// 攻撃パターン８
/// </summary>
/// <param name="wave_interval">発射間隔</param>
/// <param name="wave_duration_limit">発車時間の上限</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Boss3::Pattrn8(float wave_interval, float wave_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// オブジェクト管理クラスのインスタンスを取得
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

#if 1
	static float wave_timer = 0.0f;
	static float wave_total_time = 0.0f;

	wave_timer += delta_second;
	wave_total_time += delta_second;

	if (wave_timer >= wave_interval)
	{
		wave_timer = 0.0f;
		Vector2D e_lo = generate_location;

		// 右側
		e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x + 50, e_lo.y));
		e_shot5->SetWaveReflected(false);
		e_shot5->SetVelocity(Vector2D(0, 200));
		e_shot5->SetWaveParameters(600.0f, 0.7f);

		// 左側
		e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x - 50, e_lo.y));
		e_shot5->SetWaveReflected(true);
		e_shot5->SetVelocity(Vector2D(0, 200));
		e_shot5->SetWaveParameters(600.0f, 0.7f);
	}

	// 一定時間経過したら終了
	if (wave_total_time >= wave_duration_limit)
	{
		wave_total_time = 0.0f;
		is_shot = false;
	}
#else

	int num_shots = 20;
	float spread_speed = 150.0f;
	Vector2D origin = e_location; // 弾の初期発射位置（ボスの位置）
	Vector2D boss_center = Boss3->GetLocation(); // 吸い込み中心！

	for (int i = 0; i < num_shots; ++i)
	{
		float angle = 360.0f / num_shots * i;
		float rad = angle * DX_PI / 180.0f;

		e_shot5 = objm->CreateObject<EnemyShot5>(origin);
		e_shot5->SetVelocity(Vector2D(cosf(rad), sinf(rad)) * spread_speed);
		e_shot5->SetSuckCenter(boss_center); // 吸い込み先をセット！
	}
#endif

}

/// <summary>
/// 攻撃パターン９
/// </summary>
/// <param name="shot_count">弾の数</param>
/// <param name="radius">ボスからの距離</param>
/// <param name="angular_speed">回転速度</param>
/// <param name="bullet_speed">弾速</param>
/// <param name="rotation_timer">回転の時間計測</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Boss3::Pattrn9(int shot_count, float radius, float angular_speed, float bullet_speed, const Vector2D& generate_location, float delta_second)
{
	// オブジェクト管理クラスのインスタンスを取得
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static std::vector<float> angles;    // 各弾の角度管理
	static std::vector<EnemyShot4*> rotating_shots;  // 弾のポインタを保持

	static float rotation_timer = 0.0f;

	rotation_timer += delta_second;

	// 初期化処理
	if (angles.empty())
	{
		rotating_shots.clear();
		// 弾を生成して初期角度を設定
		for (int i = 0; i < shot_count; ++i)
		{
			float angle = 360.0f / shot_count * i;  // 弾の角度設定
			angles.push_back(angle);

			// ボスの周囲に弾を配置
			e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);

			if (e_shot4)
			{
				rotating_shots.push_back(e_shot4);  // 正しく生成されたら弾を保存
				e_shot4->SetVelocity(Vector2D(0, 0)); // 回転だけなので弾速は0
				e_shot4->SetAttackPattrn(1);
			}
		}
	}

	// 各弾の位置を更新して回転させる
	for (int i = 0; i < shot_count; ++i)
	{
		angles[i] += angular_speed * delta_second; // 角度更新
		if (angles[i] >= 360.0f) angles[i] -= 360.0f; // 角度を0?360度に調整

		float rad = angles[i] * DX_PI / 180.0f;  // ラジアンに変換

		// ボスを中心に半径 `radius` の円軌道を描く
		Vector2D new_pos = generate_location + Vector2D(cos(rad) * radius, sin(rad) * radius);

		// 弾の位置を更新
		if (rotating_shots[i])  // 弾が正しく存在するか確認
		{
			rotating_shots[i]->SetLocation(new_pos);

			// 弾の速度はゼロ（回転のみ）
			rotating_shots[i]->SetVelocity(Vector2D(0, 0));
		}
	}

	// 攻撃の終了判定（一定時間経過後）
	static float duration_timer = 0.0f;
	duration_timer += delta_second;
	if (duration_timer >= 5.0f)  // 5秒後に終了
	{
		for (int i = 0; i < shot_count; ++i)
		{
			if (rotating_shots[i])
			{
				rotating_shots[i]->SetDestroy();  // 弾を削除
			}

		}
		angles.clear();  // 角度情報もクリア
		rotating_shots.clear();  // 弾のリストもクリア
		duration_timer = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// 攻撃パターン１０
/// </summary>
/// <param name="shot_count">弾の数</param>
/// <param name="radius">ボスからの距離</param>
/// <param name="angular_speed">回転速度</param>
/// <param name="bullet_speed">弾速</param>
/// <param name="rotation_timer">回転の時間計測</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Boss3::Pattrn10(int shot_count, float radius, float angular_speed, float center_speed, float duration_limit, const Vector2D& generate_location, float delta_second)
{
	// オブジェクト管理クラスのインスタンスを取得
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float duration_timer = 0.0f;
	static Vector2D center_pos;  // 見えない中心座標
	static Vector2D center_velocity;
	static std::vector<float> angles;
	static std::vector<EnemyShot4*> rotating_shots;
	static bool initialized = false;

	// 時間経過
	duration_timer += delta_second;

	if (duration_timer < duration_limit)
	{
		if (!initialized) // 初期化処理
		{
			// ボスの位置を初期中心として、真下に移動させる
			center_pos = generate_location;
			center_velocity = Vector2D(0.0f, center_speed); // 真下方向

			angles.clear();
			rotating_shots.clear();
			for (int i = 0; i < shot_count; ++i)
			{
				float angle = 360.0f / shot_count * i;
				angles.push_back(angle);

				EnemyShot4* shot = objm->CreateObject<EnemyShot4>(center_pos);
				if (shot)
				{
					shot->SetVelocity(Vector2D(0, 0));
					rotating_shots.push_back(shot);
					e_shot4->SetAttackPattrn(1);
				}
			}

			initialized = true;
		}

		// 見えない中心の移動
		center_pos += center_velocity * delta_second;

		// 回転弾の位置更新
		for (int i = 0; i < shot_count; ++i)
		{
			angles[i] += angular_speed * delta_second;
			if (angles[i] >= 360.0f) angles[i] -= 360.0f;

			float rad = angles[i] * DX_PI / 180.0f;
			Vector2D offset(cos(rad) * radius, sin(rad) * radius);

			if (rotating_shots[i])
			{
				rotating_shots[i]->SetLocation(center_pos + offset);
			}
		}
	}
	else
	{
		// 攻撃終了後の処理
		duration_timer = 0.0f;
		is_shot = false;
		initialized = false;  // 初期化フラグをリセット
	}
}

/// <summary>
/// 攻撃パターン11（一本ビーム）
/// </summary>
/// <param name="offsets_x">ボスからの距離</param>
void Boss3::Pattrn11(float offsets_x)
{
	static bool                         beam_on = false;          // いま発射中か？
	static EnemyBeam1* b;                     // 発射中ビーム

	if (!beam_on)
	{
		GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

		b = objm->CreateObject<EnemyBeam1>(
			Vector2D(
				location.x + offsets_x,
				(location.y + 350.0f) - box_size.y
			));
		b->SetBoss3(this);
		beam_on = true;
	}

	if (b != nullptr)
	{
		// それぞれ +100 / -100 のオフセットで追従
		b->SetLocation(
			Vector2D(
				location.x + offsets_x,
				(location.y + 350.0f) + b->GetBoxSize().y
			));
	}

	if (b != nullptr && b->is_destroy)   // 5 秒後などに true になる想定
	{
		b->SetDestroy();                 // 必要なら明示削除
		is_shot = false;
		beam_on = false;
	}
}

/// <summary>
/// 攻撃パターン12（ビーム二本）
/// </summary>
void Boss3::Pattrn12()
{
	static bool                         beam_on = false;          // いま発射中か？
	static std::vector<EnemyBeam1*>     beams;                     // 発射中ビーム
	static const float                  OFFSETS_X[2] = { +100.f, -100.f }; // +100 / -100 の並び

	if (!beam_on)
	{
		GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

		for (int i = 0; i < 2; ++i)
		{
			EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
				Vector2D(
					location.x + OFFSETS_X[i],
					(location.y + 50.0f) - box_size.y
				));
			b->SetBoss3(this);
			beams.push_back(b);
		}
		beam_on = true;
	}

	for (size_t i = 0; i < beams.size(); ++i)
	{
		if (beams[i] == nullptr) continue;   // 念のため

		// それぞれ +100 / -100 のオフセットで追従
		beams[i]->SetLocation(
			Vector2D(
				location.x + OFFSETS_X[i],
				(location.y + 50.0f) + beams[i]->GetBoxSize().y
			));
	}

	for (auto it = beams.begin(); it != beams.end(); )
	{
		EnemyBeam1* b = *it;

		if (b != nullptr && b->is_destroy)   // 5 秒後などに true になる想定
		{
			b->SetDestroy();                 // 必要なら明示削除
			it = beams.erase(it);            // vector から外す
		}
		else
		{
			++it;
		}
	}

	// 全部消えたら次の攻撃を解禁
	if (beam_on && beams.empty())
	{
		is_shot = false;
		beam_on = false;
	}
}

/// <summary>
/// 攻撃パターン13（ビームの段階攻撃）バグあり
/// </summary>
/// <param name="delta_second">1フレーム当たりの時間</param>
void Boss3::Pattrn13(float delta_second)
{
	static int   step = 0;
	static float timer = 0.0f;
	static std::vector<EnemyBeam1*> beams;

	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	timer += delta_second; // 可変FPSなら delta_second を使う

	switch (step)
	{
	case 0: // 第1段階：中央に1本
	{
		EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
			Vector2D(location.x, location.y - D_OBJECT_SIZE)
		);
		b->SetBoss3(this);
		beams.push_back(b);
		step = 1;
		timer = 0.0f;
	}
	break;

	case 1: // 2秒後に第2段階：2本（左右）
		if (timer >= 2.0f)
		{
			float offsets[] = { +100.0f, -100.0f };
			for (float ox : offsets)
			{
				EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
					Vector2D(location.x + ox, location.y - D_OBJECT_SIZE)
				);
				b->SetBoss3(this);
				beams.push_back(b);
			}
			step = 2;
			timer = 0.0f;
		}
		break;

	case 2: // 2秒後に第3段階：3本（全体）
		if (timer >= 2.0f)
		{
			float offsets[] = { +200.0f, 0.0f, -200.0f };
			for (float ox : offsets)
			{
				EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
					Vector2D(location.x + ox, location.y - D_OBJECT_SIZE)
				);
				b->SetBoss3(this);
				beams.push_back(b);
			}
			step = 3;
		}
		break;

	case 3: // ビーム位置追従と終了判定
		for (size_t i = 0; i < beams.size(); ++i)
		{
			if (beams[i] == nullptr) continue;

			float offset = 0.0f;
			switch (beams.size())
			{
			case 1: offset = 0.0f; break;
			case 3: offset = (i == 0) ? +100.0f : (i == 1) ? -100.0f : 0.0f; break;
			case 6: offset = (i == 0) ? +0.0f : (i == 1) ? +100.0f : (i == 2) ? -100.0f :
				(i == 3) ? +200.0f : (i == 4) ? 0.0f : -200.0f;
				break;
			}

			beams[i]->SetLocation(
				Vector2D(
					location.x + offset,
					(location.y - D_OBJECT_SIZE) + beams[i]->GetBoxSize().y
				)
			);
		}

		// ビームがすべて消えたら終了
		for (auto it = beams.begin(); it != beams.end(); )
		{
			EnemyBeam1* b = *it;
			if (b != nullptr && b->is_destroy)
			{
				b->SetDestroy();
				it = beams.erase(it);
			}
			else ++it;
		}

		if (beams.empty())
		{
			is_shot = false;
			step = 0;
			timer = 0.0f;
		}
		break;
	}
}
