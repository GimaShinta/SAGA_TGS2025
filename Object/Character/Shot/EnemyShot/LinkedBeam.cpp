#include "LinkedBeam.h"
#include "DxLib.h"

void LinkedBeam::Initialize()
{
    z_layer = 2;
    is_destroy = false;
}

void LinkedBeam::SetEndpoints(GameObjectBase* a, GameObjectBase* b)
{
    part_a = a;
    part_b = b;
}

void LinkedBeam::SetThickness(float thickness)
{
    beam_thickness = thickness;
}

void LinkedBeam::SetLifeTime(float time)
{
    life_time = time;
}

void LinkedBeam::Update(float delta_second)
{
    elapsed_time += delta_second;
    if (elapsed_time >= life_time)
    {
        is_destroy = true;
    }
    __super::Update(delta_second);
}

void LinkedBeam::Draw(const Vector2D& screen_offset) const
{
    if (part_a && part_b)
    {
        Vector2D start = part_a->GetLocation() + screen_offset;
        Vector2D end = part_b->GetLocation() + screen_offset;

        SetDrawBlendMode(DX_BLENDMODE_ADD, 180);
        DrawLine(start.x, start.y, end.x, end.y, GetColor(255, 255, 0), static_cast<int>(beam_thickness));
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}
