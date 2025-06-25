#pragma once
#include "../CharacterBase.h"

class Shot : public CharacterBase
{
private:
	bool shot_flip = false;
	int se;
	int hit_se;

public:
	Shot();
	~Shot();

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

	void SetShotFlip(bool flip);
};

