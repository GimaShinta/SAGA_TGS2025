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
    MoveAndStopShoot,
    MoveThenDiagonal,
    Homing,
    Circular,
    Formation // ← 追加済み
};

class Zako : public EnemyBase
{
public:
    Zako();
    ~Zako();

    // 初期化・更新・描画・終了処理
    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

    // 外部からパターン設定
    void SetPattern(ZakoPattern new_pattern);

protected:
    // 敵ショットの発射処理（Override）
    void Shot(float delta_second) override;

private:
    // 現在の行動パターンとタイマー
    ZakoPattern pattern = ZakoPattern::MoveStraight;
    float pattern_timer = 0.0f;

    // 動作補助変数
    Vector2D start_location;
    bool is_returning = false;
    bool has_shot = false;
    float after_shot_timer = 0.0f;
    float spawn_delay_timer = 0.0f;

    // アニメーション関連
    std::vector<int> images;
    std::vector<int> images_a;
    std::vector<int> images_b;
    std::vector<int> anim_indices;

    // パターンをランダムに変更
    void ChangePatternRandomly();
};
