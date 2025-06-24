#pragma once
#include "EnemyShotBase.h"

class EnemyShot1 : public EnemyShotBase
{
public:
	EnemyShot1();
	~EnemyShot1();

	virtual void Initialize() override;
	virtual void Update(float delta_second) override;
	virtual void Draw(const Vector2D& screen_offset) const override;
	virtual void Finalize() override;

	void SetOmni(bool flag)
	{
		is_omni = flag;
	}

private:
	bool is_omni = false; // ‘S•ûŒü’e‚©‚Ç‚¤‚©
	int life_time;
};
