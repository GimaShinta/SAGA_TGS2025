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
	// 初期化処理
	virtual void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delata_second">１フレーム当たりの時間</param>
	virtual void Update(float delta_second) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="screen_offset"></param>
	virtual void Draw(const Vector2D& screen_offset)const override;

	// 終了時処理
	virtual void Finalize() override;

	void SetAttackPattrn(int pattrn);

};

