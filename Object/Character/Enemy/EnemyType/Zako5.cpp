#include "Zako5.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot4.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot5.h"
#include "../../../../Utility/AnimationManager.h"
#include <cmath>

Zako5::Zako5()
{}

Zako5::Zako5(const Vector2D& pos)
{
    location = pos;
}

Zako5::~Zako5()
{}

void Zako5::Initialize()
{
    enemy_type = ENE_ZAKO5;
    z_layer = 2;
    box_size = Vector2D(120, 40);
    hp = 1700;

    phase = Zako5Phase::Appearing;
    appear_timer = 0.0f;
    is_invincible = true;

    scale = 4.0f; // ç≈èâÇÕëÂÇ´Ç≠ìoèÍ
    alpha = 0;

    velocity = Vector2D(0, 0);

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

                if (velocity.x == 0.0f)
                {
                    if (location.x < 400.0f)
                        velocity = Vector2D(200.0f, 0.0f);
                    else if (location.x > 900.0f)
                        velocity = Vector2D(-200.0f, 0.0f);
                }

                // scaleÇ3.0Ç©ÇÁ2.0Ç…ÇæÇÒÇæÇÒèkè¨
                {
                    float t = appear_timer ;
                    if (t > 1.0f) t = 1.0f;
                    scale = 4.0f - (2.50f * t);  // 3.0 Å® 2.0
                }

                location += velocity * delta;

                if (velocity.x > 0 && location.x >= 450.0f)
                {
                    location.x = 450.0f;
                    velocity = Vector2D(0.0f, 30.0f);
                }
                else if (velocity.x < 0 && location.x <= 830.0f)
                {
                    location.x = 830.0f;
                    velocity = Vector2D(0.0f, 30.0f);
                }

                if (appear_timer >= 2.0f)
                {
                    phase = Zako5Phase::Fighting;
                    is_invincible = false;
                    scale = 1.5f;  // FightingÉtÉFÅ[ÉYÇÕ2.0ÇÃÇ‹Ç‹
                }
                break;

        case Zako5Phase::Fighting:
            location += velocity * delta;

            if (location.y < 100.0f || location.y > 500.0f)
                velocity.y *= -1;

            Shot(delta);

            if (hp <= 0) {

                is_destroy = true;
                AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
                anim_id = manager->PlayerAnimation(EffectName::eExprotion2, location, 0.035f, false);
                manager->SetScale(anim_id, 1.5f);
                      }
            break;
    }

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
            attack_cooldown = 4.0f;
            spiral_timer = 0.0f;
            spiral_total_time = 0.0f;
        }
    }

    if (is_attacking)
    {
        switch (attack_pattern)
        {
            case 6: Pattern6(delta_second); break;
            case 7: Pattern7(delta_second); break;
        }

        if (spiral_total_time >= 2.5f)
        {
            is_attacking = false;
            attack_pattern++;
            if (attack_pattern > 7) attack_pattern = 6;
        }
    }
}

void Zako5::Pattern6(float delta_second)
{
    spiral_timer += delta_second;
    spiral_total_time += delta_second;

    if (spiral_timer >= 1.6f)
    {
        spiral_timer = 0.0f;

        const int bullet_num = 5;
        const float base_angle = 90.0f;
        const float fan_range = 120.0f;
        const float speed = 180.0f;

        for (int i = 0; i < bullet_num; ++i)
        {
            float angle = base_angle - (fan_range / 2) + (fan_range / (bullet_num - 1)) * i;
            float rad = angle * DX_PI / 180.0f;
            Vector2D vel(cos(rad) * speed, sin(rad) * speed);

            auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot4>(location);
            shot->SetVelocity(vel);
            shot->SetAttackPattrn(1);
        }
    }
}

void Zako5::Pattern7(float delta_second)
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
}

void Zako5::Draw(const Vector2D& screen_offset) const
{
    DrawRotaGraph(location.x, location.y, scale, 3.14, image, TRUE);
   // DrawFormatString(location.x - 20, location.y - 30, GetColor(255, 255, 255), "Zako5: %.0f", hp);
   
}

void Zako5::Finalize()
{}
