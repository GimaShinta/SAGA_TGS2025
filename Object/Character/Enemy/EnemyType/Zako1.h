#pragma once

#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

// �U�R�G�̍s���p�^�[����`
enum class ZakoPattern
{
    MoveStraight,        // �^���������~
    RightMove,           // ���ړ��i�E�j
    LeftMove,            // ���ړ��i���j
    ZIgzag,              // �W�O�U�O�ړ����Ȃ���U��
    MoveAndStopShoot,    // �ړ� �� ��~ �� �U�� �� �ޔ�
    MoveThenDiagonal     // �ړ� �� �΂߈ړ�
};

class Zako : public EnemyBase
{
public:
    Zako();
    ~Zako();

    // �������E�X�V�E�`��E�I������
    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

    // �O������p�^�[���ݒ�
    void SetPattern(ZakoPattern new_pattern);

protected:
    // �G�V���b�g�̔��ˏ����iOverride�j
    void Shot(float delta_second) override;

private:
    // ���݂̍s���p�^�[���ƃ^�C�}�[
    ZakoPattern pattern = ZakoPattern::MoveStraight;
    float pattern_timer = 0.0f;

    // ����⏕�ϐ�
    Vector2D start_location;     // �o���n�_
    bool is_returning = false;   // ���������Ƒޔ𒆂�
    bool has_shot = false;       // ��x�U��������
    float after_shot_timer = 0.0f;
    float spawn_delay_timer = 0.0f;

    // �A�j���[�V�����֘A
    std::vector<int> images;         // ���݂̉摜�Z�b�g
    std::vector<int> images_a;       // �p�^�[��A�i�ʏ�j
    std::vector<int> images_b;       // �p�^�[��B�i�h��j
    std::vector<int> anim_indices;   // �A�j���[�V�����t���[���C���f�b�N�X

    // �p�^�[���������_���ɕύX
    void ChangePatternRandomly();
};
