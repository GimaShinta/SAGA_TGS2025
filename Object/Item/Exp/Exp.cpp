#include "Exp.h"
#include "DxLib.h"
#include "../../Character/Player/Player.h"
#include "../../GameObjectManager.h"


Exp::Exp() : 
    attract_range(250.0f),  //追従をONにする距離
    speed(1.0f),            //初期スピード
    max_speed(200.0f),      //最大スピード
    acceleration(100.0f),   //加速度(一秒あたりの増加量)
    lifetime(5.0f),         //自動削除する秒数
    is_attracting(false)    //当たっているか
{
}

Exp::~Exp()
{
}

// 初期化処理
void Exp::Initialize()
{
    // 当たり判定設定
    collision.is_blocking = true;                         // 接触判定有効
    collision.object_type = eObjectType::eExp;            // 自分の種類はExp
    collision.hit_object_type.push_back(eObjectType::ePlayer); // プレイヤーと衝突判定

    box_size = 6.0f;             // 経験値アイテムのサイズ（半径）
    z_layer = 1;                 // 描画レイヤー（前面に表示）

    is_mobility = true;          // 当たり判定で移動判定有効
    is_attracting = false;       // 吸収モードOFFで開始
    velocity = Vector2D(0, 50.0f); // 初期落下（ふわっと下へ）

}

// 更新処理
void Exp::Update(float delta)
{
    if (!player) 
    {
        return; // プレイヤーが未設定なら処理しない
    }

    //プレイヤー情報：位置情報
    Vector2D to_player = player->GetLocation() - location;

    // 距離の2乗で吸収開始を判定
    float dist_sq = to_player.x * to_player.x + to_player.y * to_player.y;
    float attract_range_sq = attract_range * attract_range;

    if (!is_attracting && dist_sq < attract_range_sq)
    {
        // 吸収開始
        is_attracting = true; 
    }

    if (is_attracting)
    {
        // 吸収モード中：加速してプレイヤーに向かう
        to_player.Normalize();

        speed += acceleration * delta;   // 徐々に加速
        
        if (speed > max_speed) 
        {
            speed = max_speed; // 最大スピード制限
        }
        velocity = to_player * speed;
    }

    // 現在の速度で位置更新
    location += velocity * delta;

    // 寿命減少（自動消滅処理）
    lifetime -= delta;
    if (lifetime <= 0.0f)
    {
        this->SetDestroy(); // 5秒後に削除
        return;
    }

    // 画面外に落ちたら削除
    if (location.y > D_WIN_MAX_Y + 200)
    {
        this->SetDestroy();
        return;
    }
}

// 描画処理
void Exp::Draw(const Vector2D& offset) const
{
    // 外側：水色（グロー感）
    DrawCircle(location.x, location.y, (int)box_size.x, GetColor(0, 255, 255), TRUE);

    // 内側：黄色
    DrawCircle(location.x, location.y, (int)box_size.x - 2.0f, GetColor(255, 255, 0), TRUE);
}

// 衝突処理
void Exp::OnHitCollision(GameObjectBase* hit_object)
{
    // プレイヤーと当たったら
    if (hit_object->GetCollision().object_type == eObjectType::ePlayer)
    {
        if (player && !player->GetBeamOn())  // ビーム中でなければ加算
        {
            player->AddCharge(5.0f);  // 調整可能
        }
        this->SetDestroy(); // 吸収 → 削除
    }
}
