#pragma once

#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

// ザコ敵の行動パターン定義
enum class BossPattern
{
    Entrance
};

class Stage1Boss : public EnemyBase
{
public:
    Stage1Boss();
    ~Stage1Boss();

    // 初期化・更新・描画・終了処理
    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

    // 外部からパターン設定
    void SetPattern(BossPattern new_pattern);

protected:
    // 敵ショットの発射処理（Override）
    void Shot(float delta_second) override;

private:
    // 現在の行動パターンとタイマー
    BossPattern pattern = BossPattern::Entrance;
    float pattern_timer = 0.0f;
    bool is_alive = true;

    // 浮遊フェーズの基準座標
    bool floating_center_initialized = false;
    float floating_center_x = 0.0f;
    float floating_center_y = 0.0f;

    // 動作補助変数
    Vector2D start_location;
    bool has_shot = false;
    float after_shot_timer = 0.0f;
    float spawn_delay_timer = 0.0f;

    // 変身演出関連フラグ・タイマー
    bool is_transforming = false;
    float transform_timer = 0.0f;
    float transform_duration = 2.0f;
    bool is_transformed = false;

    bool is_flashing = false;
    float flash_timer = 0.0f;
    float flash_interval = 0.2f;
    bool visible = true;

    bool is_screen_flash = false;
    float screen_flash_timer = 0.0f;
    float screen_flash_duration = 0.3f;

    // 中央に戻って上へ退場する処理用
    float life_timer = 0.0f;
    bool is_returning = false;         // 中央へ戻るフェーズ
    bool is_leaving = false;           // 上へ移動するフェーズ
    Vector2D return_target;            // 中央の座標
    float stop_timer = 0.0f;           // 中央での停止タイマー
    float stop_duration = 2.0f;        // 停止時間

    bool is_dying = false;           // 爆発演出中フラグ
    float dying_timer = 0.0f;        // 演出経過時間
    int flash_count = 0;             // 点滅カウント
    const int max_flash_count = 6;   // 点滅回数（3回点滅なら6）
    const float flash_interval = 0.2f; // 点滅の間隔（秒）
    float flash_timer = 0.0f;        // 点滅タイマー
    bool is_dying;
    float dying_timer;
    int flash_count;
    float flash_timer;
    int max_flash_count;
    float flash_interval;


    // アニメーション関連
    std::vector<int> images;
    std::vector<int> images_a;
    std::vector<int> images_b;
    std::vector<int> anim_indices;

    // パターンをランダムに変更
    void ChangePatternRandomly();

public:
    bool GetIsAlive() const;
};
