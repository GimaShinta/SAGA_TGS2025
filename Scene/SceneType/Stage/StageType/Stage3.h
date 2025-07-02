#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"

const int FPS3 = 60; // �t���[�����[�g
const int STAGE3_DISTANCE = FPS3 * 5; // �X�e�[�W�̒���

class Stage3 : public StageBase
{
private:
	class Shot* shot;
	class Beam* beam;
	class Zako2* zako2;
	class Boss* boss;
	class Boss2* boss2;
	class EnemyShot5* e_shot5;
	class EnemyShot4* e_shot4;
	class EnemyShot3* e_shot3;
	class EnemyShot2* e_shot2;
	class EnemyShot1* e_shot1;
	std::vector<EnemyBase*> enemy_list;
	StageBase* current_stage;
	int anim_id = 0;
	std::vector<int> image_handles;

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
	float gameover_timer = 0;      // ���Ԃ̐i�s���Ǘ�
	int transparent = 0;
	bool is_warning = false;
	bool is_brend = false;
	int bs_attack_pattrn = 0;
	float shot_timer;

	float spiral_angle = 0.0f; // �X�p�C�����p�x�i���t���[�������j
	float spiral_interval = 0.1f; // ���ˊԊu�i�b�j
	float spiral_timer = 0.0f; // �o�ߎ���

	float stage_timer = 0.0f; // �o�ߎ��ԁi�b�j
	float scene_timer = 0.0f;

	float spiral_duration_timer = 0.0f;
	bool is_spiral_active = false;

	int font_digital;
	int font_orbitron;
	int obi_handle;

	bool zako3_spawned = false;
	bool boss2_spawned = false;
	float enemy_spawn_timer = 0.0f;
	float boss2_spawn_timer = 0.0f;

	int enemy_group_index = 0; // ����: ���A�: �E

	float enemy_group_timer = 0.0f;
	bool is_spawning_group = false;

	bool draw_animation_first = false;  // �f�t�H���g��false


	bool is_warning_finished = false;

	bool is_warning_active = false;
	float warning_duration = 10.0f;

	float warning_text_x = 1280.0f; // �����͉E�[�O
	float warning_scroll_speed = 700.0f; // px/�b


	// �����o�ϐ��Ȃ�
	float band_center_y = 360;          // ��ʒ����̊�ʒu
	float band_half_height = 0.0f;      // �т̔����̍����i0����ő�܂ŕω��j
	const float band_max_half_height = 60.0f; // �т̍ő唼���̍����i�܂荂��120�j
	float band_expand_speed = 200.0f;   // �т��L���鑬�x(px/s)
	int font_warning;


	enum class WarningState {
		None,       // ����
		Expanding,  // �т��L�����Ă���
		Displaying, // �\�����i�e�L�X�g�X�N���[���j
		Shrinking   // �т��k��
	};

	WarningState warning_state = WarningState::None;

	// Stage3.h �Ɉȉ��̃����o�ϐ���ǉ�
	bool is_zako7_group_spawned = false;
public:
	Stage3(Player* player);
	~Stage3();

	void Initialize() override;
	void Finalize() override;
	void Update(float delta) override;
	void Draw() override;
	bool IsFinished() override;
	void SetFinished() override;
	bool IsClear() override;
	bool IsOver() override;
	StageBase* GetNextStage(Player* player) override;
	StageBase* GetNowStage() override;

private:
	void DisplayWarning(float delta_second);
	void PlayerShot();
	void EnemyAppearance(float delta);
	void EnemyShot(float delta_second);
	void UpdateGameStatus(float delta);

private:
	bool finished = false;

	/*�X�N���[��*/
	void DrawScrollBackground() const override;
	void ScrollEffectUpdate(float delta);

	void HandleZako1_LR(float delta);
	void HandleZako1_Left(float delta);
	void HandleZako1_Right(float delta);
	void HandleZako1_Center(float delta);
	void HandleZako2_Upward(float delta);
	void SpawnBossAndItems();

	void DrawFrontGrid() const;

	float zako2_spawn_timer = 0.0f;

	void StartWarning();

	//�O���b�h
	mutable float scroll_back = 0.0f;
	mutable float scroll_front = 0.0f;
	mutable float time = 0.0f;


	struct StarParticle
	{
		Vector2D pos;
		Vector2D velocity;
		float alpha;
		float length;
		float life;
	};

	mutable std::vector<StarParticle> star_particles;


	void ResultDraw(float delta);  // �� �֐��v���g�^�C�v�ǉ�

	float result_timer = 0.0f;
	float total_score = 0.0f;
	bool result_displayed = false;

	float clear_wait_timer = 0.0f;
	bool result_started = false;

	float glitch_timer = 0.0f;
	bool glitch_started = false;
	bool glitch_done = false;
	float post_result_wait_timer = 0.0f;  // �������ǉ�
	float delta_draw = 0.0f;

	bool result_fadeout_started = false;
	float result_fadeout_timer = 0.0f;
	bool result_ended = false;

	const float black_fade_duration = 60.0f; // 60�t���[���i��1�b�j���x

	public:
	int se_warning = -1;       // �x��SE�̃n���h��
	bool se_warning_played = false; // ��x�����炷�p�t���O
	bool request_play_warning_se = false;  // �x������炷�v��
	bool request_play_stage4_bgm = false;

	// ���o���Ǘ�
	int spawn_index_left = 0;
	float spawn_delay_timer_left = 0.0f;
	bool spawning_zako_left = false;

	// �E�o���Ǘ�
	int spawn_index_right = 0;
	float spawn_delay_timer_right = 0.0f;
	bool spawning_zako_right = false;

	template <typename T>
	T my_max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	}

	float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

};