#include "Zako1.h"
#include "../../Player/Player.h"
#include "../../../../Utility/AnimationManager.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

Zako1::Zako1()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

Zako1::~Zako1()
{}

void Zako1::Initialize()
{
    enemy_type = ENE_ZAKO1;
    z_layer = 2;
    box_size = 12;
    hp = 30;

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

void Zako1::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

    GameObjectBase::AnimationControl(delta_second, images, anim_indices, 10.0f);

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
                    velocity = { 0, -150 };
            
                }
            }
            break;
        case Zako1Pattern::MoveThenDiagonal:
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

    location += velocity * delta_second;

    if (hp <= 0)
    {
        is_destroy = true;
        Singleton<ScoreData>::GetInstance()->SetScoreData(100);
    }

    __super::Update(delta_second);
}

void Zako1::Draw(const Vector2D& screen_offset) const
{
    DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
    DrawRotaGraph(location.x, location.y, 1.5f, 0.0f, image, TRUE);
}

void Zako1::Finalize()
{}

void Zako1::Shot(float delta_second)
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

void Zako1::ChangePatternRandomly()
{
    int r = rand() % 6;
    SetPattern(static_cast<Zako1Pattern>(r));
}

void Zako1::SetPattern(Zako1Pattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f;
    has_shot = false;
    after_shot_timer = 0.0f;

    switch (pattern)
    {
        case Zako1Pattern::MoveStraight:
        case Zako1Pattern::RightMove:
        case Zako1Pattern::LeftMove:
        case Zako1Pattern::ZIgzag:
        case Zako1Pattern::MoveThenDiagonal:
            images = images_b;
            anim_indices = { 0, 1, 2, 3, 2, 1 };
            break;

        case Zako1Pattern::MoveAndStopShoot:
            images = images_a;
            anim_indices = { 0, 1, 2, 3 };
            break;
    }

    image = images[0];
}
