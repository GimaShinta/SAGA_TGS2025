#include "Zako3.h"
#include "../../../../Utility/ProjectConfig.h"
#include <cmath>

Zako3::Zako3()
{
}

Zako3::~Zako3()
{
}

void Zako3::Initialize()
{
    enemy_type = ENE_ZAKO3;
    z_layer = 2;
    box_size = 12;
    hp = 30;

    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    image = rm->GetImages("Resource/Image/Object/Enemy/Zako3/anime_enemy41.png", 4, 4, 1, 104, 80)[0];

    // èâä˙èÛë‘ÅiSetAppearParamsÇ≈è„èëÇ´Ç≥ÇÍÇÈÅj
    is_appearing = true;
    scale = 0.5f;
    alpha = 0;
    rotation = 0.0f;
    state = Zako3State::Appearing;

    appear_timer = 0.0f;
    appear_duration = 3.0f;
}

void Zako3::Update(float delta_second)
{
    if (is_appearing)
    {
        appear_timer += delta_second;
        float t = my_min(appear_timer / appear_duration, 1.0f);
        float ease_t = t * t * (3 - 2 * t); // easeInOutï‚ä‘

        scale = 5.0f - 4.2f * ease_t; // 5.0 Å® 1.5 Ç…ääÇÁÇ©Ç…èkè¨
        alpha = static_cast<int>(255 * ease_t);
        rotation += delta_second * 2.0f * (is_from_left ? +1.0f : -1.0f);
    }
    switch (state)
    {
    case Zako3State::Appearing:
    {
        appear_timer += delta_second;
        float t = my_min(appear_timer / appear_duration, 1.0f);
        float ease_t = 1 - powf(1 - t, 3); // easeOutCubic

        location = appear_start_pos + (appear_end_pos - appear_start_pos) * ease_t;
        float height_offset = +150.0f * sinf(t * DX_PI);
        location.y += height_offset;

        alpha = static_cast<int>(255 * ease_t);
        rotation += delta_second * (1.0f - t) * (is_from_left ? +1.0f : -1.0f);

        if (t >= 1.0f)
        {
            state = Zako3State::Floating;
            base_location = location;
            float_timer = 0.0f;
            scale = 0.8f;
            alpha = 255;
            rotation = 0.0f;
            is_appearing = false;
        }
        break;
    }
    case Zako3State::Floating:
        float_timer += delta_second;
        location.x = base_location.x + sinf(float_timer * 1.5f) * 10.0f;
        location.y = base_location.y + sinf(float_timer * 2.0f) * 5.0f;
        break;
    }

    if (hp <= 0)
        is_destroy = true;

    __super::Update(delta_second);
}

void Zako3::Draw(const Vector2D& screen_offset) const
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    DrawRotaGraph(
        location.x, location.y,
        scale, 0.0,
        image, TRUE, zako3_flip);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawBox(location.x - box_size.x, location.y - box_size.y,
        location.x + box_size.x, location.y + box_size.y,
        GetColor(0, 255, 0), TRUE);

}

void Zako3::Finalize()
{
}

void Zako3::Shot(float delta_second)
{
}

void Zako3::SetFlip(bool flip)
{
    zako3_flip = flip;
}

void Zako3::SetAppearParams(const Vector2D& start_pos, const Vector2D& end_pos, float appear_time, bool from_left)
{
    appear_start_pos = start_pos;
    appear_end_pos = end_pos;
    location = start_pos;

    appear_duration = appear_time;
    appear_timer = 0.0f;
    is_appearing = true;
    scale = 3.0f;
    alpha = 0;
    rotation = 0.0f;
    state = Zako3State::Appearing;

    is_from_left = from_left;
}
