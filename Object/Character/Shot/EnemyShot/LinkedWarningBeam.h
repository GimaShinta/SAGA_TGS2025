#pragma once
#include "../../../../Object/GameObjectBase.h"

class LinkedWarningBeam : public GameObjectBase
{
public:
    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;

    void SetEndpoints(GameObjectBase* a, GameObjectBase* b);
    void SetLifeTime(float time);

private:
    GameObjectBase* part_a = nullptr;
    GameObjectBase* part_b = nullptr;

    float elapsed_time = 0.0f;
    float warning_time = 1.0f;
};
