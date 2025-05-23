#pragma once
#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

enum class Zako1Pattern
{
    MoveStraight,
    RightMove,
    LeftMove,
    ZIgzag,
    MoveAndStopShoot,
    MoveThenDiagonal
};

class Zako1 : public EnemyBase
{
public:
    Zako1();
    ~Zako1();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

protected:
    void Shot(float delta_second) override;

private:
    Zako1Pattern pattern = Zako1Pattern::MoveStraight;
    float pattern_timer = 0.0f;
    Vector2D start_location;
    bool is_returning = false;
    bool on_hit;
    bool has_shot = false;
    float after_shot_timer = 0.0f;
    bool just_shot = false;

    std::vector<int> images;         // 現在使用中の画像セット
    std::vector<int> images_a;       // 通常行動用
    std::vector<int> images_b;       // 停止撃ちパターン用
    std::vector<int> anim_indices;   // 再生するアニメフレーム

    Vector2D follow_velocity;
    float spawn_delay_timer;

    void ChangePatternRandomly();

public:
    void SetPattern(Zako1Pattern new_pattern);
};
