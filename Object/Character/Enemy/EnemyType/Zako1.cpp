#include "Zako1.h"
#include "../../Player/Player.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include<vector>

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
    hp = 10;

    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;

    start_location = location;
    is_returning = false;
    spawn_delay_timer = 1.0f; // 1秒後に動き出す（例）



    ChangePatternRandomly();
}

void Zako1::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

    switch (pattern)
    {
        case Zako1Pattern::MoveStraight:
            velocity = { 0, 120 };
            break;

        case Zako1Pattern::MoveZigzag:
            velocity.x = sinf(pattern_timer * 4.0f) * 80;
            velocity.y = 100;
            Shot(delta_second);
            break;

        case Zako1Pattern::FollowPlayer:
            if (player)
            {
                Vector2D dir = player->GetLocation() - location;
                float distance = dir.Length();

                if (distance > 10.0f)
                {
                    dir.Normalize();
                    velocity = dir * 1.0f;
                }
                else
                {
                    velocity = { 0, 0 };
                }
            }
            break;

        case Zako1Pattern::BackAndForth:
        {
            float offset = location.y - start_location.y;
            if (!is_returning && offset > 150.0f)
                is_returning = true;
            else if (is_returning && offset < 0.0f)
                is_returning = false;

            velocity.y = is_returning ? -80 : 80;
            velocity.x = 0;
            break;
        }

        case Zako1Pattern::LateralSweep:
            velocity.x = sinf(pattern_timer * 2.0f) * 100;
            velocity.y = 0;
            break;

        case Zako1Pattern::DiagonalMove:
        {
            if (velocity.Length() == 0)
            {
                // 初期方向設定（右下へ移動）
                velocity = { 100, 100 };
            }

            location += velocity * delta_second;

            // 一定のY座標を下回ったら跳ね返る（上に戻る）
            if (location.y > 400)  // ←しきい値（任意）
            {
                velocity.y *= -1;
            }

            // 上に行きすぎたらまた戻る（オプション）
            else if (location.y < 100)
            {
                velocity.y *= -1;
            }

            break;
        }


        case Zako1Pattern::Hovering:
            velocity.x = 0;
            velocity.y = sinf(pattern_timer * 4.0f) * 20;
            break;

        case Zako1Pattern::Kamikaze:
            if (player)
            {
                Vector2D dir = player->GetLocation() - location;
                dir.Normalize();
                velocity = dir * 5.0;
            }
            break;
    }

    location += velocity * delta_second;



    if (hp <= 0)
    {
        is_destroy = true;
        ScoreData* score = Singleton<ScoreData>::GetInstance();
        score->SetScoreData(100);
    }

    __super::Update(delta_second);
}

void Zako1::Draw(const Vector2D& screen_offset) const
{
    DrawBox(location.x - box_size.x, location.y - box_size.y,
            location.x + box_size.x, location.y + box_size.y, GetColor(0, 0, 255), TRUE);
    DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
}

void Zako1::Finalize()
{}

//エネミーショット
void Zako1::Shot(float delta_second)
{

    shot_timer += delta_second;

    if (shot_timer >= 2.0f)
    {
        if (player)
        {
            Vector2D dir = player->GetLocation() - location;
            float len = dir.Length();
            if (len > 0) dir /= len;

            //ここで生成
            auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot2>(location);
            shot->SetVelocity(dir); // 追尾型発射
        }

        shot_timer = 0.0f;
    }
}



void Zako1::ChangePatternRandomly()
{
    int r = rand() % 8; // 0～7（Kamikaze追加）

    switch (r)
    {
        case 0: pattern = Zako1Pattern::MoveStraight; break;
        case 1: pattern = Zako1Pattern::MoveZigzag; break;
        case 2: pattern = Zako1Pattern::FollowPlayer; break;
        case 3: pattern = Zako1Pattern::BackAndForth; break;
        case 4: pattern = Zako1Pattern::LateralSweep; break;
        case 5: pattern = Zako1Pattern::DiagonalMove; break;
        case 6: pattern = Zako1Pattern::Hovering; break;
        case 7: pattern = Zako1Pattern::Kamikaze; break;
    }

    pattern_timer = 0.0f;
}

void Zako1::OnHitCollision(GameObjectBase* hit_object)
{
    if (hit_object->GetCollision().object_type == eObjectType::ePlayer|| hit_object->GetCollision().object_type == eObjectType::eShot)
    {
        SetDestroy();
    }
}

void Zako1::SetPattern(Zako1Pattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f; // 任意で初期化
}
