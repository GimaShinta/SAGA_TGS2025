#pragma once
#include "EnemyShot/EnemyShotBase.h"
class EnemyBeam1 : public EnemyShotBase
{
private:
	class Boss2* boss2;

	float beam_time = 0.0f;

	float alpha_timer = 0.0f;          // �_�ŗp�̃^�C�}�[
	const float growth_duration = 1.0f;  // �r�[������������܂ł̎���
private:
	std::vector<int> beam_ts;
	std::vector<int> beam_bs;
	int beam_t = NULL;
	int beam_b = NULL;


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

};

