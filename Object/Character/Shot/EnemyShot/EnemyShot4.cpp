#include "EnemyShot4.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"

EnemyShot4::EnemyShot4()
{
}

EnemyShot4::~EnemyShot4()
{
}

// 初期化処理
void EnemyShot4::Initialize()
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

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	//images = rm->GetImages("Resource/Image/Effect/anime_explosion03_01.png", 72, 12, 6, 112, 112);
	//image = images[36];

	bullet_1 = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime81.png", 6, 6, 1, 24, 24);
	bullet_2 = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Orange/Standard/anime89.png", 4, 4, 1, 16, 32);
	
	//bullet[0] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime01.png", 8, 8, 1, 16, 16);
	//bullet[1] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime02.png", 6, 6, 1, 16, 16);
	//bullet[2] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime03.png", 8, 8, 1, 24, 24);
	//bullet[3] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime04.png", 7, 7, 1, 24, 24);
	//bullet[4] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime05.png", 7, 7, 1, 16, 16);
	//bullet[5] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime06.png", 6, 6, 1, 16, 16);
	//bullet[6] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime07.png", 6, 6, 1, 16, 16);
	//bullet[7] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime08.png", 7, 7, 1, 16, 24);
	//bullet[8] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime09.png", 8, 8, 1, 24, 24);
	//bullet[9] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime10.png", 2, 2, 1, 16, 16);
	//bullet[10] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime11.png", 4, 4, 1, 16, 16);
	//bullet[11] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime12.png", 4, 4, 1, 16, 16);
	//bullet[12] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime13.png", 4, 4, 1, 16, 16);
	//bullet[13] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime14.png", 4, 4, 1, 16, 16);
	//bullet[14] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime15.png", 4, 4, 1, 16, 16);
	//bullet[15] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime16.png", 4, 4, 1, 16, 16);
	//bullet[16] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime17.png", 4, 4, 1, 16, 16);
	//bullet[17] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime18.png", 4, 4, 1, 16, 16);
	//bullet[18] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime19.png", 4, 4, 1, 24, 24);
	//bullet[19] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime20.png", 4, 4, 1, 24, 24);
	//bullet[20] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime21.png", 3, 3, 1, 16, 16);
	//bullet[21] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime22.png", 8, 8, 1, 16, 16);
	//bullet[22] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime23.png", 6, 6, 1, 16, 16);
	//bullet[23] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime24.png", 6, 6, 1, 16, 16);
	//bullet[24] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime25.png", 6, 6, 1, 16, 16);
	//bullet[25] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime26.png", 4, 4, 1, 16, 16);
	//bullet[26] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime27.png", 6, 6, 1, 16, 16);
	//bullet[27] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime28.png", 7, 7, 1, 16, 16);
	//bullet[28] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime29.png", 4, 4, 1, 16, 16);
	//bullet[29] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime30.png", 6, 6, 1, 16, 16);
	//bullet[30] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime31.png", 6, 6, 1, 32, 32);
	//bullet[31] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime32.png", 6, 6, 1, 16, 32);
	//bullet[32] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime33.png", 6, 6, 1, 16, 32);
	//bullet[33] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime34.png", 4, 4, 1, 16, 32);
	//bullet[34] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime35.png", 4, 4, 1, 8, 24);
	//bullet[35] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime36.png", 4, 4, 1, 8, 56);
	//bullet[36] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime37.png", 4, 4, 1, 24, 24);
	//bullet[37] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime38.png", 6, 6, 1, 24, 24);
	//bullet[38] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime39.png", 4, 4, 1, 32, 16);
	//bullet[39] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime40.png", 3, 3, 1, 32, 16);
	//bullet[40] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime41.png", 4, 4, 1, 32, 32);
	//bullet[41] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime42.png", 5, 5, 1, 16, 16);
	//bullet[42] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime43.png", 6, 6, 1, 16, 16);
	//bullet[43] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime44.png", 4, 4, 1, 16, 16);
	//bullet[44] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime45.png", 4, 4, 1, 16, 16);
	//bullet[45] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime46.png", 4, 4, 1, 16, 16);
	//bullet[46] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime47.png", 4, 4, 1, 16, 64);
	//bullet[47] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime48.png", 5, 5, 1, 16, 24);
	//bullet[48] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime49.png", 5, 5, 1, 16, 16);
	//bullet[49] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime50.png", 4, 4, 1, 16, 24);
	//bullet[50] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime51.png", 4, 4, 1, 16, 32);
	//bullet[51] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime52.png", 4, 4, 1, 16, 32);
	//bullet[52] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime53.png", 4, 4, 1, 16, 32);
	//bullet[53] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime54.png", 4, 4, 1, 16, 32);
	//bullet[54] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime55.png", 4, 4, 1, 16, 32);
	//bullet[55] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime56.png", 4, 4, 1, 16, 32);
	//bullet[56] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime57.png", 4, 4, 1, 16, 32);
	//bullet[57] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime58.png", 4, 4, 1, 16, 32);
	//bullet[58] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime59.png", 4, 4, 1, 16, 32);
	//bullet[59] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime60.png", 4, 4, 1, 16, 32);
	//bullet[60] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime61.png", 4, 4, 1, 16, 32);
	//bullet[61] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime62.png", 4, 4, 1, 16, 32);
	//bullet[62] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime63.png", 4, 4, 1, 16, 32);
	//bullet[63] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime64.png", 4, 4, 1, 16, 32);
	//bullet[64] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime65.png", 4, 4, 1, 16, 32);
	//bullet[65] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime66.png", 4, 4, 1, 16, 32);
	//bullet[66] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime67.png", 4, 4, 1, 16, 32);
	//bullet[67] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime68.png", 4, 4, 1, 16, 32);
	//bullet[68] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime69.png", 4, 4, 1, 16, 32);
	//bullet[69] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime70.png", 4, 4, 1, 16, 32);
	//bullet[70] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime71.png", 4, 4, 1, 16, 32);
	//bullet[71] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime72.png", 4, 4, 1, 16, 32);
	//bullet[72] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime73.png", 4, 4, 1, 16, 32);
	//bullet[73] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime74.png", 4, 4, 1, 16, 32);
	//bullet[74] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime75.png", 4, 4, 1, 16, 32);
	//bullet[75] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime76.png", 4, 4, 1, 16, 32);
	//bullet[76] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime77.png", 4, 4, 1, 16, 32);
	//bullet[77] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime78.png", 4, 4, 1, 16, 32);
	//bullet[78] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime79.png", 4, 4, 1, 16, 32);
	//bullet[79] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime80.png", 4, 4, 1, 16, 32);
	//bullet[80] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime81.png", 4, 4, 1, 16, 32);
	//bullet[81] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime82.png", 4, 4, 1, 16, 32);
	//bullet[82] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime83.png", 4, 4, 1, 16, 32);
	//bullet[83] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime84.png", 4, 4, 1, 16, 32);
	//bullet[84] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime85.png", 4, 4, 1, 16, 32);
	//bullet[85] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime86.png", 4, 4, 1, 16, 32);
	//bullet[86] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime87.png", 4, 4, 1, 16, 32);
	//bullet[87] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime88.png", 4, 4, 1, 16, 32);
	//bullet[88] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime89.png", 4, 4, 1, 16, 32);
	//bullet[89] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime90.png", 4, 4, 1, 16, 32);
	//bullet[90] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime91.png", 4, 4, 1, 16, 32);
	//bullet[91] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime92.png", 4, 4, 1, 16, 32);
	//bullet[92] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime93.png", 4, 4, 1, 16, 32);
	//bullet[93] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime94.png", 4, 4, 1, 16, 32);
	//bullet[94] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime95.png", 4, 4, 1, 16, 32);
	//bullet[95] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime96.png", 4, 4, 1, 16, 32);
	//bullet[96] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime97.png", 4, 4, 1, 16, 32);
	//bullet[97] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime98.png", 4, 4, 1, 16, 32);
	//bullet[98] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime99.png", 4, 4, 1, 16, 32);
	//bullet[99] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime100.png", 4, 4, 1, 16, 32);
	//bullet[100] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime101.png", 4, 4, 1, 16, 32);
	//bullet[101] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime102.png", 4, 4, 1, 16, 32);
	//bullet[102] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime103.png", 4, 4, 1, 16, 32);
	//bullet[103] = rm->GetImages("Resource/Image/Object/Enemy/Enemy_Bullet/Blue/standard/anime104.png", 4, 4, 1, 16, 32);

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
void EnemyShot4::Update(float delta_second)
{
	//GameObjectBase::AnimationControl(delta_second, images, {36, 37, 38, 39, 40, 41, 42, 43, 42, 41, 40, 39, 38, 37}, 10.0f);
	//GameObjectBase::AnimationControl(delta_second, images, {0, 1, 2, 3, 2, 1}, 10.0f);
	move_time += delta_second;

	// 弾が画面外に出たら削除（移動方向によって判定を変える）
	if (location.y < -100.0f || // 上方向
		location.y > D_WIN_MAX_Y + 100.0f ||
		location.x > D_WIN_MAX_X - 100.0f ||
		location.x < 0 + 100.0f) // 下方向
	{
		is_destroy = true;
	}

	location += velocity * delta_second;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void EnemyShot4::Draw(const Vector2D& screen_offset) const
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
	//DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color, TRUE);
	//DrawTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, color, TRUE);

	DrawRotaGraph(location.x, location.y, 1.9f, angle, image, TRUE);
}

// 終了時処理
void EnemyShot4::Finalize()
{
}

void EnemyShot4::SetAttackPattrn(int pattrn)
{
	attack_pattrn = pattrn;
	if (pattrn <= 1)
	{
		image = bullet_1[0];
	}
	else if (pattrn == 2)
	{
		image = bullet_2[0];
	}
	else
	{

	}

}
