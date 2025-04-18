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
	/// <summary>
	/// ヒット時処理
	/// </summary>
	/// <param name="hit_object">当たった相手</param>
	virtual void OnHitCollision(GameObjectBase* hit_object) override;

	void SetBeamFlip(bool flip);
	void SetPlayer(class Player* p_player);
};

