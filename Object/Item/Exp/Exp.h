#pragma once
#include "../ItemBase.h"
#include "../../../Object/Character/Player/Player.h"

class ItemExp : public ItemBase
{
private:
    Player* player;
    float speed = 150.0f;          // ’Ç”ö‘¬“x
    float attract_range = 150.0f;  // ‹zŽû‚ðŽn‚ß‚é‹——£
    bool is_following = false;

public:
    void Initialize() override;
    void Update(float delta) override;
    void Draw(const Vector2D& offset) const override;
    void OnHitCollision(GameObjectBase* hit_object) override;

    void SetPlayer(Player* p) { player = p; }
};
