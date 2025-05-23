#include "Zako1.h"
#include "../../Player/Player.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../../../Utility/ScoreData.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

// コンストラクタ（ランダム初期化）
Zako::Zako()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

// デストラクタ
Zako::~Zako()
{}

// 初期化処理
void Zako::Initialize()
{
    enemy_type = ENE_ZAKO1;
    z_layer = 2;
    box_size = 12;
    hp = 0;

    // 衝突判定の設定
    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;
    start_location = location;
    is_returning = false;
    has_shot = false;

    // 画像読み込み
    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images_a = rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_a.png", 4, 4, 1, 32, 32);
    images_b = rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_b.png", 12, 12, 1, 24, 24);

    images = images_a;
    image = images[0];

    // 行動パターンをランダムに決定
    ChangePatternRandomly();
}

// 更新処理
void Zako::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

    // アニメーション制御
    GameObjectBase::AnimationControl(delta_second, images, anim_indices, 10.0f);

    // 現在の行動パターンに応じて移動・攻撃処理を実行
    switch (pattern)
    {
        case ZakoPattern::MoveStraight:
            velocity = { 0, 120 };
            break;

        case ZakoPattern::RightMove:
            velocity = { 120, 0 };
            break;

        case ZakoPattern::LeftMove:
            velocity = { -120, 0 };
            break;

        case ZakoPattern::ZIgzag:
            // ジグザグ移動しつつ攻撃
            velocity.x = sinf(pattern_timer * 1.5f) * 320;
            velocity.y = 100;
            Shot(delta_second);
            break;

        case ZakoPattern::MoveAndStopShoot:
            // 一定位置で停止後に攻撃し、後退
            if (!has_shot)
            {
                if (location.y < 300)
                {
                    velocity = { 0, 100 };
                }
                else
                {
                    velocity = { 0, 0 };
                    shot_timer += delta_second;

                    if (shot_timer >= 2.0f && player)
                    {
                        Shot(delta_second);
                        has_shot = true;
                        shot_timer = 0.0f;
                    }
                }
            }
            else
            {
                velocity = { 0, 0 };
                after_shot_timer += delta_second;

                if (after_shot_timer >= 2.0f)
                {
                    velocity = { 0, -150 }; // 後退
                }
            }
            break;

        case ZakoPattern::MoveThenDiagonal:
            // 最初は直進、のちに斜め移動
            if (pattern_timer < 1.0f)
            {
                velocity = { 0, 100 };
            }
            else
            {
                velocity = { 100, 100 };
            }
            break;
    }

    // 位置更新
    location += velocity * delta_second;

    // HPが0以下なら破壊処理
    if (hp <= 0)
    {
        is_destroy = true;

        // 爆発エフェクト再生
        AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
        anim_id = manager->PlayerAnimation(EffectName::eExprotion, location, 0.05f, false);
        manager->SetScale(anim_id, 0.5f); // スケール調整

        // スコア加算
        Singleton<ScoreData>::GetInstance()->SetScoreData(100);
    }

    __super::Update(delta_second);
}

// 描画処理
void Zako::Draw(const Vector2D& screen_offset) const
{
    // HP表示（デバッグ用表示）
    DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
    DrawRotaGraph(location.x, location.y, 1.5f, 0.0f, image, TRUE);
    DrawFormatString(location.x - 10, location.y - 40, GetColor(255, 255, 255), "%.0f", hp);
}

// 終了処理（未使用だがオーバーライドのため定義）
void Zako::Finalize()
{}

// 敵の攻撃処理（プレイヤーの方向に弾を発射）
void Zako::Shot(float delta_second)
{
    shot_timer += delta_second;

    if (shot_timer >= 2.0f && player)
    {
        // プレイヤーの方向を計算
        Vector2D dir = player->GetLocation() - location;
        float len = dir.Length();
        if (len > 0) dir /= len;

        // 弾生成
        auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot2>(location);
        shot->SetVelocity(dir);

        shot_timer = 0.0f;
    }
}

// 行動パターンをランダムに選ぶ
void Zako::ChangePatternRandomly()
{
    int r = rand() % 6;
    SetPattern(static_cast<ZakoPattern>(r));
}

// 行動パターンの設定と初期化
void Zako::SetPattern(ZakoPattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f;
    has_shot = false;
    after_shot_timer = 0.0f;

    // パターンごとのHP・アニメ・画像設定
    switch (pattern)
    {
        case ZakoPattern::MoveStraight:
            hp = 10;
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            break;

        case ZakoPattern::RightMove:
        case ZakoPattern::LeftMove:
            hp = 15;
            images = images_a;
            anim_indices = { 0, 1, 2, 3 };
            break;

        case ZakoPattern::ZIgzag:
        case ZakoPattern::MoveThenDiagonal:
            hp = 20;
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            break;

        case ZakoPattern::MoveAndStopShoot:
            hp = 30;
            images = images_a;
            anim_indices = { 0, 1, 2, 3 };
            break;
    }

    // 初期フレームに設定
    image = images[0];
}
