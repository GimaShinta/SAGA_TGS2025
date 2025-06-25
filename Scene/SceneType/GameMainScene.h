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

	int vo = 70;

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

	void InputSePlay();
};
