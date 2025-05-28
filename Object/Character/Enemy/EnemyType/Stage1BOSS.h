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
    bool has_shot = false;
    float after_shot_timer = 0.0f;
    float spawn_delay_timer = 0.0f;

    // �ϐg���o�֘A�t���O�E�^�C�}�[
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

    // �����ɖ߂��ď�֑ޏꂷ�鏈���p
    float life_timer = 0.0f;
    bool is_returning = false;         // �����֖߂�t�F�[�Y
    bool is_leaving = false;           // ��ֈړ�����t�F�[�Y
    Vector2D return_target;            // �����̍��W
    float stop_timer = 0.0f;           // �����ł̒�~�^�C�}�[
    float stop_duration = 2.0f;        // ��~����

    bool is_dying = false;           // �������o���t���O
    float dying_timer = 0.0f;        // ���o�o�ߎ���
    int flash_count = 0;             // �_�ŃJ�E���g
    const int max_flash_count = 6;   // �_�ŉ񐔁i3��_�łȂ�6�j
    const float flash_interval = 0.2f; // �_�ł̊Ԋu�i�b�j
    float flash_timer = 0.0f;        // �_�Ń^�C�}�[
    bool is_dying;
    float dying_timer;
    int flash_count;
    float flash_timer;
    int max_flash_count;
    float flash_interval;


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
