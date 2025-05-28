#pragma once

#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

// �U�R�G�̍s���p�^�[����`
enum class BossPattern
{
    Entrance
};

class Stage1Boss : public EnemyBase
{
public:
    Stage1Boss();
    ~Stage1Boss();

    // �������E�X�V�E�`��E�I������
    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

    // �O������p�^�[���ݒ�
    void SetPattern(BossPattern new_pattern);

protected:
    // �G�V���b�g�̔��ˏ����iOverride�j
    void Shot(float delta_second) override;

private:
    // ���݂̍s���p�^�[���ƃ^�C�}�[
    BossPattern pattern = BossPattern::Entrance;
    float pattern_timer = 0.0f;
    bool is_alive = true;

    // ���V�t�F�[�Y�̊���W
    bool floating_center_initialized = false;
    float floating_center_x = 0.0f;
    float floating_center_y = 0.0f;

    // ����⏕�ϐ�
    Vector2D start_location;
    bool is_returning = false;
    bool has_shot = false;
    float after_shot_timer = 0.0f;
    float spawn_delay_timer = 0.0f;

    // �A�j���[�V�����֘A
    std::vector<int> images;
    std::vector<int> images_a;
    std::vector<int> images_b;
    std::vector<int> anim_indices;

    // �p�^�[���������_���ɕύX
    void ChangePatternRandomly();


public:
    bool GetIsAlive() const;

};
