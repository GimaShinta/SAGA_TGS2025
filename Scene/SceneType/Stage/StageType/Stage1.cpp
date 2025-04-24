#include "Stage1.h"
#include "Stage2.h" // 次のステージがある場合

#include <algorithm>


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
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage1.h"
#include <math.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()

Stage1::Stage1(Player* player)
    : StageBase(player),
    zako2(nullptr),
    zako4(nullptr),
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

    // 乱数初期化（1回だけ行う）
    srand(static_cast<unsigned int>(time(NULL)));

}

void Stage1::Finalize()
{
    // 終了処理
        // 敵リストをすべて削除
        for (auto& enemy : enemy_list)
        {
            enemy->SetDestroy();
        }
        enemy_list.clear();

}

void Stage1::Update(float delta)
{
    UpdateBackgroundScroll(delta);

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
   // タイマー2つに分ける
    stage_timer += delta;          // 10秒判定用
    distance_timer += delta;       // スクロール用

    if (distance_timer >= 0.01f)
    {
        if (distance > 0)
        {
            distance--;
        }
        else
        {
            distance = 0;
        }
        distance_timer = 0;
    }

    if (stage_timer >= 50.0f)
    {
        is_clear = true;
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




    if (is_clear == true || is_over == true)
    {
        scene_timer += delta;
        if (scene_timer >= 5.0f)
        {
            finished = true;
        }
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

     DrawScrollBackground();  // ここで scroll_offset を使って描画！

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Draw();

    // 左の黒帯
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // 右の黒帯
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "ゲームメイン", GetColor(255, 255, 255));
    DrawString(0, 300, "操作方法\n\n左スティック\n十字ボタン\nWASDキー : 移動\n\nAボタン\nスペースキー : 発射\n\nBボタン\nBキー : レーザー\n\nRBボタン\nLキー : 射出反転", GetColor(255, 255, 255));
    DrawFormatString(0, 20, GetColor(255, 255, 0), "敵数: %d", enemy_list.size());
    // ステージタイマーの表示（右上）
    DrawFormatString(D_WIN_MAX_X - 200, 20, GetColor(255, 255, 255), "Time: %.1f", stage_timer);


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

    // ステージ経過時間に応じた出現間隔
    float spawn_interval = 3.0f;
    if (stage_timer >= 10.0f) spawn_interval = 2.0f;
    if (stage_timer >= 20.0f) spawn_interval = 1.5f;
    if (stage_timer >= 30.0f) spawn_interval = 1.0f;
    if (stage_timer >= 45.0f) spawn_interval = 0.5f;

    if (enemy_spawn_timer >= spawn_interval)
    {
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

        const int NUM_LANES = 3;
        float lane_y[NUM_LANES] = {
            D_WIN_MAX_Y / 4.0f,         // 上レーン
            D_WIN_MAX_Y / 2.5f,         // 中央レーン
            D_WIN_MAX_Y / 2.0f          // 下レーン
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

        // Zako2を出現させる処理（Stage1の進行に合わせて）
        if (stage_timer >= 5.0f && !zako4_spawned)
        {
            printf("Zako2 is about to appear!\n");

            Vector2D spawn_pos(150.0f, 100.0f);
            zako4 = objm->CreateObject<Zako4>(spawn_pos);
            if (zako4)
            {
                zako4->SetVelocity(Vector2D(40.0f, 0.0f));
                enemy_list.push_back(zako4);
                zako4_spawned = true;
            }
        }

        // タイマーリセット
        enemy_spawn_timer = 0.0f;
    }
}




void Stage1::EnemyShot(float delta_second)
{
    // ステージタイマーが半分（15秒）を過ぎていない場合は攻撃させない
    if (stage_timer < 15.0f) return;

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    for (int i = 0; i < enemy_list.size(); i++)
    {
        if (enemy_list[i]->GetIsShot())
        {
            int enemy_type = enemy_list[i]->GetEnemyType();

            if (enemy_type == ENE_ZAKO1)
            {
                Vector2D e_location = enemy_list[i]->GetLocation();
                objm->CreateObject<EnemyShot1>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                objm->CreateObject<EnemyShot1>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                enemy_list[i]->SetIsShot();
            }
            else if (enemy_type == ENE_ZAKO2)
            {
                Vector2D b = player->GetLocation() - enemy_list[i]->GetLocation();
                float c = sqrt(pow(b.x, 2) + pow(b.y, 2));

                e_shot2 = objm->CreateObject<EnemyShot2>(enemy_list[i]->GetLocation());
                e_shot2->SetVelocity(Vector2D(b.x / c, b.y / c));
                enemy_list[i]->SetIsShot();
            }
        }
    }
}


//スクロール描画
void Stage1::DrawScrollBackground() const
{
    // 背景色（明るめの紺）
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(20, 20, 40), TRUE);

    // 背面グリッド（遠く・細かく・線細く）
    const int grid_size_back = 40;
    const int alpha_back = 100;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_back);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, 100, 255)); // ネオンブルー（薄）

    //画像だとここを変更
    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back)
    {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(0, 100, 255));
    }

    /**     二枚目     **/

    // 手前グリッド（近く・広め・線太め）
    const int grid_size_front = 80;
    const int alpha_front = 120;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_front);

    // 太いライン用の四角（縦ライン）
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(180, 0, 255), TRUE); // 手前は太めパープル

    //画像だとここを変更
    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(180, 0, 255), TRUE); // 太線で遠近感
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

