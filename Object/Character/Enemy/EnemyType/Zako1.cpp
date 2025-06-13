#include "Zako1.h"
#include "../../Player/Player.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../../../Utility/ScoreData.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

Zako::Zako()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

Zako::Zako(const Vector2D& pos)
{
    location = pos;
    srand(static_cast<unsigned int>(time(nullptr)));
}

Zako::~Zako()
{}

void Zako::Initialize()
{
    enemy_type = ENE_ZAKO1;
    z_layer = 2;
    box_size = 12;
    hp = 0;

    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;
    start_location = location;
    is_returning = false;
    has_shot = false;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images_a = rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_a.png", 4, 4, 1, 32, 32);
    images_b = rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_b.png", 12, 12, 1, 24, 24);

    images = images_a;
    image = images[0];

    ChangePatternRandomly();
}

void Zako::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

    GameObjectBase::AnimationControl(delta_second, images, anim_indices, 10.0f);

    switch (pattern)
    {
        case ZakoPattern::MoveStraight:
        {
            const float appear_time = 0.8f;  // ägëÂÇ…Ç©ÇØÇÈéûä‘

            if (pattern_timer < appear_time)
            {
                float t = pattern_timer / appear_time;
                scale = 0.5f + t * 1.5f;  // 0.2 Å® 1.5 Ç…ÉXÉPÅ[ÉãÉAÉbÉv
                velocity = { 0, 180 };     // â∫ï˚å¸Ç…ìÆÇ´ë±ÇØÇÈ
            }
            else
            {
                scale = 2.0f;
                velocity = { 0, 180 };     // í èÌÉXÉPÅ[ÉãÅïà⁄ìÆ
            }
            break;
        }


        case ZakoPattern::RightMove:
        {
            const float arc_duration = 1.2f;     // å Çï`Ç≠éûä‘
            const float decel_duration = 1.0f;   // å∏ë¨éûä‘
            const float accel_duration = 1.0f;   // çƒâ¡ë¨éûä‘

            if (pattern_timer < arc_duration)
            {
                // å Çï`Ç≠ìÆÇ´ÅiâEè„Å®íÜâõÇ÷ç~ÇËÇÈÅj
                float t = pattern_timer / arc_duration;
                float angle = (1.0f - t) * (3.1415f / 2.0f);  // ÉŒ/2 Å® 0

                velocity.x = cosf(angle) * 250.0f;
                velocity.y = sinf(angle) * 250.0f;

                scale = 4.0f - 2.0f * t;  // 4.0 Å® 2.0 Ç…èkè¨
            }
            else if (pattern_timer < arc_duration + decel_duration)
            {
                float t = (pattern_timer - arc_duration) / decel_duration;
                velocity.x = 250.0f * (1.0f - t);
                velocity.y = 0.0f;

                scale = 2.0f;
            }
            else if (pattern_timer < arc_duration + decel_duration + accel_duration)
            {
                float t = (pattern_timer - arc_duration - decel_duration) / accel_duration;
                velocity.x = 200.0f + 300.0f * t;
                velocity.y = 0.0f;

                scale = 2.0f;
            }
            else
            {
                velocity.x = 500.0f;
                velocity.y = 0.0f;

                scale = 2.0f;
            }

            break;
        }



        case ZakoPattern::LeftMove:
        {
            const float arc_duration = 1.2f;     // å Çï`Ç≠éûä‘
            const float decel_duration = 1.0f;   // å∏ë¨éûä‘
            const float accel_duration = 1.0f;   // çƒâ¡ë¨éûä‘

            if (pattern_timer < arc_duration)
            {
                // å Çï`Ç≠ìÆÇ´Åiç∂è„Å®íÜâõÇ÷ç~ÇËÇÈÅj
                float t = pattern_timer / arc_duration;
                float angle = (1.0f - t) * (3.1415f / 2.0f);  // ÉŒ/2 Å® 0

                velocity.x = -cosf(angle) * 250.0f;
                velocity.y = sinf(angle) * 250.0f;

                scale = 4.0f - 2.0f * t;  // 4.0 Å® 2.0 Ç…èkè¨
            }
            else if (pattern_timer < arc_duration + decel_duration)
            {
                float t = (pattern_timer - arc_duration) / decel_duration;
                velocity.x = -250.0f * (1.0f - t);
                velocity.y = 0.0f;

                scale = 2.0f;
            }
            else if (pattern_timer < arc_duration + decel_duration + accel_duration)
            {
                float t = (pattern_timer - arc_duration - decel_duration) / accel_duration;
                velocity.x = -200.0f - 300.0f * t;
                velocity.y = 0.0f;

                scale = 2.0f;
            }
            else
            {
                velocity.x = -500.0f;
                velocity.y = 0.0f;

                scale = 2.0f;
            }

            break;
        }





        case ZakoPattern::ZIgzag:
            velocity.x = sinf(pattern_timer * 1.5f) * 320;
            velocity.y = 100;
            Shot(delta_second);
            break;

        case ZakoPattern::SideAppearAndShoot:
        {
            const float appear_duration = 1.0f;
            const float stop_and_shoot_interval = 2.0f;

            if (pattern_timer < appear_duration)
            {
                if (location.x > 400) velocity = { -200, 0 };
                else velocity = { 200, 0 };
            }
            else
            {
                velocity = { 0, 0 };
                Shot(delta_second);
            }
            break;
        }

        case ZakoPattern::MoveAndStopShoot:
            if (!has_shot)
            {
                if (location.y < 300)
                {
                    scale = 1.5f;  // à⁄ìÆíÜÇÕëÂÇ´Ç≠
                    velocity = { 0, 300 };
                }
                else
                {
                    scale = 1.5f;  // í‚é~ÇµÇƒåÇÇ¬ëOÇ…èkè¨
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
                    velocity = { 0, -150 };
                }
            }
            break;


        case ZakoPattern::MoveThenDiagonal:
            if (pattern_timer < 1.0f)
            {
                velocity = { 0, 100 };
            }
            else
            {
                velocity = { 100, 100 };
            }
            break;

        case ZakoPattern::Formation:
            velocity.x = sinf((pattern_timer + start_location.y * 0.01f) * 1.5f) * 80.0f;
            velocity.y = 80.0f;
            break;

        case ZakoPattern::DiveOnce:
            if (pattern_timer < 1.0f)
            {
                scale = 1.5f;  // ìoèÍéûÇÕëÂÇ´Ç≠
                velocity = { 0, 300 };
            }
            else if (pattern_timer < 2.0f)
            {
                scale = 1.5f;  // àÍéûí‚é~éûÇ…èkè¨
                velocity = { 0, 0 };
            }
            else
            {
                if (!has_shot && player)
                {
                    Vector2D dir = player->GetLocation() - location;
                    float len = dir.Length();
                    if (len > 0) dir /= len;

                    velocity = dir * 600.0f;
                    has_shot = true;
                }
            }
            break;


        case ZakoPattern::ArcMoveAndStop:
        {
            const float arc_duration = 5.0f;
            if (pattern_timer < arc_duration)
            {
                float t = pattern_timer / arc_duration;
                float angle = t * 3.1415f;
                velocity.x = cosf(angle) * 150.0f;
                velocity.y = -sinf(angle) * 150.0f;
            }
            else
            {
                velocity = { 0, 0 };
            }
            break;
        }

        case ZakoPattern::DepthAppear:
        {
            const float appear_time = 1.0f;        // ìoèÍÉtÉFÅ[ÉhÉCÉìéûä‘
            const float wait_after_appear = 0.0f; // é~Ç‹ÇÈéûä‘

            if (pattern_timer < appear_time)
            {
                float t = pattern_timer / appear_time;
                scale = 0.2f + t * 1.3f;  // 0.2Å`1.0
                //location.y = 150 + (1.0f - t) * 100; // 150Ç…ãﬂÇ√ÇØÇÈ
                velocity = { 0, 0 };
            }
            else if (pattern_timer < appear_time + wait_after_appear)
            {
                scale = 1.5f;
                velocity = { 0, 0 };
                Shot(delta_second);
            }
            break;
        }
        case ZakoPattern::RetreatUp:
        {
            const float decel_duration = 0.5f;   // å∏ë¨éûä‘
            const float pause_duration = 0.3f;   // í‚ëÿéûä‘
            const float accel_duration = 0.7f;   // â¡ë¨éûä‘

            if (pattern_timer < decel_duration)
            {
                // å∏ë¨ÇµÇƒí‚é~Ç∑ÇÈÇÊÇ§Ç…Åiâ∫ï˚å¸ Å® 0Åj
                float t = pattern_timer / decel_duration;
                velocity.y = 200.0f * (1.0f - t); // 200 Å® 0
            }
            else if (pattern_timer < decel_duration + pause_duration)
            {
                // àÍéûí‚é~
                velocity.y = 0.0f;
            }
            else
            {
                // è„ï˚å¸Ç…â¡ë¨
                float t = (pattern_timer - decel_duration - pause_duration) / accel_duration;
                if (t > 1.0f) t = 1.0f;
                velocity.y = -100.0f - 400.0f * t;  // -100 Å® -500 Ç÷â¡ë¨
            }

            if (location.y < -100)
            {
                is_destroy = true;
            }

            break;
        }



    }

    location += velocity * delta_second;

    if (hp <= 0)
    {
        is_destroy = true;

        DropItems(); 

        AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
        anim_id = manager->PlayerAnimation(EffectName::eExprotion2, location, 0.035f, false);
        manager->SetScale(anim_id, 0.5f);

        Singleton<ScoreData>::GetInstance()->SetScoreData(100);
    }


    __super::Update(delta_second);
}

void Zako::Draw(const Vector2D& screen_offset) const
{
    DrawRotaGraph(location.x, location.y, scale, 0.0f, image, TRUE);
}

void Zako::Finalize()
{}

void Zako::Shot(float delta_second)
{
    shot_timer += delta_second;

    if (shot_timer >= 2.0f && player)
    {
        Vector2D dir = player->GetLocation() - location;
        float len = dir.Length();
        if (len > 0) dir /= len;

        auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot2>(location);
        shot->SetVelocity(dir);

        shot_timer = 0.0f;
    }
}

void Zako::ChangePatternRandomly()
{
    int r = rand() % static_cast<int>(ZakoPattern::Count);
    SetPattern(static_cast<ZakoPattern>(r));
}

void Zako::SetPattern(ZakoPattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f;
    has_shot = false;
    after_shot_timer = 0.0f;

    switch (pattern)
    {
        case ZakoPattern::MoveStraight:
            hp = 20;
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            scale = 1.5f;
            break;

        case ZakoPattern::RightMove:
        case ZakoPattern::LeftMove:
            hp = 30;
            images = images_a;
            anim_indices = { 0, 1, 2, 3 };
            scale = 1.5f;
            break;

        case ZakoPattern::ZIgzag:
        case ZakoPattern::MoveThenDiagonal:
            hp = 20;
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            scale = 1.5f;
            break;

        case ZakoPattern::MoveAndStopShoot:
        case ZakoPattern::Formation:
        case ZakoPattern::DiveOnce:
            hp = 30;
            images = images_a;
            anim_indices = { 0, 1, 2, 3 };
            scale = 1.5f;
            break;

        case ZakoPattern::SideAppearAndShoot:
        case ZakoPattern::ArcMoveAndStop:
            hp = 25;
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            scale = 1.5f;
            break;

        case ZakoPattern::DepthAppear:
            hp = 20;
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            scale = 0.2f;
            break;
        case ZakoPattern::RetreatUp:
            hp = 10;
            images = images_a;
            anim_indices = { 0, 1, 2, 3 };
            scale = 1.5f;
            break;

    }

    image = images[0];
}