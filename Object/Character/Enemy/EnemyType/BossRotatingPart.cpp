#include "BossRotatingPart.h"
#include "Stage2BOSS.h"
#include "../../../../Utility/ResourceManager.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include "../../../../Object/Character/Shot/EnemyBeam1.h"
#include "../../../../Object/Character/Shot/EnemyShot/FollowBeam.h"
#include "../../../../Object/Character/Player/Player.h" // Å© Player égópÇÃÇΩÇﬂ
#include <cmath>

constexpr float PI = 3.1415926f;

void BossRotatingPart::SetUp(Stage2Boss* boss, float radius, float angle_offset)
{
    this->boss = boss;
    this->radius = radius;
    this->angle_offset = angle_offset;
    this->angle = 0.0f;
}

void BossRotatingPart::Initialize()
{
    z_layer = 3;
    box_size = 20;

    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemyPart;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images = rm->GetImages("Resource/Image/Object/Enemy/Boss/s1_Boss/anime_enemy74_a.png", 6, 6, 1, 48, 48);
}

void BossRotatingPart::Update(float delta_second)
{
    if (!boss) return;

    float rotation_speed = 0.5f;

    if (boss->IsHpBelowThreshold())
    {
        if (boss->IsTransforming())
        {
            rotation_speed = 0.0f;
        }
        else
        {
            float t = boss->GetTransformProgress();
            rotation_speed = 0.5f + t * 1.0f;
        }
    }

    angle += delta_second * rotation_speed * rotation_direction;

    float theta = angle + angle_offset;
    Vector2D center = boss->GetLocation();
    location.x = center.x + std::cos(theta) * radius;
    location.y = center.y + std::sin(theta) * radius;

    __super::Update(delta_second);

    if (group_id != boss->GetActiveGroupID() &&
        attack_direction != AttackDirectionType::Omnidirectional)
    {
        image_index = 0;
        return;
    }

    shot_timer += delta_second;
    if (shot_timer >= 2.0f)
    {
        shot_timer = 0.0f;

        anim_timer += delta_second;
        if (anim_timer >= 0.1f)
        {
            image_index = (image_index + 1) % images.size();
            anim_timer = 0.0f;
        }

        switch (attack_direction)
        {
            case AttackDirectionType::Inward:
            {
                Vector2D core_pos = boss->GetLocation();
                Vector2D dir = core_pos - location;
                float len = dir.Length();
                if (len > 0.01f) dir /= len;

                auto beam = Singleton<GameObjectManager>::GetInstance()->CreateObject<FollowBeam>(location);
                beam->SetBeamTarget(core_pos);
                beam->SetFollowTarget(this, dir);
                break;
            }

            case AttackDirectionType::Outward:
            {
                float shoot_angle = angle_offset;
                Vector2D dir = { std::cos(shoot_angle), std::sin(shoot_angle) };

                auto beam = Singleton<GameObjectManager>::GetInstance()->CreateObject<FollowBeam>(location);
                beam->SetBeamTarget(location + dir * 1000.0f);
                beam->SetFollowTarget(this, dir);
                break;
            }

            case AttackDirectionType::Spiral:
            {
                static float spiral_angle = 0.0f;
                spiral_angle += delta_second * 3.0f;

                constexpr int shot_count = 6;
                for (int i = 0; i < shot_count; ++i)
                {
                    float angle_rad = spiral_angle + 2.0f * PI * i / shot_count;
                    Vector2D dir = { cos(angle_rad), sin(angle_rad) };
                    auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot1>(location);
                    shot->SetVelocity(dir * 200.0f);
                }
                break;
            }


            case AttackDirectionType::RainShot:
            {
                constexpr int rain_count = 10;
                for (int i = 0; i < rain_count; ++i)
                {
                    float rand_angle = ((float)(rand() % 60) - 30.0f) * PI / 180.0f;
                    Vector2D dir = { sin(rand_angle), 1.0f };
                    dir = dir.Normalize();
                    auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot1>(location);
                    shot->SetVelocity(dir * 400.0f);
                }
                break;
            }

            case AttackDirectionType::HomingBurst:
            {
                if (!player) break;

                Vector2D dir = player->GetLocation() - location;
                float len = dir.Length();
                if (len > 0.01f) dir /= len;

                for (int i = -1; i <= 1; ++i)
                {
                    Vector2D offset_dir = { dir.x + i * 0.1f, dir.y };
                    float len2 = offset_dir.Length();
                    if (len2 > 0.01f) offset_dir /= len2;

                    auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot1>(location);
                    shot->SetVelocity(offset_dir * 350.0f);
                }
                break;
            }

            case AttackDirectionType::Omnidirectional:
            {
                if (boss->IsHpBelowThreshold())
                {
                    constexpr int num_shots = 12;
                    for (int i = 0; i < num_shots; ++i)
                    {
                        float angle_rad = 2.0f * PI * i / num_shots;
                        Vector2D dir = { std::cos(angle_rad), std::sin(angle_rad) };

                        auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot1>(location);
                        shot->SetVelocity(dir * 300.0f);
                        shot->SetOmni(true);
                    }
                }
                break;
            }

            default:
                break;
        }
    }
    else
    {
        image_index = 0;
    }
}

void BossRotatingPart::Draw(const Vector2D& screen_offset) const
{
    if (!images.empty())
    {
        DrawRotaGraph(location.x, location.y, 1.5f, angle, images[image_index], TRUE);
    }
    else
    {
        DrawCircle(static_cast<int>(location.x), static_cast<int>(location.y), 20, GetColor(255, 0, 0), TRUE);
    }
}

void BossRotatingPart::OnHitCollision(GameObjectBase* hit_object)
{
    if (hit_object->GetCollision().object_type == eObjectType::eShot ||
        hit_object->GetCollision().object_type == eObjectType::eBeam)
    {
        hit_object->SetDestroy();
    }
}
