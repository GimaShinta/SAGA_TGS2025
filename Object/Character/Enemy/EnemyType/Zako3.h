#pragma once
#include "../EnemyBase.h"
class Zako3 : public EnemyBase
{
private:
	enum class Zako3State {
		Appearing,
		Floating
	};

private:
	Zako3State state;

	float appear_timer;
	float appear_duration;
	float scale;
	float alpha;
	float rotation;

	bool is_appearing;
	Vector2D velocity;
	float gravity;

	Vector2D base_location;
	float float_timer;

	bool zako3_flip = false;

	Vector2D appear_start_pos;
	Vector2D appear_end_pos;

	bool is_from_left = true; // デフォルトは左から登場
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
	template <typename T>
	T my_min(T a, T b) {
		return (a < b) ? a : b;
	}

	void SetAppearParams(const Vector2D& start_pos, const Vector2D& end_pos, float appear_time = 3.0f, bool from_left = false);
};

