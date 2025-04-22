#pragma once
#include "../SceneBase.h"
#include<stdio.h>



class StartScene :  public SceneBase
{
private:
	enum class Step {
		WaitForInput,
		IntroText,
		GoToTitle
	};

	Step m_step;
	float m_elapsedTime;
	int m_displayCharCount;
	static const int LINE_COUNT = 2;
	const char* m_introText[LINE_COUNT]; // 2�s���̕�����

public:
	StartScene();
	virtual ~StartScene();

public:
	// ����������
	virtual void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">�P�t���[��������̎���</param>
	/// <returns></returns>
	virtual eSceneType Update(float delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <returns></returns>
	virtual void Draw() override;

	// �I���������i�g�����C���X�^���X�̍폜�Ƃ��j
	virtual void Finalize() override;

	// ���݂̃V�[���^�C�v�i�I�[�o�[���C�h�K�{�j
	virtual eSceneType GetNowSceneType() const override;

};

