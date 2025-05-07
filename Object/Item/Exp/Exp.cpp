// ItemExp.cpp
#include "Exp.h"
#include "DxLib.h"
#include <cmath>

void ItemExp::Initialize()
{
    this->box_size = Vector2D(10.0f, 10.0f);
    this->z_layer = 1;
    this->is_mobility = true;
}

void ItemExp::Update(float delta)
{
    if (!player) return;

    Vector2D to_player = player->GetLocation() - this->location;
    float dist = std::sqrt(to_player.x * to_player.x + to_player.y * to_player.y);

    if (dist < attract_range) is_following = true;

    if (is_following && dist > 0)
    {
        Vector2D dir(to_player.x / dist, to_player.y / dist);
        this->location.x += dir.x * speed * delta;
        this->location.y += dir.y * speed * delta;
    }

    // 画面外削除処理（基底で実行）
    GameObjectBase::Update(delta);
}

void ItemExp::Draw(const Vector2D& offset) const
{
    Vector2D pos = this->location + offset;
    DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), 5, GetColor(255, 255, 0), TRUE); // 黄色の円
}

void ItemExp::OnHitCollision(GameObjectBase* hit_object)
{
    // プレイヤーと接触したら自身を削除
    if (dynamic_cast<Player*>(hit_object))
    {
        this->SetDestroy();
        // ここで経験値加算などを呼び出してもOK
    }
}
