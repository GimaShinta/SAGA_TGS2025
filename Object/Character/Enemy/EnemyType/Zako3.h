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
	// 初期化処理
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delata_second">１フレーム当たりの時間</param>
	void Update(float delta_second) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="screen_offset"></param>
	void Draw(const Vector2D& screen_offset)const override;

	// 終了時処理
	void Finalize() override;

protected:
	void Shot(float delta_second) override;

public:
	void SetFlip(bool flip);

};

