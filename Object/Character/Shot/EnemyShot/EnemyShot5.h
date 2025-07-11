#pragma once
#include "EnemyShotBase.h"
class EnemyShot5 :
    public EnemyShotBase
{
private:
	Vector2D suck_center;

public:
	bool is_wave_reflected = false; // 波の向きが反転しているか

private:
	enum class ShotState 
	{
		SpreadOut,
		PullIn,
		Explode
	};
	ShotState shot_state = ShotState::SpreadOut;

private:
	Vector2D base_velocity;     // 前進方向の速度
	float wave_amplitude;       // 波の振れ幅
	float wave_frequency;       // 波の周波数

public:
	EnemyShot5();
	~EnemyShot5();

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

public:
	void SetSuckCenter(const Vector2D& center);
	void SetWaveParameters(float amplitude, float frequency);
	void SetWaveReflected(bool reflected); // 設定関数
};

