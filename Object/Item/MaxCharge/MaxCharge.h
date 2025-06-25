#pragma once
#include "../ItemBase.h"
class MaxCharge :
    public ItemBase
{
private:
    std::vector<int> images;

public:
    void Initialize() override;
    void Update(float delta) override;
    void Draw(const Vector2D& offset) const override;
    void OnHitCollision(GameObjectBase* hit_object) override;
};

