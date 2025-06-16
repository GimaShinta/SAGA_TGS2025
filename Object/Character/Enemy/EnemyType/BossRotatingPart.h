#pragma once

#include "../../../../Object/GameObjectBase.h"

class Stage2Boss;

class BossRotatingPart : public GameObjectBase
{
public:
    BossRotatingPart() = default;
    ~BossRotatingPart() override = default;

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void OnHitCollision(GameObjectBase* hit_object) override;


    // 後からコア（ボス）などの情報を設定する
    void SetUp(Stage2Boss* boss, float radius, float angle_offset);

private:
    Stage2Boss* boss = nullptr;  // 回る対象（コア）
    float radius = 80.0f;        // 回転半径
    float angle_offset = 0.0f;   // 初期角度のズレ
    float angle = 0.0f;          // 現在角度
    std::vector<int> images;
    int image_index = 0;
    float anim_timer = 0.0f;
    int box_size = 20; // 衝突範囲・表示用サイズ

    float shot_timer = 0.0f;
    bool use_beam = false; // Beamを撃つかどうか

};
