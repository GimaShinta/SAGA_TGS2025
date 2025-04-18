#pragma once
#include "../CharacterBase.h"

class Beam : public CharacterBase
{
private:
	bool Beam_flip = false;
	class Player* player;
	float destroy_time = 0.0f;

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

