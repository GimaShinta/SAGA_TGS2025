#pragma once
#include "../SceneBase.h"
#include "../SceneType/Stage/StageBase.h"
#include <vector>
#include <string> 
#include "../../Object/Character/Enemy/EnemyBase.h"

class GameMainScene : public SceneBase
{
private:
	class Player* player;
	StageBase* current_stage;
	float next_scene_timer;
	bool isPaused = false;
private:
	int font_digital;
	int font_orbitron;
	int obi_handle; 

	float background_scroll_x = 0.0f;
private:
	int current_bgm_handle = -1;
	int stage_bgm1 = -1; // �X�e�[�W1,2�̋���BGM
	int stage_bgm3 = -1; // �X�e�[�W3�pBGM
	int stage_bgm4 = -1;
	int se_warning = -1;

	float stage3_bgm_volume = 255.0f;
	bool stage3_bgm_fadeout = false;
	bool stage4_bgm_pending = false;
	float se_warning_timer = 0.0f;


	float pause_timer = 0.0f;
	int transparent = 0.0f;

	int se_charge;

	// �� �X�R�A���O�����p�\���̂ƕϐ�
	struct ScoreLog 
	{
		std::string text;
		float timer;
		float y_offset;
	};

	std::vector<ScoreLog> score_logs;
	float previous_score_count = 0.0f;

	int m_selectedIndex = 0; // �I�𒆂̃C���f�b�N�X
	bool m_startTransitioning = false;  // �o�����o���t���O
	float m_transitionTimer = 0.0f;     // �o�����o�^�C�}�[
	int m_menuFontHandle;
	float line_effect_timer = 0.0f; // ���C�����o�p�̓Ǝ��^�C�}�[

	// --- �t�B���^�[���o�p ---
	bool effect_shield_on = false;
	bool effect_shield_off = false;
	bool effect_powerup = false;
	float effect_timer = 0.0f;
	const float effect_duration = 0.4f;



	int vo = 70;


	bool black_fade_started = false;
	float black_fade_timer = 0.0f;
	float black_in_timer = 0.0f;
	float black_in_timer2 = 0.0f;
	int alpha = 0;
	// �����o�ϐ��Ȃ�
	float band_center_y = 360;          // ��ʒ����̊�ʒu
	float band_half_height = 0.0f;      // �т̔����̍����i0����ő�܂ŕω��j
	const float band_max_half_height = 60.0f; // �т̍ő唼���̍����i�܂荂��120�j
	float band_expand_speed = 200.0f;   // �т��L���鑬�x(px/s)
	int font_warning;
	float warning_text_x = 1280.0f; // �����͉E�[�O
	float warning_timer = 0.0f;
	float warning_scroll_speed = 700.0f; // px/�b
	float warning_duration = 11.0f;

	bool is_none = false;

	enum class WarningState {
		None,       // ����
		Expanding,  // �т��L�����Ă���
		Displaying, // �\�����i�e�L�X�g�X�N���[���j
		Shrinking   // �т��k��
	};

	WarningState warning_state = WarningState::None;


public:
	GameMainScene();
	virtual ~GameMainScene();

	// ����������
	virtual void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual eSceneType Update(float delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() override;

	// �I��������
	virtual void Finalize() override;

	// ���݂̃V�[���^�C�v
	virtual eSceneType GetNowSceneType() const override;

	// �X�R�A���O�ǉ��֐�
	void AddScoreLog(const std::string& text);

	void DrawUI();

	void InputSePlay();
};
