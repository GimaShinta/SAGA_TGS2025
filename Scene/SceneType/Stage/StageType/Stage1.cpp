#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"  

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

#include"../../../../Object/Item/Exp/Exp.h"

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

    /*遷移時間*/
    if (stage_timer >= 60.0f)
    {
        is_clear = true;
    }


    // 敵の出現
    EnemyAppearance(delta);

  

    // 敵が倒された時は経験値を生成。
    for (auto& enemy : enemy_list)
    {
        if (enemy->is_destroy == true)
        {
            Exp* exp = Singleton<GameObjectManager>::GetInstance()->CreateObject<Exp>(enemy->GetLocation());
            exp->SetPlayer(player);
        }
    }

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
        // 敵全削除（既に削除されているものは何も起きない）
        for (auto& enemy : enemy_list)
        {
            enemy->SetDestroy();
        }

        enemy_list.clear(); // 管理リストもクリア

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

void Stage1::EnemyAppearance(float delta)
{
    // タイマー加算
    enemy_spawn_timer += delta;

    const float spawn_interval = 3.0f;

    // 5秒ごとに1体だけ出現
    if (enemy_spawn_timer >= spawn_interval)
    {
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

        Vector2D spawn_pos(400.0f, 100.0f);
        zako1 = objm->CreateObject<Zako1>(spawn_pos);
        enemy_list.push_back(zako1);
        zako1->SetPlayer(player);

        // パターンを時間に応じて切り替え
        if (stage_timer < 30.0f)
        {
            zako1->SetPattern(Zako1Pattern::MoveStraight); // 前半
        }
        else
        {
            zako1->SetPattern(Zako1Pattern::MoveZigzag);   // 後半
        }

        // タイマーリセットして次の出現に備える
        enemy_spawn_timer = 0.0f;
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

