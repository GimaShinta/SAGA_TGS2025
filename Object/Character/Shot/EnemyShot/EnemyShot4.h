#pragma once
#include "EnemyShotBase.h"
#include <vector>
class EnemyShot4 :
	public EnemyShotBase
{
private:
	int attack_pattrn = 0;


	std::vector<int> bullet;
	std::vector<int> bullet_1;
	std::vector<int> bullet_2;
	std::vector<int> bullet_3;

	std::vector<std::vector<int>> bullets;

public:
	EnemyShot4();
	~EnemyShot4();

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

	void SetAttackPattrn(int pattrn);

};

