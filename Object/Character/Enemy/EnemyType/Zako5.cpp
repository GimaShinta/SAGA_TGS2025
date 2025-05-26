// Zako5.cpp（Stage3 の攻撃パターン4?7を取り込んだ中ボス）
#include "Zako5.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot4.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot5.h"
#include <cmath>

Zako5::Zako5() {}
Zako5::~Zako5() {}

void Zako5::Initialize()
{
    enemy_type = ENE_ZAKO5;
    z_layer = 2;
    velocity = Vector2D(0, 20);
    box_size = Vector2D(40, 40);
    hp = 1000;
    velocity = Vector2D(0, 30);  // 下にゆっくり移動


    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    image = rm->GetImages("Resource/Image/Object/Enemy/Zako5/enemy53.png")[0];

}

void Zako5::Update(float delta_second)
{
    GameObjectBase::AnimationControl(image);
    location += velocity * delta_second;

    // 画面範囲で上下反転
    if (location.y < 100 || location.y > 500)
        velocity.y *= -1;

    Shot(delta_second);

    if (hp <= 0) is_destroy = true;

    __super::Update(delta_second);
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
