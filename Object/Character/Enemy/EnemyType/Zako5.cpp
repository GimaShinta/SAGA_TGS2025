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
    hp = 2000;

    phase = Zako5Phase::Appearing;
    appear_timer = 0.0f;
    is_invincible = true;
    scale = 4.0f;
    alpha = 0;
    velocity = Vector2D(0, 0);

    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);
    is_mobility = true;

    auto* rm = Singleton<ResourceManager>::GetInstance();
    image = rm->GetImages("Resource/Image/Object/Enemy/Zako5/enemy53.png")[0];
}

void Zako5::Update(float delta)
{
    switch (phase)
    {
        case Zako5Phase::Appearing:
        {
            appear_timer += delta;

            if (velocity.x == 0.0f && velocity.y == 0.0f)
            {
                velocity = (location.x < 640.0f)
                    ? Vector2D(0.0f, 150.0f)   // 左から登場 → 下へ
                    : Vector2D(0.0f, -150.0f); // 右から登場 → 上へ
            }

            float t = (appear_timer < 1.0f) ? appear_timer : 1.0f;
            scale = 4.0f - (2.5f * t);
            location += velocity * delta;

            if (appear_timer >= 2.0f)
            {
                phase = Zako5Phase::Fighting;
                is_invincible = false;
                scale = 1.5f;

                if (is_rectangular_loop_move && is_clockwise)
                {
                    if (velocity.y > 0.0f) // 下へ
                    {
                        velocity = Vector2D(250.0f, 0.0f);
                        move_state = MoveState::MoveRight;
                    }
                    else // 上へ
                    {
                        velocity = Vector2D(0.0f, -150.0f);
                        move_state = MoveState::MoveUp;
                    }
                }
                else if (is_rectangular_loop_move && !is_clockwise)
                {
                    if (velocity.y > 0.0f) // 左から来た → 下から開始
                    {
                        velocity = Vector2D(250.0f, 0.0f);  // →
                        move_state = MoveState::MoveRight;
                    }
                    else // 右から来た → 上から開始
                    {
                        velocity = Vector2D(-250.0f, 0.0f); // ←
                        move_state = MoveState::MoveLeft;
                    }
                }
            }
            break;
        }

        case Zako5Phase::Fighting:
            if (is_rectangular_loop_move)
            {
                if (is_clockwise)
                {
                    // 時計回り（↑ ← ↓ →）
                    switch (move_state)
                    {
                        case MoveState::MoveUp:
                            location.y += velocity.y * delta;
                            if (location.y <= LOOP_TOP)
                            {
                                location.y = LOOP_TOP;
                                velocity = Vector2D(-250.0f, 0.0f);
                                move_state = MoveState::MoveLeft;
                            }
                            break;
                        case MoveState::MoveLeft:
                            location.x += velocity.x * delta;
                            if (location.x <= LOOP_LEFT)
                            {
                                location.x = LOOP_LEFT;
                                velocity = Vector2D(0.0f, 150.0f);
                                move_state = MoveState::MoveDown;
                            }
                            break;
                        case MoveState::MoveDown:
                            location.y += velocity.y * delta;
                            if (location.y >= LOOP_BOTTOM)
                            {
                                location.y = LOOP_BOTTOM;
                                velocity = Vector2D(250.0f, 0.0f);
                                move_state = MoveState::MoveRight;
                            }
                            break;
                        case MoveState::MoveRight:
                            location.x += velocity.x * delta;
                            if (location.x >= LOOP_RIGHT)
                            {
                                location.x = LOOP_RIGHT;
                                velocity = Vector2D(0.0f, -150.0f);
                                move_state = MoveState::MoveUp;
                            }
                            break;
                    }
                }
                else
                {
                    // 反時計回り（← ↓ → ↑）→ 見た目は対称
                    switch (move_state)
                    {
                        case MoveState::MoveLeft:
                            location.x += velocity.x * delta;
                            if (location.x <= LOOP_LEFT)
                            {
                                location.x = LOOP_LEFT;
                                velocity = Vector2D(0.0f, 150.0f);  // ↓
                                move_state = MoveState::MoveDown;
                            }
                            break;
                        case MoveState::MoveDown:
                            location.y += velocity.y * delta;
                            if (location.y >= LOOP_BOTTOM)
                            {
                                location.y = LOOP_BOTTOM;
                                velocity = Vector2D(250.0f, 0.0f);  // →
                                move_state = MoveState::MoveRight;
                            }
                            break;
                        case MoveState::MoveRight:
                            location.x += velocity.x * delta;
                            if (location.x >= LOOP_RIGHT)
                            {
                                location.x = LOOP_RIGHT;
                                velocity = Vector2D(0.0f, -150.0f);  // ↑
                                move_state = MoveState::MoveUp;
                            }
                            break;
                        case MoveState::MoveUp:
                            location.y += velocity.y * delta;
                            if (location.y <= LOOP_TOP)
                            {
                                location.y = LOOP_TOP;
                                velocity = Vector2D(-250.0f, 0.0f);  // ←
                                move_state = MoveState::MoveLeft;
                            }
                            break;
                    }
                }
            }

            Shot(delta);

            if (hp <= 0)
            {
                is_destroy = true;
                auto* manager = Singleton<AnimationManager>::GetInstance();
                anim_id = manager->PlayerAnimation(EffectName::eExprotion2, location, 0.035f, false);
                manager->SetScale(anim_id, 1.5f);
            }
            break;
    }

    SetLocation(location);
    __super::Update(delta);
}




void Zako5::EnableLoopMoveNow()
{
    loop_move_enabled = true;
}


void Zako5::Shot(float delta)
{
    if (!is_attacking)
    {
        attack_cooldown -= delta;
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
        if (attack_pattern == 6) Pattern6(delta);
        else if (attack_pattern == 7) Pattern7(delta);

        if (spiral_total_time >= 2.5f)
        {
            is_attacking = false;
            attack_pattern = (attack_pattern == 6) ? 7 : 6;
        }
    }
}

void Zako5::Pattern6(float delta)
{
    spiral_timer += delta;
    spiral_total_time += delta;

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

void Zako5::Pattern7(float delta)
{
    spiral_timer += delta;
    spiral_total_time += delta;

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
}

void Zako5::Finalize()
{}

void Zako5::EnableCircularMove(const Vector2D& center, float radius, float speed, float start_angle)
{
    circular_center = center;
    circular_radius = radius;
    circular_speed = speed;
    circular_angle = start_angle;
    is_circular_move = true;
}

void Zako5::EnableRectangularLoopMove(bool clockwise)
{
    is_rectangular_loop_move = true;
    is_clockwise = clockwise;
}
