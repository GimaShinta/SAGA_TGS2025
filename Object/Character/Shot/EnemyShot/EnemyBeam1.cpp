#include "../EnemyBeam1.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"
#include "../../Enemy/EnemyType/Boss2.h"

EnemyBeam1::EnemyBeam1()
{
}

EnemyBeam1::~EnemyBeam1()
{
}

// 初期化処理
void EnemyBeam1::Initialize()
{
	z_layer = 4;
	box_size = Vector2D(48.0f, 800.0f);

	// 当たり判定のオブジェクト設定
	collision.is_blocking = true;
	// 自分のオブジェクトタイプ
	collision.object_type = eObjectType::eEnemyBeam;
	// 当たる相手のオブジェクトタイプ
	collision.hit_object_type.push_back(eObjectType::ePlayer);

	// 動くかどうか（trueなら動く、falseなら止まる）
	is_mobility = true;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delata_second">1フレーム当たりの時間</param>
void EnemyBeam1::Update(float delta_second)
{
#if 0
    beam_time += delta_second;
    alpha_timer += delta_second;

    // 太さの変化（細い → 太い）
    float max_thickness = 48.0f;
    float min_thickness = 8.0f;
    float thickness = min_thickness;

    if (beam_time < growth_duration)
    {
        float t = beam_time / growth_duration;
        thickness = min_thickness + (max_thickness - min_thickness) * t;
    }
    else
    {
        thickness = max_thickness;
    }

    box_size = Vector2D(thickness, box_size.y);

    // ビームの終了処理
    if (beam_time >= 5.0f)
    {
        SetDestroy();
        beam_time = 0.0f;
    }

    location += velocity * delta_second;


#else

    beam_time += delta_second;

    // --- 太さ変化処理 ----------------------------
    float max_thickness = 48.0f;         // 最大の太さ
    float min_thickness = 8.0f;          // 初期の細さ
    float growth_duration = 0.2f;        // 何秒で最大太さになるか

    float thickness = min_thickness;

    if (beam_time < growth_duration) {
        // 線形補間で徐々に太くする
        float t = beam_time / growth_duration;
        thickness = min_thickness + (max_thickness - min_thickness) * t;
    }
    else {
        thickness = max_thickness;  // 一定時間後に最大太さに固定
    }

    box_size = Vector2D(thickness, box_size.y);  // 横幅（太さ）だけ変える
    // --------------------------------------------

    if (beam_time >= 5.0f)
    {
        SetDestroy(); // 明示的に削除
        beam_time = 0.0f;
    }

    location += velocity * delta_second;
#endif
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="screen_offset"></param>
void EnemyBeam1::Draw(const Vector2D& screen_offset) const
{
#if 0
    // アルファ（透明度）をサイン波で変化 → 点滅
    float alpha = 255.0f;
    if (beam_time < growth_duration)
    {
        alpha = 128 + 127 * sinf(alpha_timer * 10.0f);  // 点滅周期は10Hz
    }

    int r = 255;
    int g = 0;
    int b = 255;

    int color = GetColor(r, g, b);
    int alpha_i = static_cast<int>(alpha);

    // 透過付き描画（DxLib）
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_i);

    DrawBox(
        static_cast<int>(location.x - box_size.x),
        static_cast<int>(location.y - box_size.y),
        static_cast<int>(location.x + box_size.x),
        static_cast<int>(location.y + box_size.y),
        color,
        TRUE
    );

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);  // 元に戻す
#else
    //当たり判定の可視化
    DrawBox(location.x - box_size.x, location.y - box_size.y,
        location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 255), TRUE);
#endif

}

// 終了時処理
void EnemyBeam1::Finalize()
{
}

void EnemyBeam1::OnHitCollision(GameObjectBase* hit_object)
{
}

void EnemyBeam1::SetBoss2(Boss2* p_boss)
{
	boss2 = p_boss;
}
