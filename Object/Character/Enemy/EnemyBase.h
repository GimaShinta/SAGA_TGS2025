#pragma once
#include "../CharacterBase.h"
#include "../../../Object/GameObjectManager.h"
#include <unordered_set>
#include "../../../Utility/ScoreData.h"
#include"../../../Object/Character/Player/Player.h"

const int ENEMY_MAX = 100; // 敵機の数の最大値

enum
{
	ENE_NONE,
	ENE_ZAKO1,
	ENE_ZAKO2,
	ENE_ZAKO3,
	ENE_ZAKO4,
	ENE_ZAKO5,
	ENE_BOSS,
	ENE_BOSS2,
};

class EnemyBase : public CharacterBase
{
protected:
	float hp;
	bool on_hit;
	int enemy_type;
private:
	int move_pattern = 0; // 0〜3：移動パターン

protected:
	bool is_shot;
	float shot_timer;
	float delta = 0.0f;
	float beam_damage_timer = 0.0f;
	Player* player;
	int anim_id;

public:
	EnemyBase();
	~EnemyBase();

public:
	virtual void Initialize() override;
	virtual void Update(float delta_second) override;
	virtual void Draw(const Vector2D& screen_offset) const override;
	virtual void Finalize() override;
	virtual void OnHitCollision(GameObjectBase* hit_object) override;

protected:
	virtual void Movement(float delta_second) override;
	virtual void Shot(float delta_second);

public:
	bool GetIsShot() const;
	int GetEnemyType() const;
	void SetIsShot();
	
	void SetPlayer(Player* p);

	bool is_exp_generated = false;
};
