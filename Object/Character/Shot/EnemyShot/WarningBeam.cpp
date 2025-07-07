#include "WarningBeam.h"
#include "DxLib.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Shot/EnemyShot/FollowBeam.h"

void WarningBeam::Initialize()
{
    z_layer = 2;
    is_destroy = false;
}

void WarningBeam::SetUp(const Vector2D& direction, float length, float lifetime)
{
    dir = direction.Normalize();
    beam_length = length;
    life_time = lifetime;
}

void WarningBeam::Update(float delta_second)
{
    if (follow_target)
    {
        location = follow_target->GetLocation();  // 根本を追従
    }

    if (look_target)
    {
        Vector2D to = look_target->GetLocation() - location;
        float len = to.Length();
        if (len > 0.01f)
        {
            dir = to / len;
            beam_length = len;
        }
    }

    elapsed_time += delta_second;
    if (elapsed_time >= life_time)
    {
        // 発射
        auto beam = Singleton<GameObjectManager>::GetInstance()->CreateObject<FollowBeam>(location);
        beam->SetFollowAndLookTarget(follow_target, look_target, beam_length);
        beam->SetFollowTarget(follow_target, dir);  // ★ 方向を追加設定
        beam->SetBeamThickness(20.0f);  // ★ 太さを変更（例：6ピクセル）

        SetDestroy();
    }

    __super::Update(delta_second);
}

void WarningBeam::Draw(const Vector2D& screen_offset) const
{
    Vector2D start = location + screen_offset;
    Vector2D end = start + dir * beam_length;

    SetDrawBlendMode(DX_BLENDMODE_ADD, 128);
    DrawLine(start.x, start.y, end.x, end.y, GetColor(255, 0, 0), 2); // 赤い予兆線
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
