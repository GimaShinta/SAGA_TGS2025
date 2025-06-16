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


    // �ォ��R�A�i�{�X�j�Ȃǂ̏���ݒ肷��
    void SetUp(Stage2Boss* boss, float radius, float angle_offset);

private:
    Stage2Boss* boss = nullptr;  // ���Ώہi�R�A�j
    float radius = 80.0f;        // ��]���a
    float angle_offset = 0.0f;   // �����p�x�̃Y��
    float angle = 0.0f;          // ���݊p�x
    std::vector<int> images;
    int image_index = 0;
    float anim_timer = 0.0f;
    int box_size = 20; // �Փ˔͈́E�\���p�T�C�Y

    float shot_timer = 0.0f;
    bool use_beam = false; // Beam�������ǂ���

};
