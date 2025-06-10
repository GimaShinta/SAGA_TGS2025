#include "Shield.h"

void Shield::Initialize()
{
    velocity.y = 10.0f;
    box_size = 10.0f;
    z_layer = 1;
    is_mobility = true;

    // 当たり判定のオブジェクト設定
    collision.is_blocking = true;
    // 自分のオブジェクトタイプ
    collision.object_type = eObjectType::eShield;
    // 当たる相手のオブジェクトタイプ
    collision.hit_object_type.push_back(eObjectType::ePlayer);
}

void Shield::Update(float delta)
{
    location += velocity * delta;
}

void Shield::Draw(const Vector2D& offset) const
{
    DrawBox(location.x - box_size.x, location.y - box_size.y,
        location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);
}

void Shield::OnHitCollision(GameObjectBase* hit_object)
{
    if (hit_object->GetCollision().object_type == eObjectType::ePlayer)
    {
        this->SetDestroy();
    }
}
