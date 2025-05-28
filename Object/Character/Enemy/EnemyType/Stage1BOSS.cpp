#include "Stage1BOSS.h"
#include "../../Player/Player.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../../../Utility/ScoreData.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

// コンストラクタ（ランダム初期化）
Stage1Boss::Stage1Boss()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

// デストラクタ
Stage1Boss::~Stage1Boss()
{}

void Stage1Boss::Initialize()
{
    enemy_type = ENE_ZAKO1;
    z_layer = 2;
    box_size = 12;
    hp = 2000;

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

    floating_center_initialized = false; // 初期化フラグOFF
}

void Stage1Boss::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

    GameObjectBase::AnimationControl(delta_second, images, anim_indices, 10.0f);

    switch (pattern)
    {
        case BossPattern::Entrance:
        {
            const float move_down_duration = 3.0f;   // 降りてくる時間（秒）
            const float target_y = 150.0f;            // 降り終わるY位置
            const float amplitude_x = 250.0f;         // 横振り幅
            const float frequency_x = 1.0f;           // 横振り速さ
            const float amplitude_y = 50.0f;          // 縦ゆらぎ幅
            const float frequency_y = 2.0f;           // 縦ゆらぎ速さ

            if (pattern_timer < move_down_duration && location.y < target_y)
            {
                // 下に降りるフェーズ
                velocity = { 0, 100 };
                location += velocity * delta_second;

                // 降りすぎ防止
                if (location.y > target_y)
                {
                    location.y = target_y;
                }
            }
            else
            {
                // 浮遊フェーズ

                // 浮遊基準座標を一度だけ初期化
                if (!floating_center_initialized)
                {
                    floating_center_x = location.x; // 降り終わり時のX座標を基準にする
                    floating_center_y = target_y;   // 固定のY座標を基準にする（必要なら location.y でも可）
                    floating_center_initialized = true;
                }

                float t = pattern_timer - move_down_duration;

                // 横はサイン波で左右にゆらゆら（左スタート）
                location.x = floating_center_x + amplitude_x * sinf(t * frequency_x + 3.14f);

                // 縦はゆらゆら上下に浮遊
                location.y = floating_center_y + amplitude_y * sinf(t * frequency_y);

                velocity = { 0, 0 };
            }

            break;
        }
    }

    location += velocity * delta_second;

    if (hp <= 0)
    { 
        is_alive = false;
        is_destroy = true;
      

        AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
        anim_id = manager->PlayerAnimation(EffectName::eExprotion, location, 0.05f, false);
        manager->SetScale(anim_id, 0.5f);

        Singleton<ScoreData>::GetInstance()->SetScoreData(100);
    }

    __super::Update(delta_second);


}

void Stage1Boss::Draw(const Vector2D& screen_offset) const
{
    DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
    DrawRotaGraph(location.x, location.y, 5.0f, 0.0f, image, TRUE);
    DrawFormatString(location.x - 10, location.y - 40, GetColor(255, 255, 255), "%.0f", hp);
}

void Stage1Boss::Finalize()
{}

void Stage1Boss::Shot(float delta_second)
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

void Stage1Boss::ChangePatternRandomly()
{
}

bool Stage1Boss::GetIsAlive() const
{
    return is_alive;
}

void Stage1Boss::SetPattern(BossPattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f;
    has_shot = false;
    after_shot_timer = 0.0f;
    floating_center_initialized = false; // パターン切替時に基準リセット

    switch (pattern)
    {
        case BossPattern::Entrance:
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            break;
    }

    image = images[0];
}

