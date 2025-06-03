#include "Player.h"
#include "../../../Utility/InputManager.h"
#include "../../../Utility/ProjectConfig.h"

Player::Player() : is_shot(false), count_space(1), life(3), on_hit(false), is_damage(false)
{
}

Player::~Player()
{
}

// 初期化処理
void Player::Initialize()
{
	z_layer = 2;
	velocity = 0;
	box_size = 10;

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::ePlayer;
	// 当たる相手のオブジェクトタイプ
	collision.hit_object_type.push_back(eObjectType::eEnemy);
	collision.hit_object_type.push_back(eObjectType::eBoss2);
	collision.hit_object_type.push_back(eObjectType::eExp);
	collision.hit_object_type.push_back(eObjectType::ePowerUp);

	// 動くかどうか（trueなら動く、falseなら止まる）
	is_mobility = true;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	image = rm->GetImages("Resource/Image/Object/Player/Player_03/player03.png")[0];
	player_image_left = rm->GetImages("Resource/Image/Object/Player/Player_03/anime_player03_L01.png", 2, 2, 1, 56, 64);
	player_image_right = rm->GetImages("Resource/Image/Object/Player/Player_03/anime_player03_R01.png", 2, 2, 1, 56, 64);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
void Player::Update(float delta_second)
{
	// 移動処理
	Movement(delta_second);
	// 弾を打つ
	Shot(delta_second);
	// ダメージを受けたら
	Damage(delta_second);

	// ライフが０になったらゲームオーバー
	if (life < 0)
	{
		is_alive = false;
	}

	if (powerd_time > 0.0f)
	{
		powerd_time -= delta_second;
	}
	else
	{
		powerd_on = false;
	}

	if (powerd >= 3)
	{
		powerd = 3;
	}


	// アニメーション更新処理
	anim_timer += delta_second;

	if (anim_timer >= anim_interval)
	{
		anim_timer = 0.0f;
		anim_index++;
		if (anim_index >= 2) anim_index = 1;
	}

	// 親クラスの更新処理を呼び出す
	__super::Update(delta_second);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void Player::Draw(const Vector2D& screen_offset) const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, brend);

	switch (anim_state)
	{
	case PlayerAnimState::Neutral:
		DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, image, TRUE);
		break;
	case PlayerAnimState::TiltLeft:
		DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, player_image_left[anim_index], TRUE);
		break;
	case PlayerAnimState::TiltRight:
		DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, player_image_right[anim_index], TRUE);
		break;
	}

	// ライフ表示などはそのまま
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawFormatString(D_WIN_MAX_X - 150, 0, GetColor(255, 255, 255), "残りライフ : %d", life);
}

// 終了時処理
void Player::Finalize()
{
}

/// <summary>
/// ヒット時処理
/// </summary>
/// <param name="hit_object">当たった相手</param>
void Player::OnHitCollision(GameObjectBase* hit_object)
{
	if (hit_object->GetCollision().object_type == eObjectType::eEnemy ||
		hit_object->GetCollision().object_type == eObjectType::eEnemyShot ||
		hit_object->GetCollision().object_type == eObjectType::eEnemyBeam)
	{
		if (on_hit == false)
		{
			life--;
			is_damage = true;
			on_hit = true;
		}
	}
	if (hit_object->GetCollision().object_type == eObjectType::ePowerUp)
	{
		if (powerd_on == false)
		{
			powerd++;
			powerd_on = true;
			powerd_time = 4.0f;
		}
	}
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="delta_second">１フレーム当たりの時間</param>
void Player::Movement(float delta_second)
{
	InputManager* input = Singleton<InputManager>::GetInstance();
	Vector2D stick_value = input->GetLeftStick();

	const float accel = 1000.0f;
	const float friction = 800.0f;
	const float dead_zone = 0.1f;
	const float max_speed = PLAYER_SPEED_MAX;

	// 入力ベクトル（初期化）
	Vector2D input_dir = { 0.0f, 0.0f };

	// キー入力に応じて方向を設定
	if (input->GetKey(KEY_INPUT_D) || input->GetButton(XINPUT_BUTTON_DPAD_RIGHT)) input_dir.x += 1.0f;
	if (input->GetKey(KEY_INPUT_A) || input->GetButton(XINPUT_BUTTON_DPAD_LEFT)) input_dir.x -= 1.0f;
	if (input->GetKey(KEY_INPUT_S) || input->GetButton(XINPUT_BUTTON_DPAD_DOWN)) input_dir.y += 1.0f;
	if (input->GetKey(KEY_INPUT_W) || input->GetButton(XINPUT_BUTTON_DPAD_UP)) input_dir.y -= 1.0f;

	// スティック入力も合成
	if (std::abs(stick_value.x) > dead_zone) input_dir.x += stick_value.x;
	if (std::abs(stick_value.y) > dead_zone) input_dir.y -= stick_value.y;

	// 入力方向があれば正規化
	if (input_dir.Length() > 1.0f) input_dir.Normalize();

	// 入力方向に基づいて速度を設定（加速処理を削除）
	velocity = input_dir * max_speed;  // 一定の最大速度で移動

	// 画面外移動制限
	const float limit_x = (D_WIN_MAX_X / 2);
	const float half_width = 350.0f; // プレイヤーの幅

	// X軸の制限
	if (location.x < limit_x - half_width + 10)
	{
		location.x = limit_x - half_width + 10;
		velocity.x = 0.0f; // 位置制限後に速度もゼロにする
	}
	if (location.x > limit_x + half_width - 10)
	{
		location.x = limit_x + half_width - 10;
		velocity.x = 0.0f; // 位置制限後に速度もゼロにする
	}

	// Y軸の制限
	if (location.y < 10)
	{
		location.y = 10;
		velocity.y = 0.0f; // 位置制限後に速度もゼロにする
	}
	if (location.y > D_WIN_MAX_Y - 10)
	{
		location.y = D_WIN_MAX_Y - 10;
		velocity.y = 0.0f; // 位置制限後に速度もゼロにする
	}

	// アニメーション状態決定
	if (velocity.x > 10.0f)
	{
		anim_state = PlayerAnimState::TiltRight;
	}
	else if (velocity.x < -10.0f)
	{
		anim_state = PlayerAnimState::TiltLeft;
	}
	else
	{
		anim_state = PlayerAnimState::Neutral;
	}

	// 位置更新
	location += velocity * delta_second;
}

// 弾を打つ
void Player::Shot(float delta_second)
{
	// オブジェクト管理クラスのインスタンを取得
	InputManager* input = Singleton<InputManager>::GetInstance();

	// 打つまでの時間を計測
	shot_timer += delta_second;
	beam_timer += delta_second;

	// 上下に打てるようにする
	if (input->GetKeyDown(KEY_INPUT_L) ||
		input->GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		if (shot_flip == false)
		{
			shot_flip = true;
		}
		else
		{
			shot_flip = false;
		}
	}

	// スペースを押したら単発で発射
	if (input->GetKeyDown(KEY_INPUT_SPACE) ||
		input->GetButtonDown(XINPUT_BUTTON_A))
	{
		// 何も打ってなかったら打てるようにする
		if (stop == false)
		{
			is_shot = true;
			shot_timer = 0.0f;
		}
	}
	// スペースを長押ししたら一定間隔で発射
	else if (input->GetKey(KEY_INPUT_SPACE) ||
		     input->GetButton(XINPUT_BUTTON_A))
	{
		// 何も打ってなかったら打てるようにする
		if (stop == false)
		{
			// 発射インターバルを超えたら発射
			if (shot_timer >= SHOT_INTERVAL)
			{
				is_shot = true;
				shot_timer = 0.0f;
			}
		}
	}
	else
	{
		// 押してない間は、即撃てるようにする
		shot_timer = SHOT_INTERVAL; 
	}

	// Bを押したらビーム発射
	if ((input->GetKeyDown(KEY_INPUT_B) || input->GetButtonDown(XINPUT_BUTTON_B)) &&
		CanUseSpecial())  // チャージ完了時のみ
	{
		if (stop == false)
		{
			beam_on = true;
			stop = true;
			beam_timer = 0.0f;
			UseSpecial();  // ゲージ消費
		}
	}


	// ５秒経ったらビームの再起
	if (beam_timer >= 5.0f)
	{
		stop = false;
	}
}

void Player::Damage(float delta_second)
{
	// ダメージを受けたときチカチカさせる
	if (is_damage == true)
	{
		// カウント加算
		damage_timer += delta_second;
		if (damage_timer >= 0.05f)
		{
			// 到達カウントを加算
			reach_count++;
			// 到達カウントが偶数であれば透明に
			if (reach_count % 2 == 0)
			{
				brend = 0;
			}
			// 奇数であれば不透明に
			else
			{
				brend = 255;
			}
			//カウントリセット
			damage_timer = 0;
		}
		// チカチカが終わったら全てリセット
		if (reach_count >= 30)
		{
			is_damage = false;
			on_hit = false;
			brend = 255;
			damage_timer = 0;
			reach_count = 0;
		}
	}
}

bool Player::GetIsShot() const
{
	return is_shot;
}

bool Player::GetIsAlive() const
{
	return is_alive;
}

bool Player::GetBeamOn() const
{
	return beam_on;
}

bool Player::GetShotFlip() const
{
	return shot_flip;
}

void Player::SetIsShot()
{
	is_shot = false;
}

void Player::SetBeamOn()
{
	beam_on = false;
}

void Player::AddCharge(float value)
{
	// ビーム中は加算しない
	if (beam_on) return;

	charge += value;
	if (charge >= charge_max)
	{
		charge = charge_max;
		charge_ready = true;
	}
}

bool Player::CanUseSpecial() const
{
	return charge_ready;
}

void Player::UseSpecial()
{
	charge = 0.0f;
	charge_ready = false;
}

float Player::GetChargeRate() const
{
	// 発動中は常にMAX表示
	if (beam_on) return 1.0f;
	return charge / charge_max;
}
