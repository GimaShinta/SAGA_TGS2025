// Zako5.cpp
#include "Zako5.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot4.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot5.h"
#include <cmath>

Zako5::Zako5()
{}

// コンストラクタで位置を受け取るように修正（もしまだなら）
Zako5::Zako5(const Vector2D& pos)
{
    location = pos;  // 位置セット
}

Zako5::~Zako5()
{}

void Zako5::Initialize()
{
    enemy_type = ENE_ZAKO5;
    z_layer = 2;
    box_size = Vector2D(40, 40);
    hp = 1000;

    phase = Zako5Phase::Appearing;
    appear_timer = 0.0f;
    is_invincible = true;

   

    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    image = rm->GetImages("Resource/Image/Object/Enemy/Zako5/enemy53.png")[0];
}

void Zako5::Update(float delta)
{
    switch (phase)
    {
        case Zako5Phase::Appearing:
            appear_timer += delta;

            // 横方向速度セット（初回だけ or velocityがゼロなら）
            if (velocity.x == 0.0f)
            {
                if (location.x < 400.0f)
                    velocity = Vector2D(100.0f, 0.0f);
                else if (location.x > 900.0f)
                    velocity = Vector2D(-100.0f, 0.0f);
            }

            // 横方向に移動
            location += velocity * delta;

            if (velocity.x > 0 && location.x >= 450.0f)  // 右方向の敵は450で止める（元は400）
            {
                location.x = 450.0f;
                velocity = Vector2D(0.0f, 30.0f);
            }
            else if (velocity.x < 0 && location.x <= 830.0f)  // 左方向はそのまま
            {
                location.x = 830.0f;
                velocity = Vector2D(0.0f, 30.0f);
            }

            if (appear_timer >= 2.0f)
            {
                phase = Zako5Phase::Fighting;
                is_invincible = false;
            }
            break;

        case Zako5Phase::Fighting:
            // 縦方向の上下反転移動
            location += velocity * delta;
            if (location.y < 100.0f || location.y > 500.0f)
                velocity.y *= -1;

            Shot(delta);

            if (hp <= 0)
                is_destroy = true;
            break;
    }

    // 位置を反映（もしGetLocation()/SetLocation()使うならここで）
    SetLocation(location);

    __super::Update(delta);
}




void Zako5::Shot(float delta_second)
{
    if (!is_attacking)
    {
        attack_cooldown -= delta_second;
        if (attack_cooldown <= 0.0f)
        {
            is_attacking = true;
            attack_cooldown = 4.0f;  // 次の攻撃までの間隔
            spiral_timer = 0.0f;
            spiral_total_time = 0.0f;
        }
    }

    if (is_attacking)
    {
        switch (attack_pattern)
        {
        case 6: Pattrn6(delta_second); break;
        case 7: Pattrn7(delta_second); break;
        }

        if (spiral_total_time >= 2.5f)
        {
            is_attacking = false;
            attack_pattern++;
            if (attack_pattern > 7) attack_pattern = 6;
        }
    }
}



void Zako5::Draw(const Vector2D& screen_offset) const
{
    DrawBox(location.x - box_size.x, location.y - box_size.y,
        location.x + box_size.x, location.y + box_size.y,
        GetColor(255, 100, 0), TRUE);
    DrawRotaGraph(location.x, location.y, 1.5f, 3.14, image, TRUE);

    DrawFormatString(location.x - 20, location.y - 30, GetColor(255, 255, 255), "Zako5: %.0f", hp);
}

void Zako5::Finalize() {}

void Zako5::Pattrn6(float delta_second)
{
    spiral_timer += delta_second;
    spiral_total_time += delta_second;

    if (spiral_timer >= 1.6f)
    {
        spiral_timer = 0.0f;

        const int bullet_num = 5;
        const float base_angle = 90.0f;  // 下方向中心
        const float fan_range = 120.0f;
        const float speed = 180.0f;

        for (int i = 0; i < bullet_num; ++i)
        {
            float angle = base_angle - (fan_range / 2) + (fan_range / (bullet_num - 1)) * i;
            float rad = angle * DX_PI / 180.0f;
            Vector2D vel(cos(rad) * speed, sin(rad) * speed);

            auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot4>(location);
            shot->SetVelocity(vel);
        }
    }

    if (spiral_total_time >= 1.5f)
    {
        spiral_total_time = 0.0f;
    }
}


// --- Pattern 7 ---
void Zako5::Pattrn7(float delta_second)
{
    spiral_timer += delta_second;
    spiral_total_time += delta_second;

    if (spiral_timer >= 1.6f)
    {
        spiral_timer = 0.0f;

        const int bullet_num = 5;
        const float base_angle = 90.0f;
        const float fan_range = 60.0f;
        const float speed = 160.0f;

        for (int i = 0; i < bullet_num; ++i)
        {
            float angle = base_angle - (fan_range / 2) + (fan_range / (bullet_num - 1)) * i;
            float rad = angle * DX_PI / 180.0f;
            Vector2D vel(cos(rad) * speed, sin(rad) * speed);

            auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot5>(location);
            shot->SetVelocity(vel);
        }
    }

    if (spiral_total_time >= 1.2f)
    {
        spiral_total_time = 0.0f;
    }
}
