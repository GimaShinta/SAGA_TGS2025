#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"

const int FPS = 60; // �t���[�����[�g
const int STAGE_DISTANCE = FPS * 10; // �X�e�[�W�̒���

class Stage1 : public StageBase 
{
private:
	class Shot* shot;
	class Beam* beam;
	class Zako2* zako2;
	class Zako4* zako4;
	class Boss* boss;
	class Boss2* boss2;
	class EnemyShot3* e_shot3;
	class EnemyShot2* e_shot2;
	class EnemyShot1* e_shot1;
	std::vector<EnemyBase*> enemy_list;
	StageBase* current_stage;
	float enemy_spawn_timer = 0.0f;
	float distance_timer = 0.0f; 
	bool zako4_spawned = false;


private:
	int distance = 0;   // �X�e�[�W�I�[�܂ł̋���
	int boss_idx = 0;    // �{�X���������z��̃C���f�b�N�X
	int stage = 1;      // �X�e�[�W
	int score = 0;      // �X�R�A
	int hisco = 10000;  // �n�C�X�R�A
	int no_damage = 0;   // ���G���
	int weapon_lev = 1;   // ���@�̕���̃��x���i�����ɔ��˂����e���j
	int timer = 0;      // ���Ԃ̐i�s���Ǘ�
	int reach_count = 0;      // ���Ԃ̐i�s���Ǘ�
	int brend = 0;      // ���Ԃ̐i�s���Ǘ�
	float warning_timer = 0;      // ���Ԃ̐i�s���Ǘ�
	bool is_warning = false;
	bool is_brend = false;
	int bs_attack_pattrn = 0;
	float shot_timer;

	float stage_timer = 0.0f; // �o�ߎ��ԁi�b�j
	float scene_timer = 0.0f;

public:
    Stage1(Player* player);
    ~Stage1();

    void Initialize() override;
    void Finalize() override;
    void Update(float delta) override;
    void Draw() override;
    bool IsFinished() override;
    bool IsClear() override;
    bool IsOver() override;
    StageBase* GetNextStage(Player* player) override;

private:
	void DisplayWarning(float delta_second);
	void EnemyAppearance();
	void EnemyShot(float delta_second);

private:
    bool finished = false;
};