#pragma once
#include "EnemyShotBase.h"
class EnemyShot5 :
    public EnemyShotBase
{
private:
	Vector2D suck_center;

public:
	bool is_wave_reflected = false; // �g�̌��������]���Ă��邩

private:
	enum class ShotState 
	{
		SpreadOut,
		PullIn,
		Explode
	};
	ShotState shot_state = ShotState::SpreadOut;

private:
	Vector2D base_velocity;     // �O�i�����̑��x
	float wave_amplitude;       // �g�̐U�ꕝ
	float wave_frequency;       // �g�̎��g��

public:
	EnemyShot5();
	~EnemyShot5();

public:
	// ����������
	virtual void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delata_second">�P�t���[��������̎���</param>
	virtual void Update(float delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="screen_offset"></param>
	virtual void Draw(const Vector2D& screen_offset)const override;

	// �I��������
	virtual void Finalize() override;

public:
	void SetSuckCenter(const Vector2D& center);
	void SetWaveParameters(float amplitude, float frequency);
	void SetWaveReflected(bool reflected); // �ݒ�֐�
};

