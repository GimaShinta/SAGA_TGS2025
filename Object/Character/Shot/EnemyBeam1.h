#pragma once
#include "EnemyShot/EnemyShotBase.h"
class EnemyBeam1 : public EnemyShotBase
{
private:
	class Boss2* boss2;
	class Boss3* boss3;

	float beam_time = 0.0f;

	float alpha_timer = 0.0f;          // �_�ŗp�̃^�C�}�[

	float shrink_duration = 1.0f;  // �k���ɂ����鎞��
	float beam_lifetime = 8.0f;    // �r�[���S�̂̐������ԁi���Ɠ����j
	enum class BeamOwnerType 
	{
		None,
		Boss2,
		Boss3,
	};

	BeamOwnerType beam_owner_type = BeamOwnerType::None;
	float min_thickness = 8.0f;
	float max_thickness = 48.0f;
	int beam_t_index = 10;
	int beam_b_index = 10;
	float growth_duration_b = 4.0f;
	float growth_duration_s = 0.5f;

private:
	std::vector<int> beam_ts;
	std::vector<int> beam_bs;
	std::vector<int> beams_t;
	std::vector<int> beams_b;
	int beam_t = NULL;
	int beam_b = NULL;


	enum class BeamState 
	{
		Warning,
		Firing,
		Holding,   // �� �ǉ�
		Shrinking, // �� �ǉ��I
	};

	BeamState state = BeamState::Warning;
	float warning_time = 1.0f;  // �\�����̕\������
	float state_timer = 0.0f;   // ��Ԃ̌o�ߎ���

public:
	EnemyBeam1();
	~EnemyBeam1();

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

	/// <summary>
	/// �q�b�g������
	/// </summary>
	/// <param name="hit_object">������������</param>
	virtual void OnHitCollision(GameObjectBase* hit_object) override;

	void SetBoss2(class Boss2* p_boss);
	void SetBoss3(class Boss3* p_boss);

	template<typename T>
	T MyMax(const T& a, const T& b) {
		return (a > b) ? a : b;
	}
};

