#pragma once

#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

// ザコ敵の行動パターン定義
enum class ZakoPattern
{
    MoveStraight,
    RightMove,
    LeftMove,
    ZIgzag,
    SideAppearAndShoot,
    MoveAndStopShoot,
    MoveThenDiagonal,
    Formation,
    DiveOnce,
    ArcMoveAndStop,
    DepthAppear,
    RetreatUp,     // ← 追加：上方向に退場
    Count
};

class Zako : public EnemyBase
{
public:
    Zako();
    Zako(const Vector2D& pos);
    ~Zako();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

    void SetPattern(ZakoPattern new_pattern);
    ZakoPattern GetPattern() const
    {
        return pattern;
    }  // ← 追加

protected:
    void Shot(float delta_second) override;

private:
    ZakoPattern pattern = ZakoPattern::MoveStraight;
    float pattern_timer = 0.0f;

    Vector2D start_location;
    bool is_returning = false;
    bool has_shot = false;
    float after_shot_timer = 0.0f;
    float spawn_delay_timer = 0.0f;
    float scale = 1.0f;

    std::vector<int> images;
    std::vector<int> images_a;
    std::vector<int> images_b;
    std::vector<int> anim_indices;

    void ChangePatternRandomly();
};
