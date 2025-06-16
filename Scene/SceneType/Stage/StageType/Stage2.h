#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"  // Zako1�N���X���C���N���[�h

class Stage2 :
    public StageBase
{
private:
    class Shot* shot;
    class Beam* beam;
    class Zako4* zako4;
    class Zako* zako;
    class Stage2BOSS* boss;
    std::vector<EnemyBase*> enemy_list;
    float stage_timer = 0.0f; // �o�ߎ��ԁi�b�j
    bool zako4_spawned = false;
    bool zako5_spawned = false;

    // �G�̏o���^�C�}�[
    float enemy_spawn_timer = 0.0f;
    int stage = 2;
    int brend = 0;
    bool is_warning = false;   // �x���\�������ǂ���
    bool is_brend = false;     // �u�����h���o�����ǂ���

public:
    Stage2(Player* player);
    ~Stage2();

    void Initialize() override;
    void Finalize() override;
    void Update(float delta) override;
    void Draw() override;
    bool IsFinished() override;
    bool IsClear() override;
    bool IsOver() override;
    StageBase* GetNextStage(Player* player) override;

private:
    bool finished = false;
    float scene_timer = 0.0f;  // ���o���ԑJ�ڗp�^�C�}�[

    // �G�o������
    void EnemyAppearance(float delta);

    // �w�i�X�N���[��
    void DrawScrollBackground() const override;
};
