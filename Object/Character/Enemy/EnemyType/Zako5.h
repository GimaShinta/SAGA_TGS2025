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
    // �U������p
    float attack_cooldown = 0.0f;     // �U���̍��Ԏ���
    bool is_attacking = false;       // �U�������ǂ���
    int attack_pattern = 6;          // �g�p���̍U���p�^�[���i4?7�j

    // �p�^�[���p�ʃ^�C�}�[�istatic�p�~�j
    float spiral_timer = 0.0f;
    float spiral_total_time = 0.0f;




    // �U���p�^�[���iStage3����ڐA�j
    void Pattrn6(float delta_second);
    void Pattrn7(float delta_second);
};