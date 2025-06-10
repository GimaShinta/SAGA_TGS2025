#pragma once
#include "../CharacterBase.h"

class Beam : public CharacterBase
{
private:
	enum class BeamState { Idle, Charging, Firing };
	BeamState beam_state = BeamState::Idle;

	float charge_time = 0.0f;
	const float charge_duration = 1.0f; // 1�b�Ń`���[�W����
	bool charge_on = false;
private:
	bool Beam_flip = false;
	class Player* player;
	float destroy_time = 0.0f;
	float anim_time = 0.0f;
	bool anim_on = false;
private:
	float last_explosion_time = 0.0f; // ���߂̔�����������

private:
	std::vector<int> beam_ts;
	std::vector<int> beam_bs;
	int beam_t = NULL;
	int beam_b = NULL;

public:
	Beam();
	~Beam();

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
	/// <summary>
	/// �q�b�g������
	/// </summary>
	/// <param name="hit_object">������������</param>
	virtual void OnHitCollision(GameObjectBase* hit_object) override;

	void SetBeamFlip(bool flip);
	void SetPlayer(class Player* p_player);
};

