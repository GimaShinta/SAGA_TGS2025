#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"

const int FPS = 60; // �t���[�����[�g�i1�b������̍X�V�񐔁j
const int STAGE_DISTANCE = FPS * 10; // �X�e�[�W�̑S���i10�b���j

class Stage1 : public StageBase
{
private:
	// �e��U���E�G�L�����N�^�[�̃|�C���^��`
	class Shot* shot;               // �v���C���[�̃V���b�g
	class Beam* beam;               // �v���C���[�̃r�[���U��
	class Zako* zako;             // �G���G����1
	class Zako2* zako2;             // �G���G����2
	class Zako4* zako4;             // �G���G����4�i1��̂ݏo���j
	class Stage1Boss* boss1;
	class Boss* boss;               // �{�X�L����1
	class Boss2* boss2;             // �{�X�L����2
	class EnemyShot3* e_shot3;      // �G�̃V���b�g�^�C�v3
	class EnemyShot2* e_shot2;      // �G�̃V���b�g�^�C�v2
	class EnemyShot1* e_shot1;      // �G�̃V���b�g�^�C�v1

	std::vector<EnemyBase*> enemy_list; // �X�e�[�W��ɑ��݂���G�̃��X�g
	StageBase* current_stage;           // ���݂̃X�e�[�W�i�X�e�[�W�J�ڗp�j

	float enemy_spawn_timer = 0.0f;     // �G�o���^�C�}�[
	float distance_timer = 0.0f;        // �X�e�[�W�̐i�s�^�C�}�[
	bool zako4_spawned = false;         // Zako4�����łɏo���������ǂ���

private:
	int distance = 0;          // �X�e�[�W�I�[�܂ł̎c�苗��
	int boss_idx = 0;          // �{�X�̏o������C���f�b�N�X
	int stage = 1;             // ���݂̃X�e�[�W�ԍ�
	int score = 0;             // �v���C���[�̃X�R�A
	int hisco = 10000;         // �n�C�X�R�A
	int no_damage = 0;         // �v���C���[���G���ԊǗ��p�t���O
	int weapon_lev = 1;        // ����̃��x���i�e�̐��ɉe���j
	int timer = 0;             // ���Ԍo�ߗp�ėp�^�C�}�[
	int reach_count = 0;       // �X�e�[�W���B����p�J�E���^�[
	int brend = 0;             // �u�����h�G�t�F�N�g�p�^�C�}�[
	float warning_timer = 0;   // �x���\���̃^�C�}�[
	bool is_warning = false;   // �x���\�������ǂ���
	bool is_brend = false;     // �u�����h���o�����ǂ���
	int bs_attack_pattrn = 0;  // �{�X�̍U���p�^�[������p�C���f�b�N�X
	float shot_timer;          // �V���b�g���˂̊Ԋu����^�C�}�[

	float stage_timer = 0.0f;  // �X�e�[�W�̌o�ߎ��ԁi�b�P�ʁj

	float scene_timer = 0.0f;  // ���o���ԑJ�ڗp�^�C�}�[
	bool boss_spawned = false;
	

	


	static const int TILE_COLS = 8;
	static const int TILE_ROWS = 6;

	bool tile_destroy_flags[TILE_ROWS][TILE_COLS] = { false };
	float tile_destroy_timer = 0.0f;


public:
	// �R���X�g���N�^�E�f�X�g���N�^
	Stage1(Player* player);     // �v���C���[���������Ɏ��X�e�[�W������
	~Stage1();                  // �f�X�g���N�^

	// ��{�I�ȃX�e�[�W�����Q
	void Initialize() override;              // �X�e�[�W����������
	void Finalize() override;                // �X�e�[�W�I������
	void Update(float delta) override;       // ���t���[���̍X�V����
	void Draw() override;                    // �`�揈��
	bool IsFinished() override;              // �X�e�[�W���I���������ǂ���
	bool IsClear() override;                 // �X�e�[�W�N���A����
	bool IsOver() override;                  // �Q�[���I�[�o�[����
	StageBase* GetNextStage(Player* player) override; // ���̃X�e�[�W���擾



private:
	// �X�e�[�W��������
	void DisplayWarning(float delta_second); // �x�����o����
	void EnemyAppearance(float delta);       // �G�̏o������
	void PlayerShot();

private:
	bool finished = false;                   // �X�e�[�W�I���t���O

private:
	int bg_image;	//�w�i�摜
	mutable float grid_scroll_back = 0.0f;
	mutable float grid_scroll_mid = 0.0f;
	bool is_boss_dying = false;
	float boss_death_timer = 0.0f;
	float screen_shake_power = 0.0f;


	int font_digital;

	float scroll_y = 0.0f;

	struct StarParticle {
		Vector2D pos;
		Vector2D velocity;
		float alpha;
		float length;   // ���̒���
		float life;
	};

	mutable std::vector<StarParticle> star_particles;


	void DrawScrollBackground() const override; // �w�i�̃X�N���[���`��

	mutable std::vector<std::pair<Vector2D, float>> ripple_list; // �ʒu�Ǝ���

};
