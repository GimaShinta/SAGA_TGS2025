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

    // ��ʊO�폜�����i���Ŏ��s�j
    GameObjectBase::Update(delta);
}

void ItemExp::Draw(const Vector2D& offset) const
{
    Vector2D pos = this->location + offset;
    DrawCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), 5, GetColor(255, 255, 0), TRUE); // ���F�̉~
}

void ItemExp::OnHitCollision(GameObjectBase* hit_object)
{
    // �v���C���[�ƐڐG�����玩�g���폜
    if (dynamic_cast<Player*>(hit_object))
    {
        this->SetDestroy();
        // �����Ōo���l���Z�Ȃǂ��Ăяo���Ă�OK
    }
}
