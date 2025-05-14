#include "Zako1.h"
#include "../../Player/Player.h"
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
    hp = 10;

    // 衝突判定設定
    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;
    start_location = location;
    is_returning = false;
   

    ChangePatternRandomly(); // 初期パターンをランダムで決定
}

void Zako1::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

   

    // パターンに応じて移動方向を設定
    switch (pattern)
    {
        case Zako1Pattern::MoveStraight: velocity = { 0, 120 };  break;
        case Zako1Pattern::RightMove:    velocity = { 120, 0 };  break;
        case Zako1Pattern::LeftMove:     velocity = { -120, 0 }; break;
        case Zako1Pattern::ZIgzag:       velocity.x = sinf(pattern_timer * 1.5f) * 160;
                                         velocity.y = 100;       break;

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
    // 後処理が必要な場合に記述
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
    int r = rand() % 4; 

    switch (r)
    {
        case 0: pattern = Zako1Pattern::MoveStraight; break;
        case 1: pattern = Zako1Pattern::RightMove;    break;
        case 2: pattern = Zako1Pattern::LeftMove;     break;
        case 3: pattern = Zako1Pattern::ZIgzag;       break;
    }

    pattern_timer = 0.0f;
}

void Zako1::SetPattern(Zako1Pattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f;
}
