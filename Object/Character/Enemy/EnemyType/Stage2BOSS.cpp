#include "Stage2BOSS.h"
#include "BossRotatingPart.h"
#include "../../../../Utility/ResourceManager.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../Shot/EnemyShot/EnemyShot2.h"
#include <cmath>

Stage2Boss::Stage2Boss()
{}
Stage2Boss::~Stage2Boss()
{}

void Stage2Boss::Initialize()
{
    hp = 10000;
    box_size = 50;
    z_layer = 2;
    enemy_type = ENE_ZAKO1;

    collision.is_blocking = true;
    collision.object_type = eObjectType::eBoss2;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    location = { 640.0f, 250.0f };
    velocity = { 0.0f, 0.0f };

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images_a = rm->GetImages("Resource/Image/Object/Enemy/Boss/s1_Boss/anime_enemy74_b.png", 6, 6, 1, 48, 48);
    images_b = rm->GetImages("Resource/Image/Object/Enemy/Boss/s1_Boss/anime_enemy74_b.png", 6, 6, 1, 48, 48);
    images = images_b;
    anim_indices = { 0, 1, 2, 3, 4, 5 };
    image = images[0];

    auto* manager = Singleton<GameObjectManager>::GetInstance();
    for (int i = 0; i < 6; ++i)
    {
        float offset = i * 2.0f * 3.1415926f / 6.0f;
        auto part = manager->CreateObject<BossRotatingPart>(this->GetLocation());
        part->SetUp(this, 300.0f, offset);
        part->SetGroupID(i % 2);

        // 青と赤で交互に設定
        if (i % 2 == 0)
            part->SetPartType(BossRotatingPart::PartType::Red);
        else
            part->SetPartType(BossRotatingPart::PartType::Blue);

        rotating_parts.push_back(part);
    }


    for (auto& part : rotating_parts)
    {
        if (part->GetGroupID() == active_group_id)
            part->SetAttackDirection(BossRotatingPart::AttackDirectionType::Inward);
        else
            part->SetAttackDirection(BossRotatingPart::AttackDirectionType::Omnidirectional);
    }
}

void Stage2Boss::Update(float delta_second)
{
    life_timer += delta_second;

    if (!is_transformed && hp < 5000)
    {
        is_transforming = true;
        is_transformed = true;
        transform_timer = 0.0f;
        flash_timer = 0.3f;
        visible = true;
        is_flashing = true;
        is_screen_flash = true;
        screen_flash_timer = screen_flash_duration;
    }

    if (is_transforming)
    {
        transform_timer += delta_second;
        flash_timer += delta_second;

        if (flash_timer >= flash_interval)
        {
            flash_timer = 0.0f;
            visible = !visible;
        }

        velocity = { 0, 0 };

        if (transform_timer >= transform_duration)
        {
            // 変身完了！
            is_transforming = false;
            visible = true;
            images = images_a;
            image = images[0];

            // ★逆回転処理：1回だけ全パーツに指示
            for (auto& part : rotating_parts)
            {
                part->SetRotationDirection(-1.0f);
            }

            return; // ← ここで1回だけ抜ける
        }

        return; // ← まだ変身中の間はここで抜ける
    }


    if (is_flashing)
    {
        flash_timer -= delta_second;
        if (flash_timer <= 0.0f)
        {
            is_flashing = false;
            flash_timer = 0.0f;
        }
    }

    if (is_screen_flash)
    {
        screen_flash_timer -= delta_second;
        if (screen_flash_timer <= 0.0f)
        {
            is_screen_flash = false;
        }
    }

    if (hp <= 0)
    {
        is_alive = false;
        is_destroy = true;

        auto* manager = Singleton<AnimationManager>::GetInstance();
        int anim_id = manager->PlayerAnimation(EffectName::eExprotion, location, 0.05f, false);
        manager->SetScale(anim_id, 1.0f);
    }

    // アニメーション処理（Stage1Boss と同様に image を更新）
    static float anim_timer = 0.0f;
    static int anim_index = 0;

    if (!anim_indices.empty() && !images.empty())
    {
        anim_timer += delta_second;
        if (anim_timer >= 0.1f)  // アニメ速度調整
        {
            anim_timer = 0.0f;
            anim_index = (anim_index + 1) % anim_indices.size();
            int idx = anim_indices[anim_index];
            if (idx >= 0 && idx < (int)images.size())
            {
                image = images[idx];
            }
        }
    }


    part_attack_timer += delta_second;
    if (part_attack_timer >= 4.0f)
    {
        part_attack_timer = 0.0f;
        active_group_id = 1 - active_group_id;

        // Inward/Outwardは常に交互に撃たせる（基本ビーム）
        if (main_attack_type == BossRotatingPart::AttackDirectionType::Inward)
            main_attack_type = BossRotatingPart::AttackDirectionType::Outward;
        else
            main_attack_type = BossRotatingPart::AttackDirectionType::Inward;

        for (auto& part : rotating_parts)
        {
            // 常時ビーム（基本攻撃）
            if (part->GetGroupID() == active_group_id)
            {
                part->SetAttackDirection(main_attack_type);
            }
            else
            {
                // ▼ HPに応じて追加攻撃を設定（ビーム以外）
                if (hp < 1500)
                {
                    part->SetAttackDirection(BossRotatingPart::AttackDirectionType::HomingBurst);
                }
                else if (hp < 3000)
                {
                    part->SetAttackDirection(BossRotatingPart::AttackDirectionType::RainShot);
                }
                else if (hp < 5000)
                {
                    part->SetAttackDirection(BossRotatingPart::AttackDirectionType::Spiral);
                }
                else
                {
                    part->SetAttackDirection(BossRotatingPart::AttackDirectionType::Omnidirectional);
                }
            }
        }
    }


   /* Shot(delta_second);*/
    __super::Update(delta_second);
}

void Stage2Boss::Draw(const Vector2D& screen_offset) const
{
    if (is_transforming)
    {
        if (visible)
        {
            SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
            DrawRotaGraph(location.x, location.y, 4.0f, 0.0f, image, TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
    else
    {
        DrawRotaGraph(location.x, location.y, 4.0f, 0.0f, image, TRUE);
    }

    DrawFormatString(static_cast<int>(location.x - 30), static_cast<int>(location.y - 60), GetColor(255, 0, 0), "HP: %d", hp);
}

void Stage2Boss::Finalize()
{}

void Stage2Boss::TakeDamage(int amount)
{
    hp -= amount;
    if (hp <= 0 && is_alive)
    {
        is_alive = false;
        is_destroy = true;

        auto* manager = Singleton<AnimationManager>::GetInstance();
        int anim_id = manager->PlayerAnimation(EffectName::eExprotion, location, 0.05f, false);
        manager->SetScale(anim_id, 1.0f);
    }
}

void Stage2Boss::Shot(float delta_second)
{
    if (!player) return;

    shot_timer += delta_second;

    if (shot_timer >= 1.0f)
    {
        Vector2D dir = player->GetLocation() - location;
        float len = dir.Length();
        if (len > 0) dir /= len;

        auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot2>(location);
        shot->SetVelocity(dir);

        shot_timer = 0.0f;
    }
}
