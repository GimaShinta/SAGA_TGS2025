#pragma once
#include "../EnemyBase.h"

enum class Zako5Phase
{
    Appearing,   // 登場中
    Fighting     // 通常戦闘
};

class Zako5 : public EnemyBase
{
public:
    Zako5();
    Zako5(const Vector2D& pos);  // 位置を受け取るコンストラクタを追加
    ~Zako5();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

protected:
    void Shot(float delta_second) override;

private:
    float attack_cooldown = 0.0f;     // 攻撃の合間時間
    bool is_attacking = false;         // 攻撃中かどうか
    int attack_pattern = 6;            // 使用中の攻撃パターン（6～7）

    float spiral_timer = 0.0f;
    float spiral_total_time = 0.0f;
    Zako5Phase phase = Zako5Phase::Appearing;

    float appear_timer = 0.0f;
    bool is_invincible = true;

    void Pattrn6(float delta_second);
    void Pattrn7(float delta_second);
};
