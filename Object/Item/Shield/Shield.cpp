#include "Shield.h"
#include "../../../Utility/InputManager.h"
#include "../../../Utility/ScoreData.h"

void Shield::Initialize()
{
    velocity.y = 60.0f;
    box_size = 20.0f;
    z_layer = 1;
    is_mobility = true;

    // 当たり判定のオブジェクト設定
    collision.is_blocking = true;
    // 自分のオブジェクトタイプ
    collision.object_type = eObjectType::eShield;
    // 当たる相手のオブジェクトタイプ
    collision.hit_object_type.push_back(eObjectType::ePlayer);

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images = rm->GetImages("Resource/Image/Object/Item/PowerUp/anime_item32.png", 12, 6, 2, 32, 32);
    image = images[0];
    se = rm->GetSounds("Resource/sound/se/effect/audiostock_1133382.mp3");
    ChangeVolumeSoundMem(255 * 100 / 100, se);
}

void Shield::Update(float delta)
{
    InputManager* input = Singleton<InputManager>::GetInstance();

    std::vector<int> animation_num = { 6, 7, 8, 9, 10, 11 };
    //フレームレートで時間を計測
    animation_time += delta;
    //8秒経ったら画像を切り替える
    if (animation_time >= 0.03f)
    {
        //計測時間の初期化
        animation_time = 0.0f;
        //時間経過カウントの増加
        animation_count++;
        //カウントがアニメーション画像の要素数以上になったら
        if (animation_count >= animation_num.size())
        {
            //カウントの初期化
            animation_count = 0;
        }
        // アニメーションが順番に代入される
        image = images[animation_num[animation_count]];
    }

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
}

void Shield::Draw(const Vector2D& offset) const
{
    //DrawBox(location.x - box_size.x, location.y - box_size.y,
    //    location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);

    DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, image, TRUE);

}

void Shield::OnHitCollision(GameObjectBase* hit_object)
{
    if (hit_object->GetCollision().object_type == eObjectType::ePlayer)
    {
        PlaySoundMem(se, DX_PLAYTYPE_BACK);
        Singleton<ScoreData>::GetInstance()->AddScore(GetRand(100) + 50);
        this->SetDestroy();
    }
}
