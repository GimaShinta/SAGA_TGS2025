#pragma once

#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

// ザコ敵の行動パターン定義
enum class ZakoPattern
{
    MoveStraight,        // 真っすぐ下降
    RightMove,           // 横移動（右）
    LeftMove,            // 横移動（左）
    ZIgzag,              // ジグザグ移動しながら攻撃
    MoveAndStopShoot,    // 移動 → 停止 → 攻撃 → 退避
    MoveThenDiagonal     // 移動 → 斜め移動
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
    Vector2D start_location;     // 出現地点
    bool is_returning = false;   // 撃ったあと退避中か
    bool has_shot = false;       // 一度攻撃したか
    float after_shot_timer = 0.0f;
    float spawn_delay_timer = 0.0f;

    // アニメーション関連
    std::vector<int> images;         // 現在の画像セット
    std::vector<int> images_a;       // パターンA（通常）
    std::vector<int> images_b;       // パターンB（派手）
    std::vector<int> anim_indices;   // アニメーションフレームインデックス

    // パターンをランダムに変更
    void ChangePatternRandomly();
};
