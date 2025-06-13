#pragma once
#include "../ItemBase.h"
#include "../../../Utility/ProjectConfig.h"

class Exp : public ItemBase
{
private:
    std::vector<int> images;

    float attract_range;   //追従をONにする距離
    float speed;           // 初期スピード
    float max_speed;       // 最大スピード
    float acceleration;    // 加速度（1秒あたりの増加量）
    float lifetime;        //5秒で自動削除

    bool is_attracting;    //当たっているか
    int se;

public:
    Exp();
    ~Exp();

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize() override; 

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="delta"></param>
    void Update(float delta) override;

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="offset"></param>
    void Draw(const Vector2D& offset) const override;

    /// <summary>
    /// 当たり判定処理
    /// </summary>
    /// <param name="ヒットオブジェクト"></param>
    void OnHitCollision(GameObjectBase* hit_object) override;

};
