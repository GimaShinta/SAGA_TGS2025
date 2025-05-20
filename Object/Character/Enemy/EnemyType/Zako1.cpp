#include "Zako1.h"
#include "../../Player/Player.h"
#include"../../../../Utility/AnimationManager.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

Zako1::Zako1()
{
    srand(static_cast<unsigned int>(time(nullptr))); // 乱数初期化
}

Zako1::~Zako1()
{}

void Zako1::Initialize()
{
    enemy_type = ENE_ZAKO1;
    z_layer = 2;
    box_size = 12;
    hp = 30;

    // 衝突判定設定
    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;
    start_location = location;
    is_returning = false;
    has_shot = false;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images = rm->GetImages("Resource/Image/Object/Enemy/Zako2/anime_enemy45.png", 6, 6, 1, 32, 32);

    /*AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    anim_id =  manager->PlayerAnimation(image_num, location, 0.01f, true);*/


   

    ChangePatternRandomly(); // 初期パターンをランダムで決定
}

void Zako1::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

    GameObjectBase::AnimationControl(delta_second, images, {1, 2, 3, 4, 5, 4, 3, 2}, 1.0f);

  /*  AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->SetPosition(anim_id, location); */

    // パターンに応じて移動方向を設定
    switch (pattern)
    {
        case Zako1Pattern::MoveStraight:
            velocity = { 0, 120 };
            break;
        case Zako1Pattern::RightMove:
            velocity = { 120, 0 };
            break;
        case Zako1Pattern::LeftMove:
            velocity = { -120, 0 };
            break;
        case Zako1Pattern::ZIgzag:
            velocity.x = sinf(pattern_timer * 1.5f) * 320;
            velocity.y = 100;
            Shot(delta_second);
            break;

        case Zako1Pattern::MoveAndStopShoot:
            if (!has_shot)
            {
                // 降下処理
                if (location.y < 300)
                {
                    velocity = { 0, 100 };
                }
                else
                {
                    velocity = { 0, 0 }; // 停止
                    shot_timer += delta_second;

                    if (shot_timer >= 2.0f && player)
                    {
                        Shot(delta_second);        // 弾発射
                        has_shot = true;
                        shot_timer = 0.0f;
                    }
                }
            }
            else
            {
                velocity = { 0, 0 }; // 弾を撃った後もしばらく静止

                after_shot_timer += delta_second;
                if (after_shot_timer >= 2.0f)
                {
                    velocity = { 0, -150 }; // 上昇開始
                    if (location.y + box_size.y < 0)
                    {
                        ChangePatternRandomly(); // 画面外で次へ
                    }
                }
            }
            break;


        case Zako1Pattern::MoveThenDiagonal:
            if (pattern_timer < 1.0f)
            {
                velocity = { 0, 100 }; // 最初は真下
            }
            else
            {
                velocity = { 100, 100 }; // 斜め右下（左下なら -100 に）
            }
            break;
    }


    location += velocity * delta_second; // 移動処理

    if (hp <= 0)
    {
        is_destroy = true; // 撃破フラグ
        Singleton<ScoreData>::GetInstance()->SetScoreData(100); // スコア加算
    }

    __super::Update(delta_second);
}

void Zako1::Draw(const Vector2D& screen_offset) const
{
    // 敵の当たり判定とHP表示
    DrawBox(location.x - box_size.x, location.y - box_size.y,
            location.x + box_size.x, location.y + box_size.y, GetColor(0, 0, 255), TRUE);

    DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
   

}

void Zako1::Finalize()
{
       
}

void Zako1::Shot(float delta_second)
{
    shot_timer += delta_second;

    if (shot_timer >= 2.0f && player)
    {
        // プレイヤー方向に発射
        Vector2D dir = player->GetLocation() - location;
        float len = dir.Length();
        if (len > 0) dir /= len;

        auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot2>(location);
        shot->SetVelocity(dir); // 方向設定

        shot_timer = 0.0f;
    }
}

void Zako1::ChangePatternRandomly()
{
    int r = rand() % 6; // 6パターンに拡張
    switch (r)
    {
        case 0: pattern = Zako1Pattern::MoveStraight; break;
        case 1: pattern = Zako1Pattern::RightMove;    break;
        case 2: pattern = Zako1Pattern::LeftMove;     break;
        case 3: pattern = Zako1Pattern::ZIgzag;       break;
        case 4: pattern = Zako1Pattern::MoveAndStopShoot; break;
        case 5: pattern = Zako1Pattern::MoveThenDiagonal; break;
    }


    pattern_timer = 0.0f;
}

void Zako1::SetPattern(Zako1Pattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f;
    has_shot = false;
    after_shot_timer = 0.0f;
}
