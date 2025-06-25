#pragma once
#include "../SceneBase.h"
class TitleScene : public SceneBase
{
private:
	int m_selectedIndex; // �I�𒆂̃C���f�b�N�X
	int m_menuFontHandle;

	int m_logoHandle;
	float m_logoScale; // �A�j���p�X�P�[��
	int m_logoAlpha;   // �A�j���p�����x

	//���S�̍��W�����ϐ�
	int m_logoX;
	int m_logoY;
	int m_logoW;
	int m_logoH;

	float m_logoAppearTimer = 0.0f;
	bool m_logoAppeared = false;

	int logo_se;

	int drawY;
	float m_cursorY; // �J�[�\��Y���W



	//�I������ԃt���O
	bool m_startTransitioning = false;  // �o�����o���t���O
	float m_transitionTimer = 0.0f;     // �o�����o�^�C�}�[


	float m_menuScale = 1.0f;
	float m_menuAlpha = 255.0f;


	//�p�[�e�B�N���̉��o�p
	struct Particle {
		float x, y, vy;
		int alpha;
	};
	std::vector<Particle> m_particles;


public:
	TitleScene();
	virtual ~TitleScene();

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


private:
	//�`��n�֐�
	void DrawBackgroundGrid();	//�w�i
	void DrawLogo();			//���S
	void DrawParticles();		//�p�[�e�B�N��
	void DrawMenu();			//���j���[
};

