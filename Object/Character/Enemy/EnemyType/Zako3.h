#pragma once
#include "../EnemyBase.h"
class Zako3 : public EnemyBase
{
private:
	bool zako3_flip = false;

public:
	Zako3();
	~Zako3();

public:
	// ����������
	void Initialize() override;

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delata_second">�P�t���[��������̎���</param>
	void Update(float delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="screen_offset"></param>
	void Draw(const Vector2D& screen_offset)const override;

	// �I��������
	void Finalize() override;

protected:
	void Shot(float delta_second) override;

public:
	void SetFlip(bool flip);

};

