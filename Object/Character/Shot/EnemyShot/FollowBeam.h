#pragma once
#include "../../../../Object/GameObjectBase.h"

class FollowBeam : public GameObjectBase
{
public:
    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void OnHitCollision(GameObjectBase* hit_object) override;

    void SetFollowTarget(GameObjectBase* target, const Vector2D& direction);
    void SetBeamTarget(Vector2D target_position);

    bool GetIsDestroy() const
    {
        return is_destroy;
    }

    void SetFollowAndLookTarget(GameObjectBase* follow, GameObjectBase* look, float max_length)
    {
        follow_target = follow;
        look_target = look;
        length = max_length;
    }

    // ★ 太さを設定する関数を追加
    void SetBeamThickness(float thickness)
    {
        beam_thickness = thickness;
    }

private:
    GameObjectBase* follow_target = nullptr;
    GameObjectBase* look_target = nullptr;
    Vector2D dir;
    Vector2D target_pos;
    float length = 600.0f;
    float life_time = 1.0f;
    float elapsed_time = 0.0f;

    float beam_thickness = 3.0f;  // ★ デフォルト太さ
};
