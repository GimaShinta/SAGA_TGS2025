#include "Stage3.h"
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
#include "../../../../Scene/SceneType/Stage/StageType/Stage3.h"
#include <math.h>

Stage3::Stage3(Player* player) : StageBase(player), zako2(nullptr), boss(nullptr), e_shot1(nullptr), e_shot2(nullptr) {}
Stage3::~Stage3() {}

void Stage3::Initialize()
{
    // 初期化処理
        // 【記述位置は仮】ステージの長さを代入
    distance = STAGE3_DISTANCE;
}

void Stage3::Finalize()
{
    // 終了処理
}

void Stage3::Update(float delta)
{
    //// ゲームの骨組みとなる処理を、ここに記述する
    //int spd = 1; // スクロールの速さ
    //if (distance == 0)
    //{
    //    spd = 0; // ボス戦はスクロール停止
    //}
    //scrollBG(spd); // 背景のスクロール
    //moveEnemy(); // 敵機の制御
    //moveBullet(); // 弾の制御
    //moveItem(); // アイテムの制御
    //drawEffect(); // エフェクト
    //stageMap(); // ステージマップ
    //drawParameter(); // 自機のシールドなどのパラメーターを表示

    timer++; // タイマーをカウント

    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    // プレイヤーが弾を打つ準備ができていたら弾を生成        
    if (player->GetIsShot() == true)
    {
        Vector2D p_location = player->GetLocation();
        player->SetIsShot();
        // 上下反転していなかったら下方向に生成
        if (player->GetShotFlip() == false)
        {
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
            shot->SetShotFlip(false);
        }
        // 反転していたら上方向に生成
        else
        {
            Vector2D p_location = player->GetLocation();
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x, p_location.y + D_OBJECT_SIZE));
            shot->SetShotFlip(true);
        }
    }
    // プレイヤーがビームをうつ準備ができていたらビームを生成
    else if (player->GetBeamOn() == true)
    {
        Vector2D p_location = player->GetLocation();
        // 上下反転していなかったら下方向に生成
        if (player->GetShotFlip() == false)
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y - D_OBJECT_SIZE) - 848));
            beam->SetBeamFlip(false);
        }
        // 反転していたら上方向に生成
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

    // 一定時間ごとに distance を減らす（例：0.1秒ごとに1減らす）
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

    // 警告表示
    DisplayWarning(delta);

    if (boss != nullptr && boss->GetIsAlive() == false && is_over == false)
    {
        boss->SetDestroy();
        is_clear = true;
    }

    if (boss2 != nullptr && boss2->GetIsAlive() == false && is_over == false)
    {
        boss2->SetDestroy();
        is_clear = true;
    }

    if (player != nullptr && player->GetIsAlive() == false && is_clear == false)
    {
        // オブジェクト管理クラスのインスタンスを取得
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
        objm->Finalize();

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

void Stage3::Draw()
{
    DrawString(0, 0, "ゲームメイン", GetColor(255, 255, 255));

    DrawString(0, 300, "操作方法\n\n左スティック\n十字ボタン\nWASDキー : 移動\n\nAボタン\nスペースキー : 発射\n\nBボタン\nBキー : レーザー\n\nRBボタン\nLキー : 射出反転", GetColor(255, 255, 255));

    DrawFormatString(0, 20, GetColor(255, 255, 0), "敵数: %d", enemy_list.size());

    // ステージ描画
    if (stage <= 1)
    {
        DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

        if (is_warning == true)
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

    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Draw();

    if (is_clear == true)
    {
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "ゲームクリア", GetColor(0, 0, 0));
    }
    else if (is_over == true)
    {
        DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "ゲームオーバー", GetColor(0, 0, 0));
    }
}

bool Stage3::IsFinished()
{
    return finished;
}

bool Stage3::IsClear()
{
    return is_clear;
}

bool Stage3::IsOver()
{
    return is_over;
}

StageBase* Stage3::GetNextStage(Player* player)
{
    return new Stage2(player); // 次のステージへ
}

void Stage3::DisplayWarning(float delta_second)
{
    // 警告表示（赤く光る）
    if (is_warning == true)
    {
        // カウント加算
        warning_timer += delta_second;
        if (warning_timer >= 0.0005f)
        {
            // 透明度を０から２５５まで加算減算を繰り返す
            if (is_brend == false)
            {
                if (brend < 255)
                {
                    brend++;
                }
                else
                {
                    is_brend = true;
                }
            }
            else
            {
                if (brend > 0)
                {
                    brend--;
                }
                else
                {
                    // 到達カウントを加算
                    reach_count++;
                    is_brend = false;
                }
            }
            // カウントリセット
            warning_timer = 0;
        }
        // ３回警告が終わったら全てリセット
        if (reach_count >= 3)
        {
            is_warning = false;
            brend = 255;
            warning_timer = 0;
            reach_count = 0;
        }
    }
}

void Stage3::EnemyAppearance()
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // 雑魚１と雑魚２の出現
    if (300 < distance && distance % 1000 == 0)
    {
        // 出現範囲を画面中央の幅200に設定
        int center_x = D_WIN_MAX_X / 2;
        int half_width = 250; // 出現範囲の半分の幅
        int x = center_x - half_width + rand() % (half_width * 2);
        int y = -50;
        int e = 1 + rand() % 2;
        if (e == ENE_ZAKO1)
        {
            enemy_list.push_back(objm->CreateObject<Zako1>(Vector2D(x, y)));
        }
        if (e == ENE_ZAKO2)
        {
            int vx = 0;
            if (player->GetLocation().x < x)
            {
                vx = -60;
            }
            if (player->GetLocation().x > x)
            {
                vx = 60;
            }
            enemy_list.push_back(zako2 = objm->CreateObject<Zako2>(Vector2D(x, -100)));
            zako2->SetVelocity(Vector2D(vx, 50));
        }
    }
    // ザコ3の出現
    if (300 < distance && distance < 900 && distance % 400 == 0)
    {
        // 出現範囲を画面中央の幅200に設定
        int center_x = D_WIN_MAX_X / 2;
        int half_width = 250; // 出現範囲の半分の幅
        int x = center_x - half_width + rand() % (half_width * 2);
        int y = -50;
        enemy_list.push_back(objm->CreateObject<Zako3>(Vector2D(x, -100)));
    }
    // 警告表示
    if (distance == 150)
    {
        is_warning = true;
    }
    // ボス出現
    if (distance == 1 && boss2 == nullptr)
    {
        //enemy_list.push_back(boss = objm->CreateObject<Boss>(Vector2D(D_WIN_MAX_X / 2 + 200, D_WIN_MAX_Y + 200)));
        enemy_list.push_back(boss2 = objm->CreateObject<Boss2>(Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2)));
    }
}

void Stage3::EnemyShot(float delta_second)
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // 敵が弾を打つ準備ができていたら弾を発射する
    for (int i = 0; i < enemy_list.size(); i++)
    {
        if (enemy_list[i]->GetIsShot() == true)
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
                //テキの位置からプレイヤーへのベクトルを求める
                Vector2D b = player->GetLocation() - enemy_list[i]->GetLocation();
                float c = sqrt(pow(b.x, 2) + pow(b.y, 2));
                //プレイヤーに向かって弾を打つ
                e_shot2 = objm->CreateObject<EnemyShot2>(enemy_list[i]->GetLocation());
                e_shot2->SetVelocity(Vector2D(b.x / c, b.y / c));
                enemy_list[i]->SetIsShot();
            }
            else if (enemy_type == ENE_BOSS)
            {
                // ボスの攻撃パターンを取得
                bs_attack_pattrn = boss->GetAttackPattrn();
                Vector2D e_location = enemy_list[i]->GetLocation();

                // パターン１（下方向に３発）
                if (bs_attack_pattrn == 1)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        objm->CreateObject<EnemyShot1>(Vector2D((e_location.x - 30) + (30 * i), e_location.y + D_OBJECT_SIZE));
                    }
                    enemy_list[i]->SetIsShot();
                }
                // パターン２（４方向に３発）
                else if (bs_attack_pattrn == 2)
                {
                    // 縦２方向
                    for (int i = 0; i < 2; i++)
                    {
                        for (int j = 0; j < 5; j++)
                        {
                            e_shot1 = objm->CreateObject<EnemyShot1>(Vector2D((e_location.x - 60) + (30 * j), e_location.y));
                            if (i == 0)
                            {
                                e_shot1->SetVelocity(Vector2D(0, 120));
                            }
                            else if (i == 1)
                            {
                                e_shot1->SetVelocity(Vector2D(0, -120));
                            }
                        }
                    }
                    // 横２方向
                    for (int i = 0; i < 2; i++)
                    {
                        for (int j = 0; j < 5; j++)
                        {
                            e_shot1 = objm->CreateObject<EnemyShot1>(Vector2D(e_location.x, (e_location.y - 60) + (30 * j)));
                            if (i == 0)
                            {
                                e_shot1->SetVelocity(Vector2D(-120, 0));
                            }
                            else if (i == 1)
                            {
                                e_shot1->SetVelocity(Vector2D(120, 0));
                            }
                        }
                    }
                    enemy_list[i]->SetIsShot();
                }
                else if (bs_attack_pattrn == 3)
                {
                    // 縦に３よこに５つ生成（片方）
                    for (int j = 1; j < 4; j++)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            e_shot3 = objm->CreateObject<EnemyShot3>(Vector2D(boss->GetLocation()));
                            e_shot3->SetPlayer(player);
                            e_shot3->SetPlayerLocation(player->GetLocation());
                            // +１しなかったら０秒で発射される
                            e_shot3->SetStepShot(j);
                            if (i < 5)
                            {
                                e_shot3->SetVelocity(Vector2D(240, 0));
                                e_shot3->SetPurposeLocation(Vector2D((e_location.x + 150) + (30 * i), (e_location.y + 40) - (50 * j)));
                            }
                            else
                            {
                                e_shot3->SetVelocity(Vector2D(-240, 0));
                                e_shot3->SetPurposeLocation(Vector2D(e_location.x - (30 * i), (e_location.y + 40) - (50 * j)));
                            }
                        }
                    }
                    enemy_list[i]->SetIsShot();
                }
            }
            else if (enemy_type == ENE_BOSS2)
            {
                // ボスの攻撃パターンを取得
                bs_attack_pattrn = boss2->GetAttackPattrn();
                Vector2D e_location = enemy_list[i]->GetLocation();

            if (bs_attack_pattrn == 3)
            {
                // 縦に３よこに５つ生成（片方）
                for (int j = 1; j < 4; j++)
                {
                    for (int i = 0; i < 10; i++)
                    {
                        e_shot3 = objm->CreateObject<EnemyShot3>(Vector2D(boss2->GetLocation()));
                        e_shot3->SetPlayer(player);
                        e_shot3->SetPlayerLocation(player->GetLocation());
                        // +１しなかったら０秒で発射される
                        e_shot3->SetStepShot(j);
                        if (i < 5)
                        {
                            e_shot3->SetVelocity(Vector2D(240, 0));
                            e_shot3->SetPurposeLocation(Vector2D((e_location.x + 150) + (30 * i), (e_location.y + 40) - (50 * j)));
                        }
                        else
                        {
                            e_shot3->SetVelocity(Vector2D(-240, 0));
                            e_shot3->SetPurposeLocation(Vector2D(e_location.x - (30 * i), (e_location.y + 40) - (50 * j)));
                        }
                    }
                }
                enemy_list[i]->SetIsShot();
                }
            }
        }
    }
}
