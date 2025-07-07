#pragma once

#include "../../../../Object/GameObjectBase.h"
#include "../../../../Object/Character/Player/Player.h"

class Stage2Boss;

class BossRotatingPart : public GameObjectBase
{
public:
    BossRotatingPart() = default;
    ~BossRotatingPart() override = default;
    void SetPlayer(Player* p)
    {
        player = p;
    }

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void OnHitCollision(GameObjectBase* hit_object) override;

    void SetUp(Stage2Boss* boss, float radius, float angle_offset);
    void SetGroupID(int id)
    {
        group_id = id;
    }
    int GetGroupID() const
    {
        return group_id;
    }

    enum class AttackDirectionType
    {
        Inward,
        Outward,
        Omnidirectional,
        Triangle,       // ★ 三角形ビーム
        LinkedDown,     // ★ コアをつなぎ下に撃つ
        HomingCenter,   // ★ 中央追従
        Spiral,
        RainShot,
        HomingBurst
    };

    void SetAttackDirection(AttackDirectionType type)
    {
        attack_direction = type;
    }

    enum class PartType
    {
        Red,
        Blue
    };

    void SetPartType(PartType new_type)
    {
        type = new_type;
    }

    void SetRotationDirection(float dir)
    {
        rotation_direction = dir;
    }

private:
    Stage2Boss* boss = nullptr;
    float radius = 80.0f;
    float angle_offset = 0.0f;
    float angle = 0.0f;
    Player* player = nullptr;

    std::vector<int> images;
    int image_index = 0;
    float anim_timer = 0.0f;

    float shot_timer = 0.0f;
    bool use_beam = false;
    float rotation_direction = 1.0f;
    float appear_timer = 0.0f;
    bool is_appearing = true;
    float scale = 1.0f;
    int alpha = 255;

    int group_id = 0;
    AttackDirectionType attack_direction = AttackDirectionType::Inward;
    PartType type = PartType::Red;
};
