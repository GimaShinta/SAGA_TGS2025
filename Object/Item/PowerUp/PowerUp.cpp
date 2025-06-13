#include "PowerUp.h"
#include "DxLib.h"

void PowerUp::Initialize()
{
    velocity.y = 60.0f;
    box_size = 10.0f;
    z_layer = 1;
    is_mobility = true;

    // 当たり判定のオブジェクト設定
    collision.is_blocking = true;
    // 自分のオブジェクトタイプ
    collision.object_type = eObjectType::ePowerUp;
    // 当たる相手のオブジェクトタイプ
    collision.hit_object_type.push_back(eObjectType::ePlayer);

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images = rm->GetImages("Resource/Image/Object/Item/PowerUp/anime_item31.png", 16, 8, 2, 32, 32);
    image = images[0];
}

void PowerUp::Update(float delta)
{
    std::vector<int> animation_num = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
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


    location += velocity * delta;
}

void PowerUp::Draw(const Vector2D& offset) const
{
    DrawBox(location.x - box_size.x, location.y - box_size.y,
        location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 255), TRUE);

    DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, image, TRUE);
}

void PowerUp::OnHitCollision(GameObjectBase* hit_object)
{
    if (hit_object->GetCollision().object_type == eObjectType::ePlayer)
    {
        this->SetDestroy();
    }
}
