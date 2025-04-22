#include "Stage1.h"
#include "Stage2.h" // 次のステージがある場合

#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot3.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako3.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage1.h"
#include <math.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()

Stage1::Stage1(Player* player)
    : StageBase(player),
    zako2(nullptr),
    boss(nullptr),
    e_shot1(nullptr),
    e_shot2(nullptr),
    enemy_spawn_timer(0.0f)
{}

Stage1::~Stage1()
{}

void Stage1::Initialize()
{
    // 初期化処理
    distance = STAGE_DISTANCE;
}

void Stage1::Finalize()
{
    // 終了処理
}

void Stage1::Update(float delta)
{
    // 敵が画面外に出た場合に削除
    for (auto it = enemy_list.begin(); it != enemy_list.end(); )
    {
        Vector2D enemy_pos = (*it)->GetLocation();

        if (enemy_pos.x < 100 || enemy_pos.x > 1100)
        {
            (*it)->SetDestroy();
            it = enemy_list.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // タイマーをカウント
    timer++;

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    // プレイヤーが弾を打つ準備ができていたら弾を生成
    if (player->GetIsShot())
    {
        Vector2D p_location = player->GetLocation();
        player->SetIsShot();

        if (player->GetShotFlip() == false)
        {
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
            shot->SetShotFlip(false);
        }
        else
        {
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x, p_location.y + D_OBJECT_SIZE));
            shot->SetShotFlip(true);
        }
    }
    // プレイヤーがビームをうつ準備ができていたらビームを生成
    else if (player->GetBeamOn())
    {
        Vector2D p_location = player->GetLocation();

        if (player->GetShotFlip() == false)
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y - D_OBJECT_SIZE) - 848));
            beam->SetBeamFlip(false);
        }
        else
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y + D_OBJECT_SIZE) + 848));
            beam->SetBeamFlip(true);
        }

        beam->SetPlayer(player);
        player->SetBeamOn();
    }

    // delta_second 分加算
    stage_timer += delta;

    if (stage_timer >= 0.01f)
    {
        if (distance > 0)
        {
            distance--;
        }
        else
        {
            distance = 0;
        }
        stage_timer = 0;
    }

    // 敵の出現
    EnemyAppearance();

    // 敵の攻撃
    EnemyShot(delta);

    // ゲームクリア・オーバー判定
    if (boss && !boss->GetIsAlive() && !is_over)
    {
        boss->SetDestroy();
        is_clear = true;
    }

    if (boss2 && !boss2->GetIsAlive() && !is_over)
    {
        boss2->SetDestroy();
        is_clear = true;
    }

    if (player && !player->GetIsAlive() && !is_clear)
    {
        player->SetDestroy();
        is_over = true;
    }


    //stage遷移
    stage_timer += delta;

    if (stage_timer >= 10.0f)
    {
        finished = true;
    }


    // 仮の条件：スペースキーを押したらステージ終了
    if (CheckHitKey(KEY_INPUT_N))
    {
        finished = true;
    }
}

void Stage1::Draw()
{
    // 背景を白で塗る（最初に描画）
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Draw();

    // 左の黒帯
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // 右の黒帯
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "ゲームメイン", GetColor(255, 255, 255));
    DrawString(0, 300, "操作方法\n\n左スティック\n十字ボタン\nWASDキー : 移動\n\nAボタン\nスペースキー : 発射\n\nBボタン\nBキー : レーザー\n\nRBボタン\nLキー : 射出反転", GetColor(255, 255, 255));
    DrawFormatString(0, 20, GetColor(255, 255, 0), "敵数: %d", enemy_list.size());

    // ステージ描画
    if (stage <= 1)
    {
        if (is_warning)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, brend);
            DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(255, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        }
    }
    else if (stage >= 2)
    {
        DrawBox((D_WIN_MAX_X / 2) - 250, 0, (D_WIN_MAX_X / 2) + 250, D_WIN_MAX_Y, GetColor(100, 100, 100), TRUE);
    }

    if (is_clear)
    {
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "ゲームクリア", GetColor(0, 0, 0));
    }
    else if (is_over)
    {
        DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "ゲームオーバー", GetColor(0, 0, 0));
    }
}

bool Stage1::IsFinished()
{
    return finished;
}

bool Stage1::IsClear()
{
    return is_clear;
}

bool Stage1::IsOver()
{
    return is_over;
}

StageBase* Stage1::GetNextStage(Player* player)
{
    return new Stage2(player); // 次のステージへ
}

void Stage1::EnemyAppearance()
{
    enemy_spawn_timer += 1.0f / 60.0f; // 1フレームごとに加算（60FPS想定）

    if (enemy_spawn_timer >= 30.0f)
    {
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

        const int NUM_LANES = 3;
        float lane_y[NUM_LANES] = {
            D_WIN_MAX_Y / 8.0f,         // 上レーン
            D_WIN_MAX_Y / 4.0f,         // 中央レーン
            (D_WIN_MAX_Y / 8.0f) * 3.0f // 下レーン（上半分まで）
        };

        bool lane_occupied[NUM_LANES] = { false, false, false };

        // 現在の敵の位置からレーン占有状態をチェック
        for (const auto& enemy : enemy_list)
        {
            float e_y = enemy->GetLocation().y;
            for (int i = 0; i < NUM_LANES; i++)
            {
                if (fabs(e_y - lane_y[i]) < 30.0f) // 30px以内にいるなら同一レーン
                {
                    lane_occupied[i] = true;
                }
            }
        }

        // 使用可能なレーンを収集
        std::vector<int> available_lanes;
        for (int i = 0; i < NUM_LANES; i++)
        {
            if (!lane_occupied[i])
                available_lanes.push_back(i);
        }

        // 出現可能なレーンがあるか確認
        if (!available_lanes.empty())
        {
            int chosen_index = rand() % available_lanes.size(); // 使用可能なレーンの中からランダム
            int lane_id = available_lanes[chosen_index];
            float spawn_y = lane_y[lane_id];

            int random_direction = rand() % 2; // 左からか右からか
            float base_x = (random_direction == 0)
                ? (rand() % ((D_WIN_MAX_X / 2) - 400))         // 左
                : ((D_WIN_MAX_X / 2) + 400 + rand() % (D_WIN_MAX_X - ((D_WIN_MAX_X / 2) + 400))); // 右

            Vector2D velocity(random_direction == 0 ? 60 : -60, 0);

            for (int i = 0; i < 4; i++)
            {
                float offset = i * 50.0f; // 横に50pxずつずらして配置
                float spawn_x = (random_direction == 0)
                    ? base_x + offset
                    : base_x - offset;

                auto enemy = objm->CreateObject<Zako1>(Vector2D(spawn_x, spawn_y));
                enemy->SetVelocity(velocity);
                enemy_list.push_back(enemy);
            }
        }

        enemy_spawn_timer = 0.0f;
    }
}



void Stage1::EnemyShot(float delta_second)
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // 敵が弾を打つ準備ができていたら弾を発射する
    for (int i = 0; i < enemy_list.size(); i++)
    {
        if (enemy_list[i]->GetIsShot())
        {
            int enemy_type = enemy_list[i]->GetEnemyType();

            // 敵が雑魚１だったら通常弾を発射する
            if (enemy_type == ENE_ZAKO1)
            {
                Vector2D e_location = enemy_list[i]->GetLocation();
                objm->CreateObject<EnemyShot1>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                objm->CreateObject<EnemyShot1>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                enemy_list[i]->SetIsShot();
            }
            // 敵が雑魚２だったらプレイヤーを狙った弾を発射する
            else if (enemy_type == ENE_ZAKO2)
            {
                // テキの位置からプレイヤーへのベクトルを求める
                Vector2D b = player->GetLocation() - enemy_list[i]->GetLocation();
                float c = sqrt(pow(b.x, 2) + pow(b.y, 2));

                // プレイヤーに向かって弾を打つ
                e_shot2 = objm->CreateObject<EnemyShot2>(enemy_list[i]->GetLocation());
                e_shot2->SetVelocity(Vector2D(b.x / c, b.y / c));
                enemy_list[i]->SetIsShot();
            }
            // ボスの攻撃パターン
            else if (enemy_type == ENE_BOSS)
            {
                Vector2D e_location = enemy_list[i]->GetLocation();
                objm->CreateObject<EnemyShot3>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                objm->CreateObject<EnemyShot3>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                enemy_list[i]->SetIsShot();
            }
        }
    }
}
