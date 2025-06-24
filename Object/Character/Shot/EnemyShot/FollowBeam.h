#pragma once
#include "../../../../Object/GameObjectBase.h"

class FollowBeam : public GameObjectBase
{
public:
    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void OnHitCollision(GameObjectBase* hit_object) override; // Åö í«â¡

    void SetFollowTarget(GameObjectBase* target, const Vector2D& direction);
    void SetBeamTarget(Vector2D target_position);

    bool GetIsDestroy() const
    {
        return is_destroy;
    }

private:
    GameObjectBase* follow_target = nullptr;
    Vector2D dir;
    Vector2D target_pos;
    float length = 600.0f;
    float life_time = 1.0f;
    float elapsed_time = 0.0f;
};
