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
	box_size = 60;
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
void Boss2::Draw(const Vector2D& screen_offset) const
{
	// 雑魚１を描画する
	DrawBox(location.x - box_size.x, location.y - box_size.y,
		location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 0), TRUE);

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

	// 浮遊感：サイン波による微細な変化
	float float_amplitude_y = 10.0f;
	float float_speed_y = 2.0f;

	float float_amplitude_x = 5.0f;
	float float_speed_x = 1.5f;

	Vector2D float_offset;
	float_offset.y = sinf(move_time * float_speed_y) * float_amplitude_y;
	float_offset.x = cosf(move_time * float_speed_x) * float_amplitude_x;

	if (location.y < -200)
	{
		location.x = base_position.x;
		location.y = base_position.y - 200;

		velocity.y = 100;
	}
	else
	{

	}
		// 本来の移動（例：左右にゆっくり移動）
        // ここで本体の動きを作る
		velocity.y = -1000.0f; // 移動スピード（px/sec）

		generate_base_position += velocity * delta_second; // base_position が進行方向に移動

		// 最終的な座標：本体の位置＋浮遊
		location = generate_base_position + float_offset;

}

void Boss2::Shot(float delta_second)
{
	shot_timer += delta_second;

	// 五秒経過したら攻撃パターンを変更して弾を発射
	if (shot_timer >= 5.0f && generate_time >= 2.0f)
	{
		attack_pattrn = 1 + rand() % MAX_ATTACK_PATTRN;
		is_shot = true;
		shot_timer = 0;
		generate_time = 10.0f;
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
