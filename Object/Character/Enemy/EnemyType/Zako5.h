// Zako5.h
#pragma once
#include "../EnemyBase.h"

class Zako5 : public EnemyBase
{
public:
    Zako5();
    ~Zako5();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

protected:
    void Shot(float delta_second) override;

private:
    // 攻撃制御用
    float attack_cooldown = 0.0f;     // 攻撃の合間時間
    bool is_attacking = false;       // 攻撃中かどうか
    int attack_pattern = 6;          // 使用中の攻撃パターン（4?7）

    // パターン用個別タイマー（static廃止）
    float spiral_timer = 0.0f;
    float spiral_total_time = 0.0f;




    // 攻撃パターン（Stage3から移植）
    void Pattrn6(float delta_second);
    void Pattrn7(float delta_second);
};