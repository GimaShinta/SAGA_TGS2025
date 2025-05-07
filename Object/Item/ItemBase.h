#pragma once
#include "../GameObjectBase.h"
class ItemBase : public GameObjectBase
{
public:
	// 初期化処理
	virtual void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">１フレーム当たりの時間</param>
	/// <returns></returns>
	virtual void Update(float delata_second);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="delta_second"></param>
	/// <returns></returns>
	virtual void Draw(const Vector2D& screen_offset)const;

	// 終了時処理
	virtual void Finalize();

public:
	/// <summary>
	/// ヒットしたときの処理
	/// </summary>
	/// <param name="hit_object">プレイヤーだとしたら、ここは敵とかブロックと当たったことになる</param>
	virtual void OnHitCollision(GameObjectBase* hit_object);
};

