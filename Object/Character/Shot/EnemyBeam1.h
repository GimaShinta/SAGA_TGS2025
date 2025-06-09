#pragma once
#include "EnemyShot/EnemyShotBase.h"
class EnemyBeam1 : public EnemyShotBase
{
private:
	class Boss2* boss2;

	float beam_time = 0.0f;

	float alpha_timer = 0.0f;          // 点滅用のタイマー
	const float growth_duration = 1.0f;  // ビームが完成するまでの時間
private:
	std::vector<int> beam_ts;
	std::vector<int> beam_bs;
	int beam_t = NULL;
	int beam_b = NULL;


public:
	EnemyBeam1();
	~EnemyBeam1();

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

	/// <summary>
	/// ヒット時処理
	/// </summary>
	/// <param name="hit_object">当たった相手</param>
	virtual void OnHitCollision(GameObjectBase* hit_object) override;

	void SetBoss2(class Boss2* p_boss);

};

