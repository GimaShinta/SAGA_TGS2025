// Stage2BOSS.h（修正済み）
#pragma once

#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "BossRotatingPart.h"

class Stage2Boss : public EnemyBase
{
public:
    Stage2Boss();
    ~Stage2Boss();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;
    void TakeDamage(int amount);
    void SetPlayer(Player* p)
    {
        player = p;
    }

    int GetActiveGroupID() const
    {
        return active_group_id;
    }
    int GetHP() const
    {
        return hp;
    }
    bool GetIsAlive() const
    {
        return is_alive;
    }

    bool IsHpBelowThreshold() const
    {
        return hp < 6666;
    }
    bool IsTransforming() const
    {
        return is_transforming;
    }
    float GetTransformProgress() const;

    bool IsInAttackPhase() const // ←★追加ポイント
    {
        return pattern == Stage2BossPattern::RotateStart || pattern == Stage2BossPattern::AttackPhase;
    }

    enum class Stage2BossPattern
    {
        Entrance,
        Idle,
        RotateStart,
        AttackPhase
    };

    Stage2BossPattern pattern = Stage2BossPattern::Entrance;
    float pattern_timer = 0.0f;

private:
    Player* player = nullptr;
    float shot_timer = 0.0f;

    std::vector<int> images;
    std::vector<int> images_a;
    std::vector<int> images_b;
    std::vector<int> anim_indices;
    int image = -1;

    float part_attack_timer = 0.0f;
    int active_group_id = 0;

    BossRotatingPart::AttackDirectionType main_attack_type = BossRotatingPart::AttackDirectionType::Inward;
    std::vector<BossRotatingPart*> rotating_parts;

    int hp = 7000;
    bool is_alive = true;

    bool is_flashing = false;
    float flash_timer = 0.0f;
    bool visible = true;
    const float flash_interval = 0.2f;

    bool is_transforming = false;
    bool is_transformed = false;
    float transform_timer = 0.0f;
    const float transform_duration = 2.0f;

    bool is_screen_flash = false;
    float screen_flash_timer = 0.0f;
    const float screen_flash_duration = 0.3f;

    float life_timer = 0.0f;
    bool has_started_rotation = false;
    float wait_after_entry_timer = 0.0f;

    void Shot(float delta_second);

public:
    const std::vector<BossRotatingPart*>& GetRotatingParts() const
    {
        return rotating_parts;
    }

};
