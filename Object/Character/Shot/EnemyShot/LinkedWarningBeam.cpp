#include "LinkedWarningBeam.h"
#include "LinkedBeam.h"
#include "../../../../Object/GameObjectManager.h"
#include "DxLib.h"

void LinkedWarningBeam::Initialize()
{
    z_layer = 2;
    is_destroy = false;
}

void LinkedWarningBeam::SetEndpoints(GameObjectBase* a, GameObjectBase* b)
{
    part_a = a;
    part_b = b;
}

void LinkedWarningBeam::SetLifeTime(float time)
{
    warning_time = time;
}

void LinkedWarningBeam::Update(float delta_second)
{
    elapsed_time += delta_second;
    if (elapsed_time >= warning_time)
    {
        if (part_a && part_b)
        {
            auto beam = Singleton<GameObjectManager>::GetInstance()->CreateObject<LinkedBeam>(part_a->GetLocation());
            beam->SetEndpoints(part_a, part_b);
            beam->SetThickness(10.0f);
            beam->SetLifeTime(1.5f);
        }
        is_destroy = true;
    }

    __super::Update(delta_second);
}

void LinkedWarningBeam::Draw(const Vector2D& screen_offset) const
{
    if (part_a && part_b)
    {
        Vector2D start = part_a->GetLocation() + screen_offset;
        Vector2D end = part_b->GetLocation() + screen_offset;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawLine(start.x, start.y, end.x, end.y, GetColor(255, 0, 0), 2); // ê‘Ç¢ç◊ê¸
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}
