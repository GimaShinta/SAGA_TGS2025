#include "Stage3.h"
#include "Stage2.h" // 次のステージがある場合

#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot3.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot4.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot5.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako3.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Object/Item/PowerUp/PowerUp.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage3.h"
#include <math.h>
#include <memory>

Stage3::Stage3(Player* player) : StageBase(player), zako2(nullptr), boss(nullptr), e_shot1(nullptr), e_shot2(nullptr)
{
    bg_scroll_speed_layer1 = 300.0f; // 奥
    bg_scroll_speed_layer2 = 600.0f; // 手前
}
Stage3::~Stage3() {}

void Stage3::Initialize()
{
    // 初期化処理
    // 【記述位置は仮】ステージの長さを代入
    distance = STAGE3_DISTANCE;

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) - 100.0f));
    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 + 100, (D_WIN_MAX_Y / 2) - 100.0f));
    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 + 150, (D_WIN_MAX_Y / 2) - 100.0f));

}

void Stage3::Finalize()
{
    // 終了処理
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Finalize();
}

void Stage3::Update(float delta)
{
    UpdateBackgroundScroll(delta);

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

    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    // プレイヤーの攻撃
    PlayerShot();

    // 敵の出現
    EnemyAppearance();

    // 敵の攻撃
    EnemyShot(delta);

    // 警告表示
    DisplayWarning(delta);

    // クリア判定
    UpdateGameStatus(delta);
}

void Stage3::Draw()
{
    // 背景を白で塗る（最初に描画）
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    //スクロール背景
    DrawScrollBackground();  

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
        if (is_warning == true)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, brend);
            DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(255, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        }
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
    return nullptr; // 次のステージへ
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

void Stage3::PlayerShot()
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

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
}

// 敵の出現
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

// 敵の攻撃
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

                // 攻撃パターン１（下方向に３発）
                if (bs_attack_pattrn == 1)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        objm->CreateObject<EnemyShot1>(Vector2D((e_location.x - 30) + (30 * i), e_location.y + D_OBJECT_SIZE));
                    }
                    enemy_list[i]->SetIsShot();
                }
                // 攻撃パターン２（４方向に３発）
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
                // 攻撃パターン３（三段階のやつ）
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
            // ボス２の攻撃パターン
            else if (enemy_type == ENE_BOSS2)
            {
                // ボスの攻撃パターンを取得
                bs_attack_pattrn = boss2->GetAttackPattrn();
                Vector2D e_location = enemy_list[i]->GetLocation();

                // パターン１（下方向に３発）
                if (bs_attack_pattrn == 1)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 30) + (30 * i), e_location.y + D_OBJECT_SIZE));
                        e_shot4->SetVelocity(Vector2D(0, 120));
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
                            e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 60) + (30 * j), e_location.y));
                            if (i == 0)
                            {
                                e_shot4->SetVelocity(Vector2D(0, 120));
                            }
                            else if (i == 1)
                            {
                                e_shot4->SetVelocity(Vector2D(0, -120));
                            }
                        }
                    }
                    // 横２方向
                    for (int i = 0; i < 2; i++)
                    {
                        for (int j = 0; j < 5; j++)
                        {
                            e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(e_location.x, (e_location.y - 60) + (30 * j)));
                            if (i == 0)
                            {
                                e_shot4->SetVelocity(Vector2D(-120, 0));
                            }
                            else if (i == 1)
                            {
                                e_shot4->SetVelocity(Vector2D(120, 0));
                            }
                        }
                    }
                    enemy_list[i]->SetIsShot();
                }
                // 攻撃パターンが３（段階の攻撃）
                else if (bs_attack_pattrn == 3)
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
                else if (bs_attack_pattrn == 4)
                {
                    /// <summary>
                    /// 攻撃パターン４（花火）
                    /// </summary>
                    /// <param name="bullet_num">弾の数</param>
                    /// <param name="speed">弾の速度</param>
                    /// <param name="spiral_interval">発射間隔</param>
                    /// <param name="spiral_duration_limit">攻撃する時間</param>
                    /// <param name="generate_location">生成する位置</param>
                    /// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
                    Pattrn4(24, 150.0f, 3.0f, 0.3f, boss2->GetLocation(), delta_second);
                }
                // 攻撃パターン５（渦巻き）
                else if (bs_attack_pattrn == 5)
                {
#if 0
                    static float spiral_timer = 0.0f;            // 弾発射のインターバル管理
                    static float spiral_angle = 0.0f;            // 回転角度
                    static float spiral_total_time = 0.0f;       // パターン5に入ってからの累計時間
                    const float spiral_interval = 0.1f;
                    const float spiral_duration_limit = 3.0f;    // 最大発射時間

                    // 3秒経過したら何もしない
                    if (spiral_total_time >= spiral_duration_limit) return;

                    spiral_timer += delta_second;
                    spiral_total_time += delta_second;

                    if (spiral_timer >= spiral_interval)
                    {
                        spiral_timer = 0.0f;

                        float rad = spiral_angle * DX_PI / 180.0f;
                        float speed = 160.0f;
                        Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

                        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
                        EnemyShot4* shot = objm->CreateObject<EnemyShot4>(boss2->GetLocation());
                        shot->SetVelocity(velocity);

                        spiral_angle += 15.0f;
                        if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
                    }
#else

                    /// <summary>
                    /// 攻撃パターン５（渦巻き）
                    /// </summary>
                    /// <param name="spiral_interval">発射間隔</param>
                    /// <param name="spiral_duration_limit">攻撃する時間</param>
                    /// <param name="spiral_speed">弾の速度</param>
                    /// <param name="generate_location">生成する位置</param>
                    /// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
                    Pattrn5(0.3f, 3.0f, 160.0f, boss2->GetLocation(), delta_second);
#endif
                }
                else if (bs_attack_pattrn == 6)
                {
                    /// <summary>
                    /// 攻撃パターン６（バラバラ扇型）
                    /// </summary>
                    /// <param name="fan_angle_range">扇の角度（60なら±30°）</param>
                    /// <param name="bullet_speed">弾の速度</param>
                    /// <param name="fan_interval">発射間隔</param>
                    /// <param name="fan_duration_limit">攻撃する時間</param>
                    /// <param name="generate_location">生成する位置</param>
                    /// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
                    Pattrn6(0.0f, 180.0f, 0.2f, 3.0f, boss2->GetLocation(), delta_second);
                }
                else if (bs_attack_pattrn == 7)
                {
                    /// <summary>
                    /// 攻撃パターン７（段階扇形）
                    /// </summary>
                    /// <param name="fan_angle_range">扇の角度範囲（60なら±30°）</param>
                    /// <param name="bullet_speed">弾の速度</param>
                    /// <param name="fan_interval">発射の間隔</param>
                    /// <param name="fan_duration_limit">攻撃時間</param>
                    /// <param name="generate_location">生成する位置</param>
                    /// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
                    Pattrn7(60.0f, 180.0f, 0.4f, 3.0f, boss2->GetLocation(), delta_second);
                }
                else if (bs_attack_pattrn == 8)
                {
                    /// <summary>
                    /// 攻撃パターン８（ひもQ）
                    /// </summary>
                    /// <param name="wave_interval">発射間隔</param>
                    /// <param name="wave_duration_limit">発車時間の上限</param>
                    /// <param name="generate_location">生成する位置</param>
                    /// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
                    Pattrn8(0.1f, 1.0f, boss2->GetLocation(), delta_second);
                }
                else if (bs_attack_pattrn == 9)
                {
                    /// <summary>
                    /// 攻撃パターン９（ボス中心回転弾）
                    /// </summary>
                    /// <param name="shot_count">弾の数</param>
                    /// <param name="radius">ボスからの距離</param>
                    /// <param name="angular_speed">回転速度</param>
                    /// <param name="bullet_speed">弾速</param>
                    /// <param name="rotation_timer">回転の時間計測</param>
                    /// <param name="generate_location">生成する位置</param>
                    /// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
                    Pattrn9(27, 150.0f, 90.0f, 0.0f, boss2->GetLocation(), delta_second);
                }
                else if (bs_attack_pattrn == 10)
                {
                    /// <summary>
                    /// 攻撃パターン１０（ベクトル中心回転弾）
                    /// </summary>
                    /// <param name="shot_count">弾の数</param>
                    /// <param name="radius">ボスからの距離</param>
                    /// <param name="angular_speed">回転速度</param>
                    /// <param name="bullet_speed">弾速</param>
                    /// <param name="rotation_timer">回転の時間計測</param>
                    /// <param name="center_location">生成する場所</param>
                    /// <param name="generate_location">生成する場所</param>
                    /// <param name="delta_second">１フレームあたりの時間（基本的に変更なし）</param>
                    Pattrn10(8, 120.0f, 90.0f, 150.0f, 5.0f, boss2->GetLocation(), delta_second);
                }
            }
        }
    }
}

// クリア判定
void Stage3::UpdateGameStatus(float delta)
{
    // ボスが倒れたらクリア
    if (boss != nullptr && boss->GetIsAlive() == false && is_over == false)
    {
        boss->SetDestroy();
        is_clear = true;
    }

    // ボス２が倒れたらクリア
    if (boss2 != nullptr && boss2->GetIsAlive() == false && is_over == false)
    {
        boss2->SetDestroy();
        is_clear = true;
    }

    // プレイヤーが倒れたらゲームオーバー
    if (player != nullptr && player->GetIsAlive() == false && is_clear == false)
    {
        // オブジェクト管理クラスのインスタンスを取得
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
        objm->Finalize();
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

void Stage3::DrawScrollBackground() const
{
    static float time = 0.0f;
    time += 0.05f;

    // 背景色：やや赤みがかったグレー
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(30, 10, 10), TRUE);

    // === 背面レイヤー（奥・細かく・暗め） ===
    const int grid_size_back = 40;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(100, 0, 0));

    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(100, 0, 0));
    }

    // === 前面レイヤー（太く・明るく・警告感） ===
    const int grid_size_front = 80;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(255, 40, 40), TRUE);

    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(255, 40, 40), TRUE);
    }

    // === ノイズ演出（崩壊感） ===
    for (int i = 0; i < 5; ++i) {
        if (rand() % 70 == 0) {
            int nx = rand() % D_WIN_MAX_X;
            int ny = rand() % D_WIN_MAX_Y;
            DrawBox(nx, ny, nx + 3, ny + 3, GetColor(255, 100, 50), TRUE); // オレンジ警告ドット
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

/// <summary>
/// 攻撃パターン４
/// </summary>
/// <param name="bullet_num">弾の数</param>
/// <param name="speed">弾の速度</param>
/// <param name="spiral_interval">発射間隔</param>
/// <param name="spiral_duration_limit">攻撃する時間</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Stage3::Pattrn4(int bullet_num, float speed, float spiral_interval, float spiral_duration_limit, const Vector2D& generate_location, float delta_second)
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static float spiral_timer = 0.0f;
    static float spiral_total_time = 0.0f;

    spiral_timer += delta_second;
    spiral_total_time += delta_second;

    if (spiral_timer >= spiral_interval)
    {
        spiral_timer = 0.0f;

        for (int i = 0; i < bullet_num; i++)
        {
            float angle = (360.0f / bullet_num) * i;
            float rad = angle * DX_PI / 180.0f;

            Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

            EnemyShot4* e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
            e_shot4->SetVelocity(velocity);
        }
    }

    // 時間制限を超えたら終了（発射しない）
    if (spiral_total_time >= spiral_duration_limit)
    {
        spiral_total_time = 0.0f;
        boss2->SetIsShot();
    }
}
/// <summary>
/// 攻撃パターン５
/// </summary>
/// <param name="spiral_interval">発射間隔</param>
/// <param name="spiral_duration_limit">攻撃する時間</param>
/// <param name="spiral_speed">弾の速度</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Stage3::Pattrn5(float spiral_interval, float spiral_duration_limit, float spiral_speed, const Vector2D& generate_location, float delta_second)
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static float spiral_timer = 0.0f;
    static float spiral_angle = 0.0f;
    static float spiral_total_time = 0.0f;

    spiral_timer += delta_second;
    spiral_total_time += delta_second;

    if (spiral_timer >= spiral_interval)
    {
        spiral_timer = 0.0f;

        for (int dir = 0; dir < 2; dir++)
        {
            float base_angle = (dir == 0) ? 90.0f : 270.0f;
            float current_angle = base_angle + spiral_angle;

            float rad = current_angle * DX_PI / 180.0f;
            Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

            EnemyShot4* shot = objm->CreateObject<EnemyShot4>(generate_location);
            shot->SetVelocity(velocity);
        }

        spiral_angle += 10.0f;
        if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
    }

    // 時間制限を超えたら終了（発射しない）
    if (spiral_total_time >= spiral_duration_limit)
    {
        spiral_total_time = 0.0f;
        boss2->SetIsShot();
    }
}

/// <summary>
/// 攻撃パターン６
/// </summary>
/// <param name="fan_angle_range">扇の角度（60なら±30°）</param>
/// <param name="bullet_speed">弾の速度</param>
/// <param name="fan_interval">発射間隔</param>
/// <param name="fan_duration_limit">攻撃する時間</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Stage3::Pattrn6(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // 攻撃持続用 static 変数
    static float fan_timer = 0.0f;
    static float fan_total_time = 0.0f;

    // 攻撃パターンが変更されたらリセットしたい → boss2側で状態持たせるのもあり

    fan_timer += delta_second;
    fan_total_time += delta_second;

    // 一定間隔ごとに発射
    if (fan_timer >= fan_interval)
    {
        fan_timer = 0.0f;

        // ランダムな角度（中心±60°）
        float base_angle = 90.0f; // 下方向中心
        float random_angle = base_angle - fan_angle_range / 2.0f + (rand() % (int)fan_angle_range);

        float rad = random_angle * DX_PI / 180.0f;
        Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

        e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
        e_shot4->SetVelocity(velocity);
    }

    // 時間制限を超えたら終了（発射しない）
    if (fan_total_time >= fan_duration_limit)
    {
        fan_total_time = 0.0f;
        boss2->SetIsShot();
    }
}

/// <summary>
/// 攻撃パターン７
/// </summary>
/// <param name="fan_angle_range">扇の角度範囲（60なら±30°）</param>
/// <param name="bullet_speed">弾の速度</param>
/// <param name="fan_interval">発射の間隔</param>
/// <param name="fan_duration_limit">攻撃時間</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Stage3::Pattrn7(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static float fan_timer = 0.0f;
    static float fan_total_time = 0.0f;

    fan_timer += delta_second;
    fan_total_time += delta_second;

    if (fan_timer >= fan_interval)
    {
        fan_timer = 0.0f;

        int bullet_count = 3;
        float base_angle = 90.0f; // 中心下方向

        for (int i = 0; i < bullet_count; ++i)
        {
            float angle = base_angle - fan_angle_range / 2.0f +
                (fan_angle_range / (bullet_count - 1)) * i;

            float rad = angle * DX_PI / 180.0f;
            Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

            e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
            e_shot4->SetVelocity(velocity);
        }
    }

    // 一定時間経過したら終了
    if (fan_total_time >= fan_duration_limit)
    {
        fan_total_time = 0.0f;
        boss2->SetIsShot();
    }
}

/// <summary>
/// 攻撃パターン８
/// </summary>
/// <param name="wave_interval">発射間隔</param>
/// <param name="wave_duration_limit">発車時間の上限</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Stage3::Pattrn8(float wave_interval, float wave_duration_limit, const Vector2D& generate_location, float delta_second)
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

#if 1
    static float wave_timer = 0.0f;
    static float wave_total_time = 0.0f;

    wave_timer += delta_second;
    wave_total_time += delta_second;

    if (wave_timer >= wave_interval)
    {
        wave_timer = 0.0f;
        Vector2D e_lo = generate_location;

        // 右側
        e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x + 50, e_lo.y));
        e_shot5->SetWaveReflected(false);
        e_shot5->SetVelocity(Vector2D(0, 200));
        e_shot5->SetWaveParameters(400.0f, 0.7f);

        // 左側
        e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x - 50, e_lo.y));
        e_shot5->SetWaveReflected(true);
        e_shot5->SetVelocity(Vector2D(0, 200));
        e_shot5->SetWaveParameters(400.0f, 0.7f);
    }

    // 一定時間経過したら終了
    if (wave_total_time >= wave_duration_limit)
    {
        wave_total_time = 0.0f;
        boss2->SetIsShot();
    }
#else

    int num_shots = 20;
    float spread_speed = 150.0f;
    Vector2D origin = e_location; // 弾の初期発射位置（ボスの位置）
    Vector2D boss_center = boss2->GetLocation(); // 吸い込み中心！

    for (int i = 0; i < num_shots; ++i)
    {
        float angle = 360.0f / num_shots * i;
        float rad = angle * DX_PI / 180.0f;

        e_shot5 = objm->CreateObject<EnemyShot5>(origin);
        e_shot5->SetVelocity(Vector2D(cosf(rad), sinf(rad)) * spread_speed);
        e_shot5->SetSuckCenter(boss_center); // 吸い込み先をセット！
    }
#endif

}

/// <summary>
/// 攻撃パターン９
/// </summary>
/// <param name="shot_count">弾の数</param>
/// <param name="radius">ボスからの距離</param>
/// <param name="angular_speed">回転速度</param>
/// <param name="bullet_speed">弾速</param>
/// <param name="rotation_timer">回転の時間計測</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Stage3::Pattrn9(int shot_count, float radius, float angular_speed, float bullet_speed, const Vector2D& generate_location, float delta_second)
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static std::vector<float> angles;    // 各弾の角度管理
    static std::vector<EnemyShot4*> rotating_shots;  // 弾のポインタを保持

    static float rotation_timer = 0.0f;

    rotation_timer += delta_second;

    // 初期化処理
    if (angles.empty())
    {
        rotating_shots.clear();
        // 弾を生成して初期角度を設定
        for (int i = 0; i < shot_count; ++i)
        {
            float angle = 360.0f / shot_count * i;  // 弾の角度設定
            angles.push_back(angle);

            // ボスの周囲に弾を配置
            e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);

            if (e_shot4)
            {
                rotating_shots.push_back(e_shot4);  // 正しく生成されたら弾を保存
                e_shot4->SetVelocity(Vector2D(0, 0)); // 回転だけなので弾速は0
            }
        }
    }

    // 各弾の位置を更新して回転させる
    for (int i = 0; i < shot_count; ++i)
    {
        angles[i] += angular_speed * delta_second; // 角度更新
        if (angles[i] >= 360.0f) angles[i] -= 360.0f; // 角度を0?360度に調整

        float rad = angles[i] * DX_PI / 180.0f;  // ラジアンに変換

        // ボスを中心に半径 `radius` の円軌道を描く
        Vector2D new_pos = generate_location + Vector2D(cos(rad) * radius, sin(rad) * radius);

        // 弾の位置を更新
        if (rotating_shots[i])  // 弾が正しく存在するか確認
        {
            rotating_shots[i]->SetLocation(new_pos);

            // 弾の速度はゼロ（回転のみ）
            rotating_shots[i]->SetVelocity(Vector2D(0, 0));
        }
    }

    // 攻撃の終了判定（一定時間経過後）
    static float duration_timer = 0.0f;
    duration_timer += delta_second;
    if (duration_timer >= 5.0f)  // 5秒後に終了
    {
        for (int i = 0; i < shot_count; ++i)
        {
            if (rotating_shots[i])
            {
                rotating_shots[i]->SetDestroy();  // 弾を削除
            }

        }
        angles.clear();  // 角度情報もクリア
        rotating_shots.clear();  // 弾のリストもクリア
        duration_timer = 0.0f;
        boss2->SetIsShot();  // 攻撃終了
    }
}

/// <summary>
/// 攻撃パターン１０
/// </summary>
/// <param name="shot_count">弾の数</param>
/// <param name="radius">ボスからの距離</param>
/// <param name="angular_speed">回転速度</param>
/// <param name="bullet_speed">弾速</param>
/// <param name="rotation_timer">回転の時間計測</param>
/// <param name="generate_location">生成する場所</param>
/// <param name="delta_second">１フレームあたりの時間</param>
void Stage3::Pattrn10(int shot_count, float radius, float angular_speed, float center_speed, float duration_limit, const Vector2D& generate_location, float delta_second)
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static float duration_timer = 0.0f;
    static Vector2D center_pos;  // 見えない中心座標
    static Vector2D center_velocity;
    static std::vector<float> angles;
    static std::vector<EnemyShot4*> rotating_shots;
    static bool initialized = false;

    // 時間経過
    duration_timer += delta_second;

    if (duration_timer < duration_limit)
    {
        if (!initialized) // 初期化処理
        {
            // ボスの位置を初期中心として、真下に移動させる
            center_pos = generate_location;
            center_velocity = Vector2D(0.0f, center_speed); // 真下方向

            angles.clear();
            rotating_shots.clear();
            for (int i = 0; i < shot_count; ++i)
            {
                float angle = 360.0f / shot_count * i;
                angles.push_back(angle);

                EnemyShot4* shot = objm->CreateObject<EnemyShot4>(center_pos);
                if (shot)
                {
                    shot->SetVelocity(Vector2D(0, 0));
                    rotating_shots.push_back(shot);
                }
            }

            initialized = true;
        }

        // 見えない中心の移動
        center_pos += center_velocity * delta_second;

        // 回転弾の位置更新
        for (int i = 0; i < shot_count; ++i)
        {
            angles[i] += angular_speed * delta_second;
            if (angles[i] >= 360.0f) angles[i] -= 360.0f;

            float rad = angles[i] * DX_PI / 180.0f;
            Vector2D offset(cos(rad) * radius, sin(rad) * radius);

            if (rotating_shots[i])
            {
                rotating_shots[i]->SetLocation(center_pos + offset);
            }
        }
    }
    else
    {
        // 攻撃終了後の処理
        duration_timer = 0.0f;
        boss2->SetIsShot();  // 攻撃終了
        initialized = false;  // 初期化フラグをリセット
    }
}