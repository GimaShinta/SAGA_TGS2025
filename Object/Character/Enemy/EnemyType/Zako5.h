#pragma once
#include "../EnemyBase.h"

enum class Zako5Phase
{
    Appearing,
    Fighting
};

class Zako5 : public EnemyBase
{
public:
    Zako5();
    Zako5(const Vector2D& pos);
    ~Zako5();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

protected:
    void Shot(float delta_second) override;

private:
    // フェーズ管理
    Zako5Phase phase = Zako5Phase::Appearing;
    float appear_timer = 0.0f;

    // 登場演出
    float scale = 0.2f;
    int alpha = 0;

    // 無敵状態（登場中など）
    bool is_invincible = true;

    // 攻撃関連
    bool is_attacking = false;
    float attack_cooldown = 0.0f;
    int attack_pattern = 6;

    float spiral_timer = 0.0f;
    float spiral_total_time = 0.0f;

    void Pattern6(float delta_second);
    void Pattern7(float delta_second);
};
