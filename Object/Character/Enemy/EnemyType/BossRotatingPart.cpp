#include "BossRotatingPart.h"
#include "Stage2BOSS.h"
#include "../../../../Utility/ResourceManager.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include "../../../../Object/Character/Shot/EnemyBeam1.h"
#include "../../../../Object/Character/Shot/EnemyShot/FollowBeam.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/WarningBeam.h"
#include "../../../../Object/Character/Shot/EnemyShot/LinkedWarningBeam.h"
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

    appear_timer = 0.0f;
    is_appearing = true;
    scale = 0.0f;
    alpha = 0;
}

void BossRotatingPart::Update(float delta_second)
{
    if (!boss || !boss->IsInAttackPhase()) return;

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

    if (is_appearing)
    {
        appear_timer += delta_second;
        float t = appear_timer / 1.0f;
        if (t > 1.0f) t = 1.0f;
        scale = 0.3f + 0.7f * t;
        alpha = static_cast<int>(255 * t);

        if (t >= 1.0f)
        {
            is_appearing = false;
            scale = 1.0f;
            alpha = 255;
        }
    }

    __super::Update(delta_second);

    anim_timer += delta_second;
    if (anim_timer >= 0.1f)
    {
        image_index = (image_index + 1) % images.size();
        anim_timer = 0.0f;
    }

    bool is_allowed = (group_id == boss->GetActiveGroupID() ||
                       attack_direction == AttackDirectionType::Omnidirectional ||
                       attack_direction == AttackDirectionType::Spiral ||
                       attack_direction == AttackDirectionType::RainShot ||
                       attack_direction == AttackDirectionType::HomingBurst);

    if (!is_allowed) return;

    float base_interval = 3.5f;
    int hp = boss->GetHP();
    if (hp < 6666 && hp >= 3333) base_interval = 2.5f;
    else if (hp < 3333) base_interval = 2.0f;

    shot_timer += delta_second;
    if (shot_timer >= base_interval)
    {
        shot_timer = 0.0f;

        switch (attack_direction)
        {
            case AttackDirectionType::Inward:
            {
                auto warn = Singleton<GameObjectManager>::GetInstance()->CreateObject<WarningBeam>(location);
                warn->SetUp(Vector2D(0, 0), 1000.0f, 1.0f);
                warn->SetFollowAndLookTarget(this, boss);
                break;
            }
            case AttackDirectionType::Outward:
            {
                float shoot_angle = angle_offset;
                Vector2D dir = { std::cos(shoot_angle), std::sin(shoot_angle) };
                auto warn = Singleton<GameObjectManager>::GetInstance()->CreateObject<WarningBeam>(location);
                warn->SetUp(dir, 1000.0f, 1.0f);
                warn->SetFollowAndLookTarget(this, nullptr);
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
                    shot->SetVelocity(dir * 50.0f);
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
                        shot->SetVelocity(dir * 200.0f);
                        shot->SetOmni(true);
                    }
                }
                break;
            }
            case AttackDirectionType::LinkedDown:
            {
                const auto& parts = boss->GetRotatingParts();

                for (size_t i = 0; i + 1 < parts.size(); i += 2)
                {
                    GameObjectBase* partA = parts[i];
                    GameObjectBase* partB = parts[i + 1];

                    if (partA && partB)
                    {
                        auto warn = Singleton<GameObjectManager>::GetInstance()->CreateObject<LinkedWarningBeam>(partA->GetLocation());
                        warn->SetEndpoints(partA, partB);
                        warn->SetLifeTime(1.0f);
                    }
                }

                break;
            }

            default:
                break;
        }
    }
}

void BossRotatingPart::Draw(const Vector2D& screen_offset) const
{
    if (!images.empty())
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawRotaGraph(location.x, location.y, scale, angle, images[image_index], TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    else
    {
        DrawCircle(static_cast<int>(location.x), static_cast<int>(location.y), 20, GetColor(255, 0, 0), TRUE);
    }
}

void BossRotatingPart::OnHitCollision(GameObjectBase* hit_object)
{
    if (hit_object->GetCollision().object_type == eObjectType::eShot)
    {
        hit_object->SetDestroy();
    }
}
