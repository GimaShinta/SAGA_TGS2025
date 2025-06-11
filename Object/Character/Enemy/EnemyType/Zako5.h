#pragma once
#include "../EnemyBase.h"

enum class Zako5Phase
{
    Appearing,   // �o�ꒆ
    Fighting     // �ʏ�퓬
};

class Zako5 : public EnemyBase
{
public:
    Zako5();
    Zako5(const Vector2D& pos);  // �ʒu���󂯎��R���X�g���N�^��ǉ�
    ~Zako5();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

protected:
    void Shot(float delta_second) override;

private:
    float attack_cooldown = 0.0f;     // �U���̍��Ԏ���
    bool is_attacking = false;         // �U�������ǂ���
    int attack_pattern = 6;            // �g�p���̍U���p�^�[���i6�`7�j

    float spiral_timer = 0.0f;
    float spiral_total_time = 0.0f;
    Zako5Phase phase = Zako5Phase::Appearing;

    float appear_timer = 0.0f;
    bool is_invincible = true;

    void Pattrn6(float delta_second);
    void Pattrn7(float delta_second);
};
