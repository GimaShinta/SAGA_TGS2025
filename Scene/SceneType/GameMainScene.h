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

	// �� �X�R�A���O�����p�\���̂ƕϐ�
	struct ScoreLog {
		std::string text;
		float timer;
		float y_offset;
	};
	std::vector<ScoreLog> score_logs;
	float previous_score_count = 0.0f;

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
};
