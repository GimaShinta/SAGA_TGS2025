#include "Zako6.h"
#include "../../../../Utility/ProjectConfig.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

#include <cmath>

Zako6::Zako6()
{
}

Zako6::~Zako6()
{
}

void Zako6::Initialize()
{
    enemy_type = ENE_ZAKO3;
    z_layer = 3;
    box_size = 12;
    hp = 10;

    //collision.is_blocking = true;
    //collision.object_type = eObjectType::eEnemy;
    //collision.hit_object_type.push_back(eObjectType::eShot);
    //collision.hit_object_type.push_back(eObjectType::eBeam);

    collision.is_blocking = false;
    collision.object_type = eObjectType::eNone;
    collision.hit_object_type.clear();

    is_mobility = true;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    zako2_images = rm->GetImages("Resource/Image/Object/Enemy/Zako3/anime_enemy43.png", 4, 4, 1, 104, 72);
    zako3_images = rm->GetImages("Resource/Image/Object/Enemy/Zako3/anime_enemy41.png", 4, 4, 1, 104, 80);
    zako7_image = rm->GetImages("Resource/Image/Object/Enemy/Zako4/enemy94.png")[0];

    enemy_jet = rm->GetImages("Resource/Image/Object/Player/Shot/anime_effect17.png", 6, 6, 1, 8, 88);
    jet = enemy_jet[4];

    //sound_destroy = rm->GetSounds("Resource/sound/se/se_effect/kill_4.mp3");
    //ChangeVolumeSoundMem(255 * 100 / 100, sound_destroy);

    // 共通パラメータ
    scale_min = 0.3f;
    scale_max = 0.8f;
    appear_duration = 2.0f;
    descend_duration = 1.0f;

    start_pos = Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y + 50);
    target_pos = Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2 - 50);
    descend_pos = Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2);

    location = start_pos;
    scale = scale_min;
    alpha = 0;
    rotation = 0.0f;
    state = ZakoState::Appearing;
    float_timer = 0.0f;
    appear_timer = 0.0f;

    mode = ZakoMode::Zako2;
}

void Zako6::SetMode(ZakoMode new_mode)
{
    mode = new_mode;
}

void Zako6::ShootToPlayer(float speed)
{
    GameObjectManager* gm = Singleton<GameObjectManager>::GetInstance();
    GameObjectBase* shot = gm->CreateObject<EnemyShot2>(location); // ←使用中の弾クラスに変更
    if (shot && player)
    {
        Vector2D dir = player->GetLocation() - location;
        dir.Normalize();
        shot->SetVelocity(dir * speed);
    }
}

void Zako6::Update(float delta_second)
{
    float_timer += delta_second;

    if (mode == ZakoMode::Zako2) {
        switch (state)
        {
        case ZakoState::Appearing: {
            float t = my_min(float_timer / appear_duration, 1.0f);
            float ease_t = t * t * (3 - 2 * t);

            Vector2D prev = location;
            location = start_pos + (target_pos - start_pos) * ease_t;
            velocity = (location - prev) / delta_second;

            scale = scale_min + (0.1f * (scale_max - scale_min)) * ease_t;
            alpha = 255;
            rotation += delta_second * (1.5f * (1.0f - t));

            if (t >= 1.0f) {
                AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot); // ←仮のSE名

                ShootToPlayer(0.003f); // ← 弾速を適切に設定
                float_timer = 0.0f;
                state = ZakoState::Descending;
            }
            break;
        }
        case ZakoState::Descending: {
            float t = my_min(float_timer / descend_duration, 1.0f);
            float ease_t = 1 - powf(1 - t, 3);

            Vector2D prev = location;
            location = target_pos + (descend_pos - target_pos) * ease_t;
            velocity = (location - prev) / delta_second;

            scale = (scale_min + 0.1f * (scale_max - scale_min)) + ((scale_max - scale_min) * 0.9f) * ease_t;
            rotation *= (1.0f - t);

            if (t >= 1.0f) {
                float_timer = 0.0f;
                state = ZakoState::Floating;
                base_location = descend_pos;
                rotation = 0.0f;
            }
            break;
        }
        case ZakoState::Floating: {
            location.x = base_location.x + sinf(float_timer * 1.5f) * 10.0f;
            location.y = base_location.y + sinf(float_timer * 2.0f) * 5.0f;

            if (float_timer >= 3.0f) {
                state = ZakoState::Leaving;
                float_timer = 0.0f;
            }
            break;
        }
        case ZakoState::Leaving: {
            location.y -= delta_second * 300.0f;  // 300px/secで上に移動
            if (location.y + box_size.y < 0) {
                is_destroy = true;  // 画面外に出たら削除
            }
            break;
        }
        }
    }
    else if (mode == ZakoMode::Zako3) {
        switch (state)
        {
        case ZakoState::Appearing: {
            appear_timer += delta_second;
            float t = my_min(appear_timer / 1.2f, 1.0f);
            float ease_t = 1 - powf(1 - t, 3);

            location = start_pos + (target_pos - start_pos) * ease_t;
            location.y += 150.0f * sinf(t * DX_PI);
            alpha = static_cast<int>(255 * ease_t);
            rotation += delta_second * (1.0f - t) * (is_from_left ? +1.0f : -1.0f);
            scale = 5.0f - 4.2f * ease_t;

            if (t >= 1.0f) {
                AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot); // ←仮のSE名

                ShootToPlayer(0.003f);
                state = ZakoState::Floating;
                base_location = location;
                float_timer = 0.0f;
                scale = scale_max;
                alpha = 255;
                rotation = 0.0f;
            }
            break;
        }
        case ZakoState::Floating: {
            location.x = base_location.x + sinf(float_timer * 1.5f) * 10.0f;
            location.y = base_location.y + sinf(float_timer * 2.0f) * 5.0f;

            if (float_timer >= 3.0f) {
                state = ZakoState::Leaving;
                float_timer = 0.0f;
            }
            break;
        }
        case ZakoState::Leaving: {
            location.y -= delta_second * 300.0f;
            if (location.y + box_size.y < 0) {
                is_destroy = true;
            }
            break;
        }
        }
    }
    else if (mode == ZakoMode::Zako7) {
        switch (state)
        {
        case ZakoState::Appearing: {
            float t = my_min(float_timer / appear_duration, 1.0f);
            float ease_t = t * t * (3 - 2 * t);

            Vector2D prev = location;
            location = start_pos + (target_pos - start_pos) * ease_t;
            velocity = (location - prev) / delta_second;

            scale = scale_min + (scale_max - scale_min) * ease_t * 0.2f;
            alpha = static_cast<int>(255 * ease_t);

            if (t >= 1.0f) {
                float_timer = 0.0f;
                base_location = location;
                state = ZakoState::Redirecting;
            }
            break;
        }

        case ZakoState::Redirecting: {
            float t = my_min(float_timer / 1.0f, 1.0f);
            float ease_t = 1 - powf(1 - t, 3);

            // Y方向にだけ移動する（Xはそのまま）
            location.x = base_location.x;
            location.y = base_location.y - 150.0f * ease_t;  // ← ゆっくり上へ（50px上昇）

            scale = scale_min + (0.6 - scale_min) * ease_t;

            if (t >= 1.0f) {
                AnimationManager::GetInstance()->PlaySE(SE_NAME::EnemyShot); // ←仮のSE名

                ShootToPlayer(0.003f);
                float_timer = 0.0f;
                state = ZakoState::Floating;
                base_location = location;
            }
            break;
        }

        case ZakoState::Floating: {
            location.x = base_location.x + sinf(float_timer * 1.5f) * 10.0f;
            location.y = base_location.y + sinf(float_timer * 2.0f) * 5.0f;

            if (float_timer >= 3.0f) {
                state = ZakoState::Leaving;
                float_timer = 0.0f;
            }
            break;
        }

        case ZakoState::Leaving: {
            location.y -= delta_second * 300.0f;
            if (location.y + box_size.y < 0) {
                is_destroy = true;
            }
            break;
        }
    }
}

    // 出現中は無敵、浮遊状態になったら当たり判定ON
    if (state == ZakoState::Floating || state == ZakoState::Leaving) 
    {
        collision.is_blocking = true;
        collision.object_type = eObjectType::eEnemy;
        collision.hit_object_type = { eObjectType::eShot, eObjectType::eBeam };
    }
    else
    {
        collision.is_blocking = false;
        collision.object_type = eObjectType::eNone;
        collision.hit_object_type.clear();
    }

    if (hp <= 0)
    {
        is_destroy = true;

        DropItems();
        AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
        manager->PlaySE(SE_NAME::Destroy);
        anim_id = manager->PlayerAnimation(EffectName::eExprotion2, location, 0.035f, false);
        manager->SetScale(anim_id, 0.5f);

        Singleton<ScoreData>::GetInstance()->SetScoreData(500);
    }

    std::vector<int> animation_num;
    switch (mode)
    {
    case ZakoMode::Zako2:
        animation_num = { 0, 1, 2, 3 };
        break;
    case ZakoMode::Zako3:
        animation_num = { 0, 1, 2, 3 };
        break;
    case ZakoMode::Zako7:
        animation_num = { 0 };
        break;
    default:
        animation_num = { 0 };
        break;
    }

    //フレームレートで時間を計測
    animation_time += delta_second;
    //8秒経ったら画像を切り替える
    if (animation_time >= 0.1f)
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
        switch (mode)
        {
        case ZakoMode::Zako2:
            image = zako2_images[animation_num[animation_count]];
            break;
        case ZakoMode::Zako3:
            image = zako3_images[animation_num[animation_count]];
            break;
        case ZakoMode::Zako7:
            image = zako7_image;
            break;
        default:
            image = zako7_image;
            break;
        }
        // アニメーションが順番に代入される
    }


    std::vector<int> engen_num = { 3, 4, 5, 4 };
    //フレームレートで時間を計測
    jet_time += delta_second;
    //8秒経ったら画像を切り替える
    if (jet_time >= 0.02f)
    {
        //計測時間の初期化
        jet_time = 0.0f;
        //時間経過カウントの増加
        jet_count++;
        //カウントがアニメーション画像の要素数以上になったら
        if (jet_count >= engen_num.size())
        {
            //カウントの初期化
            jet_count = 0;
        }
        // アニメーションが順番に代入される
        jet = enemy_jet[engen_num[jet_count]];
    }


    __super::Update(delta_second);
}

void Zako6::Draw(const Vector2D& screen_offset) const
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    //switch (mode)
    //{
    //case ZakoMode::Zako2:
    //    DrawRotaGraph(location.x - 10, location.y - 80, scale + 0.5f, 3.14 / 1, jet, TRUE);
    //    DrawRotaGraph(location.x + 10, location.y - 80, scale + 0.5f, 3.14 / 1, jet, TRUE);
    //    break;
    //case ZakoMode::Zako3:
    //    DrawRotaGraph(location.x - 10, location.y - 80, scale + 0.5f, 3.14 / 1, jet, TRUE);
    //    DrawRotaGraph(location.x + 10, location.y - 80, scale + 0.5f, 3.14 / 1, jet, TRUE);
    //    break;
    //case ZakoMode::Zako7:
    //    DrawRotaGraph(location.x, location.y - 20, scale + 0.5f, 3.14 / 1, jet, TRUE);
    //    break;
    //default:
    //    DrawRotaGraph(location.x, location.y - 20, scale + 0.5f, 3.14 / 1, jet, TRUE);
    //    break;
    //}
    DrawRotaGraph(location.x, location.y, scale, 0.0f, image, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Zako6::Finalize()
{
}

void Zako6::Shot(float delta_second)
{
}

void Zako6::SetFlip(bool flip)
{
    //zako3_flip = flip;
}

void Zako6::SetAppearParams(const Vector2D& start_pos_param, const Vector2D& end_pos_param, float appear_time, bool from_left)
{
    start_pos = start_pos_param;
    target_pos = end_pos_param;
    descend_pos = Vector2D(end_pos_param.x, end_pos_param.y + 50.0f); // Zako4は使わない

    location = start_pos;
    appear_duration = appear_time;
    float_timer = 0.0f;
    appear_timer = 0.0f;

    scale = (mode == ZakoMode::Zako2) ? scale_min :
        (mode == ZakoMode::Zako3) ? 3.0f : scale_min;

    alpha = 0;
    rotation = 0.0f;

    is_from_left = from_left;

    state = ZakoState::Appearing;
}
