#pragma once
#include "../../../../Object/GameObjectBase.h"

class LinkedBeam : public GameObjectBase
{
public:
    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;

    void SetEndpoints(GameObjectBase* a, GameObjectBase* b);
    void SetThickness(float thickness);
    void SetLifeTime(float time);

private:
    GameObjectBase* part_a = nullptr;
    GameObjectBase* part_b = nullptr;

    float beam_thickness = 6.0f;
    float life_time = 1.5f;
    float elapsed_time = 0.0f;
};
