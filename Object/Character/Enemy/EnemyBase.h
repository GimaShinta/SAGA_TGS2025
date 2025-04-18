#pragma once
#include "../CharacterBase.h"
#include "../../../Object/GameObjectManager.h"
#include <unordered_set>

const int ENEMY_MAX = 100; // 敵機の数の最大値

enum
{
	ENE_NONE,
	ENE_ZAKO1,
	ENE_ZAKO2,
	ENE_ZAKO3,
	ENE_BOSS,
	ENE_BOSS2,
};

class EnemyBase : public CharacterBase
{
protected:
	float hp;
	bool on_hit;
	int enemy_type;

protected:
	bool is_shot;
	float shot_timer;
	float delta = 0.0f;
	float beam_damage_timer = 0.0f;

public:
	EnemyBase();
	~EnemyBase();

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

protected:
	/// <summary>
	/// 移動処理
	/// </summary>
	/// <param name="delta_second">１フレーム当たりの時間</param>
	virtual void Movement(float delta_second) override;

	virtual void Shot(float delta_second);

public:
	bool GetIsShot() const;
	int GetEnemyType() const;
	void SetIsShot();

};

