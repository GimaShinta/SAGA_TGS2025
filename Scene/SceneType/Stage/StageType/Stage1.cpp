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
#include "../../../../Object/Character/Enemy/EnemyType/Zako3.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Stage1BOSS.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage1.h"

#include"../../../../Object/Item/Exp/Exp.h"
#include"../../../../Utility/AnimationManager.h"
#include <math.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()

Stage1::Stage1(Player* player)
    : StageBase(player),
    zako2(nullptr),
    zako4(nullptr),
    boss(nullptr),
    boss1(nullptr),
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

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    bg_image = rm->GetImages("Resource/Image/BackGround/Main/Stage1/bg_01.png")[0];
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

        //BGMを止める
        //StopSoundMem(bgm);
}

void Stage1::Update(float delta)
{   
    UpdateBackgroundScroll(delta);

    // 敵が画面外に出た場合に削除
    for (auto it = enemy_list.begin(); it != enemy_list.end(); )
    {
        Vector2D enemy_pos = (*it)->GetLocation();

        if (enemy_pos.x < -100 || enemy_pos.x > 1100 || enemy_pos.y > 720)
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
    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->Update(delta);
   


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

    //スクロール
    scroll_y += 400.0f * delta;

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
    if (stage_timer >= 130.0f)
    {
        is_clear = true;
    }

    if (player->life == -1)
    {
        is_over = true; 
    }

    // 敵の出現
    EnemyAppearance(delta);

  

    // 敵が倒された時は経験値を生成。
    for (auto& enemy : enemy_list)
    {
        if (enemy->is_destroy == true && !enemy->is_exp_generated)
        {
            Exp* exp = Singleton<GameObjectManager>::GetInstance()->CreateObject<Exp>(enemy->GetLocation());
            exp->SetPlayer(player);
            enemy->is_exp_generated = true;
        }
    }

    // ボスが倒れたらクリア
    if (boss1 != nullptr && boss1->GetIsAlive() == false && is_over == false)
    {
        boss1->SetDestroy();
        is_clear = true;
    }

    if(boss1 != nullptr )

    if (player && !player->GetIsAlive() && !is_clear)
    {
        player->SetDestroy();
        is_over = true;
    }


    if (is_clear == true || is_over == true)
    {
        for (auto& enemy : enemy_list) {
            enemy->SetDestroy();
        }
        enemy_list.clear();

        // フェードアウト完了かつ少し待機したら終了
        scene_timer += delta;
        if (fade_alpha >= 255.0f && scene_timer >= 1.5f)
        {
            finished = true;
        }
    }

    // 仮の条件：N押したらステージ終了
    if (CheckHitKey(KEY_INPUT_N))
    {
        finished = true;
    }


    UpdateFade(delta);
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

    DrawString(0, 0, "STAGE1", GetColor(255, 255, 255));
    DrawFormatString(0, 50, GetColor(255, 255, 0), "Enemy: %d", enemy_list.size());
    // 右側操作説明パネル
    int panel_x = D_WIN_MAX_X - 270;
    int panel_y = 60;
    int panel_w = 250;
    int panel_h = 180;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);



    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->Draw();

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
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "NEXT STAGE...", GetColor(255, 255, 255));
    }
    else if (is_over)
    {
        DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "ゲームオーバー", GetColor(255, 255, 255));
    }

    // ステージタイマーの表示（右上）
    DrawFormatString(0, 20, GetColor(255, 255, 255), "Time: %.1f", stage_timer);

    DrawFadeOverlay();
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
    enemy_spawn_timer += delta;

    // 経過時間に応じてスポーン間隔を調整（最低0.5秒）
    float spawn_interval = 2.0f - (stage_timer / 5.0f);
    if (spawn_interval < 0.5f)
    {
        spawn_interval = 0.5f;
    }

    if (enemy_spawn_timer >= spawn_interval)
    {
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

        if (stage_timer < 24.0f)
        {
            // ランダムレーンにZako1を1体出現
            const int num_lanes = 7;
            const int lane_x[num_lanes] = { 250, 350, 450, 550, 650, 750, 850 };
            int lane_index = GetRand(num_lanes);
            float x = static_cast<float>(lane_x[lane_index]);
            Vector2D spawn_pos(x, 0.0f);
            Zako* zako = objm->CreateObject<Zako>(spawn_pos);
            zako->SetPattern(ZakoPattern::MoveStraight);
            zako->SetPlayer(player);
            enemy_list.push_back(zako);


        }

        else if(stage_timer < 25.0f)
        {} //インターバル

        else if (stage_timer < 48.0f)
        {
            // ランダムなY位置で左右からZako1が来る + 一定条件でZako4も出現
            const int num_lanes = 3;
            const int lane_y[num_lanes] = { 100, 220, 340 };
            int lane_index_y = GetRand(num_lanes);
            float y = static_cast<float>(lane_y[lane_index_y]);

            bool from_left = fmod(stage_timer, 20.0f) < 10.0f;
            Vector2D spawn_pos = from_left ? Vector2D(200.0f, y) : Vector2D(1000.0f, y);
            ZakoPattern pattern = from_left ? ZakoPattern::RightMove : ZakoPattern::LeftMove;

            Zako* zako = objm->CreateObject<Zako>(spawn_pos);
            zako->SetPattern(pattern);
            zako->SetPlayer(player);
            enemy_list.push_back(zako);

            if (stage_timer >= 20.0f && !zako4_spawned)
            {
                Zako4* zako4 = objm->CreateObject<Zako4>(Vector2D(60, 20));
                zako4->SetPlayer(player);
                enemy_list.push_back(zako4);

                zako4_spawned = true;
            }

        }

        else if(stage_timer < 49.0f)
        {} //インターバル

        else if (stage_timer < 74.0f)
        {
            // ランダムレーンにZako1を1体出現
            const int num_lanes = 6;
            const int lane_x[num_lanes] = {350, 450, 550, 650, 750, 850 };
            int lane_index = GetRand(num_lanes);
            float x = static_cast<float>(lane_x[lane_index]);
            Vector2D spawn_pos(x, 0.0f);
            Zako* zako = objm->CreateObject<Zako>(spawn_pos);

            // ランダムにパターンを選ぶ
            int r = GetRand(2); // 0か1をランダムで取得
            if (r == 0)
                zako->SetPattern(ZakoPattern::MoveAndStopShoot);
            else
                zako->SetPattern(ZakoPattern::DiveOnce);

            zako->SetPlayer(player);
            enemy_list.push_back(zako);
        }

        else if (stage_timer < 77.0)
        {
        }

        else 
        {
            if (!boss_spawned)
            {
                boss1 = objm->CreateObject<Stage1Boss>(Vector2D(670, -200));
                boss1->SetPattern(BossPattern::Entrance);
                enemy_list.push_back(boss1); 
                boss_spawned = true;
                boss1->SetPlayer(player);

            }
        }


        enemy_spawn_timer = 0.0f;
    }
}



void Stage1::DrawScrollBackground() const
{
    // 背景画像のスクロール
    int bg_height;
    GetGraphSize(bg_image, nullptr, &bg_height);

    float offset_f = fmod(scroll_y, static_cast<float>(bg_height));
    int offset = static_cast<int>(offset_f);

    DrawGraph(0, offset - bg_height, bg_image, TRUE);
    DrawGraph(0, offset, bg_image, TRUE);

    // === 背面グリッド（深めの緑・アニメ効果）===
    const int grid_back = 160;  // 背景奥のグリッド（縦横）
    const int grid_front = 80; // 前景グリッド（強調）

// === 背面グリッド（深めの緑・アニメ効果）===
SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
for (int x = 0; x < D_WIN_MAX_X; x += grid_back)
{
    int color = GetColor(0, 160 + (x % 3) * 20, 100);
    DrawLine(x, 0, x, D_WIN_MAX_Y, color);
}
for (int y = -grid_back; y < D_WIN_MAX_Y + grid_back; y += grid_back)
{
    int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_back;
    int color = GetColor(0, 200, 100);
    DrawLine(0, sy, D_WIN_MAX_X, sy, color);
}

// === ランダムに流れるハイライトライン（横）===
if (GetNowCount() % 120 < 60)
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    int flash_y = 40 + (GetNowCount() / 2) % (D_WIN_MAX_Y - 80);
    DrawLine(0, flash_y, D_WIN_MAX_X, flash_y, GetColor(0, 255, 255));
}

// === 前面グリッド（ネオン＋多色風）===
SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
for (int x = 0; x < D_WIN_MAX_X; x += grid_front)
{
    int intensity = 200 + (x % 3) * 20;
    DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(0, intensity, 255), TRUE);
}

for (int y = -grid_front; y < D_WIN_MAX_Y + grid_front; y += grid_front)
{
    int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_front;
    int blue_shift = 150 + ((sy / 20) % 3) * 30;
    DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(0, blue_shift, 180), TRUE);
}



const int total_lines = D_WIN_MAX_Y / grid_front;

SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

// 横線にグラデーション＆太さ＋透明度変化
for (int i = 0; i < total_lines + 2; ++i)
{
    int y = i * grid_front;
    int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_front;

    // 遠近感を演出：上ほど薄く・細く、下ほど濃く・太く
    float depth_ratio = static_cast<float>(i) / static_cast<float>(total_lines);
    int alpha = static_cast<int>(depth_ratio * 255);                  // 透明度：遠→薄い
    int thickness = static_cast<int>(1 + depth_ratio * 3);            // 線の太さ：遠→細い
    int r = static_cast<int>(50 + depth_ratio * 150);                 // R: 紫寄りへ
    int g = static_cast<int>(150 - depth_ratio * 100);                // G: 減らす
    int b = static_cast<int>(200 + depth_ratio * 55);                 // B: 青強めへ

    int color = GetColor(r, g, b);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    for (int t = -thickness / 2; t <= thickness / 2; ++t)
    {
        DrawLine(0, sy + t, D_WIN_MAX_X, sy + t, color);
    }
}


    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}





