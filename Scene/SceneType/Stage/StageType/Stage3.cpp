#include "Stage3.h"
#include "Stage2.h" 
#include "../../../../Scene/SceneType/TitleScene.h"

#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot3.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot4.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot5.h"
#include "../../../../Object/Character/Shot/EnemyBeam1.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako3.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako5.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako6.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Object/Item/PowerUp/PowerUp.h"
#include "../../../../Object/Item/Shield/Shield.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage3.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage4.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../../../Utility/ResourceManager.h"
#include "../../../../Utility/InputManager.h"
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

    //GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    //objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) + 100.0f));


#if 0
    //-----------アニメーション生成パターン１-----------
    // 画像を読み込んでから生成
    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    image_handles = rm->GetImages("Resource/Image/Effect/E_Explosion.png", 54, 9, 6, 517, 517);

    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->PlayerAnimation(image_handles, Vector2D(player->GetLocation().x, player->GetLocation().y), 0.05f, false);
    //---------------------------------------------------
#else
    //-----------アニメーション生成パターン２-----------
    // 生成したいエフェクトを指定する（エフェクトのみ）
    //AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    //anim_id = manager->PlayerAnimation(EffectName::eExprotion, Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2), 0.05f, false);
    //--------------------------------------------------
#endif

    //// アニメーションの追加設定
    //manager->SetAlpha(anim_id, 255);       // 半透明
    //manager->SetScale(anim_id, 5.0f);      // 1.5倍拡大
    //manager->SetZLayer(anim_id, 1);        // 描画順指定

    //フォント
    // font_digital = CreateFontToHandle("DS-Digital", 28, 6, DX_FONTTYPE_ANTIALIASING);
    font_digital = CreateFontToHandle("Orbitron", 28, 6, DX_FONTTYPE_ANTIALIASING);
    font_orbitron = CreateFontToHandle("Orbitron", 22, 6, DX_FONTTYPE_ANTIALIASING);

}

void Stage3::Finalize()
{
    // 終了処理
    // 敵リストをすべて削除
    for (auto& enemy : enemy_list)
    {
        enemy->SetDestroy();
    }
    enemy_list.clear();
    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->RemoveAnimation(anim_id);
}

void Stage3::Update(float delta)
{
    // ｘ押したらアニメーションの生成
    InputManager* input = Singleton<InputManager>::GetInstance();
    if (input->GetKeyDown(KEY_INPUT_X))
    {
        AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
        // アニメーション作成
        manager->PlayerAnimation(EffectName::eExprotion, Vector2D(player->GetLocation().x, player->GetLocation().y), 0.05f, false);
    }

    UpdateBackgroundScroll(delta);
    ScrollEffectUpdate(delta);

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

    //timer++; // タイマーをカウント

    // delta_second 分加算
    stage_timer += delta;
    delta_draw = delta;  // ← delta を保存

    //// 一定時間ごとに distance を減らす（例：0.1秒ごとに1減らす）
    //if (stage_timer >= 0.01f)
    //{
    //    if (distance > 0)
    //    {
    //        distance--;
    //    }
    //    else
    //    {
    //        distance = 0;
    //    }
    //    stage_timer = 0;
    //}

    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    // 毎フレーム
    manager->Update(delta);       // 更新（deltaTimeは前のフレームとの経過時間）

    // アニメーションが終了したか確認（削除が必要な場合）
    if (manager->GetAnimationFinished(anim_id) == true)
    {
        manager->RemoveAnimation(anim_id);  // 不要になったら削除
    }

    // プレイヤーの攻撃
    PlayerShot();

    // 敵の出現
    EnemyAppearance(delta);

    // 敵の攻撃
    EnemyShot(delta);

    // 警告表示
    DisplayWarning(delta);

    // クリア判定
    UpdateGameStatus(delta);

    if (is_clear)
    {
        if (result_started)
        {
            ResultDraw(delta);  
        }
    }

}

void Stage3::Draw()
{
    // 背景などの描画
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);
    DrawScrollBackground();
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // テキスト表示
    DrawString(0, 0, "Stage3", GetColor(255, 255, 255));
    DrawFormatString(0, 20, GetColor(255, 255, 0), "敵数: %d", enemy_list.size());

    // 警告演出
    if (stage <= 1 && is_warning)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, brend);
        DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(255, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    }

    // --- 描画順を制御 ---
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();

    if (draw_animation_first)  // ← このフラグで順序を切り替える
    {
        manager->Draw();       // 先にアニメーション
        objm->Draw();          // 後にオブジェクト
    }
    else
    {
        objm->Draw();          // 先にオブジェクト
        manager->Draw();       // 後にアニメーション
    }

    // ゲームクリア描画
    if (is_clear)
    {
        if (result_started)
        {
            int fade_alpha = (result_timer * 12.0f < 60.0f) ? static_cast<int>(result_timer * 12.0f) : 60;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
            DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
            ResultDraw(delta_draw);
        }

        //DrawFormatStringToHandle((D_WIN_MAX_X / 2) - 100.0f,120, GetColor(255, 255, 255), font_digital, "GAME CLEAR");
    }
    else if (is_over)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
        DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

        DrawFormatStringToHandle((D_WIN_MAX_X / 2) - 100.0f, (D_WIN_MAX_Y / 2), GetColor(255, 255, 255), font_digital, "GAME OVER");
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
    return new Stage4(player);
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
            if (player->GetPowerd() <= 1)
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));

            }
            else if (player->GetPowerd() == 2)
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 30, p_location.y));
            }
            else
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 50, p_location.y + D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 50, p_location.y + D_OBJECT_SIZE));

            }
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
void Stage3::EnemyAppearance(float delta)
{
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    stage_timer += delta;
    enemy_spawn_timer += delta;
    zako2_spawn_timer += delta;
    boss2_spawn_timer += delta;

    // 1. ボスが出ていればZakoは出現しない
    if (boss2_spawned) return;

    // 2. 各時間帯ごとの出現処理に分岐
    if (stage_timer < 5.0f)
    {
        SpawnBossAndItems();
        HandleZako1_LR(delta);
    }
    else if (stage_timer < 10.0f)
    {
        HandleZako1_Center(delta);
    }
    else if (stage_timer < 15.0f)
    {
        HandleZako2_Upward(delta);
    }
    else if (stage_timer < 20.0f)
    {
        const float spawn_interval = 4.0f;
        if (zako2_spawn_timer < spawn_interval) return;

        const int num = 6;
        const float spacing = 100.0f;
        const float base_y = D_WIN_MAX_Y + 50.0f;
        const float target_y = D_WIN_MAX_Y / 2 - 100.0f;
        const float delay_per = 0.3f;

        float start_x = ((D_WIN_MAX_X / 2) - 180.0f) - spacing;

        for (int i = 0; i < num; ++i)
        {
            float x = start_x + spacing * i;
            float delay = 2.0f + delay_per * i;

            Vector2D target_pos(x, target_y);
            Vector2D appear_pos(x - 50.0f * i, base_y);  // ← ここが修正ポイント

            auto zako = objm->CreateObject<Zako6>(appear_pos);
            zako->SetMode(ZakoMode::Zako2);
            zako->SetAppearParams(appear_pos, target_pos - (50.0f * i), delay, true);
        }

        zako2_spawn_timer = 0.0f;
    }
    else if (stage_timer < 30.0f)
    {
        const float spawn_interval = 4.0f;
        if (zako2_spawn_timer < spawn_interval) return;

        const int num = 6;
        const float spacing = 0.0f;
        const float base_y = D_WIN_MAX_Y + 50.0f;
        const float target_y = D_WIN_MAX_Y / 2 - 100.0f;
        const float delay_per = 0.3f;

        float start_x = ((D_WIN_MAX_X / 2) + 300.0f) - spacing;

        for (int i = 0; i < num; ++i)
        {
            float x = start_x - spacing * i;
            float delay = 2.0f + delay_per * i;

            Vector2D appear_pos(x - 50.0f * i, base_y);
            Vector2D target_pos(x, target_y);

            auto zako = objm->CreateObject<Zako6>(appear_pos);
            zako->SetMode(ZakoMode::Zako2);
            zako->SetAppearParams(appear_pos, target_pos - (50.0f * i), delay, true);
        }

        zako2_spawn_timer = 0.0f;
    }
    else if (stage_timer < 32.0f)
    {
        //float spawn_interval = my_max(2.0f - stage_timer / 5.0f, 0.5f);
        //if (enemy_spawn_timer < spawn_interval) return;

        //const int num = 5;
        //const float spacing = 150.0f;

        //bool from_left = (enemy_group_index % 2 == 0);

        //// y座標を from_left に応じて上下に切り替える
        //float appear_y = from_left ? -100.0f : D_WIN_MAX_Y + 100.0f;
        //float target_y = 300.0f;

        //float base_x = from_left ? 60.0f : (D_WIN_MAX_X - 60.0f);
        //float dx = from_left ? spacing : -spacing;
        //float end_base_x = from_left ? D_WIN_MAX_X / 2 + 120.0f : D_WIN_MAX_X / 2 - 240.0f;

        //for (int i = 0; i < num; ++i)
        //{
        //    float x = base_x + dx * i;
        //    float delay = i * 0.5f;

        //    Vector2D appear_pos(x, appear_y);
        //    Vector2D end_pos(end_base_x + spacing * i, target_y + (enemy_group_index * 100.0f));

        //    auto zako = objm->CreateObject<Zako6>(appear_pos);
        //    zako->SetMode(ZakoMode::Zako3);
        //    zako->SetAppearParams(appear_pos, end_pos, 1.3f + delay, from_left);
        //}

        //enemy_group_index++;
        //enemy_spawn_timer = 0.0f;
    }
    else if (stage_timer < 35.0f)
    {

    }
    else if (stage_timer < 37.0f)
    {
        is_warning = true;
    }
    else
    {
        SpawnBossAndItems();
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
            }
        }
    }
}

// クリア判定
void Stage3::UpdateGameStatus(float delta)
{
    if (boss2 != nullptr && boss2->GetIsCrashing() == true)
    {
        draw_animation_first = true;
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
        gameover_timer += delta;

        if (gameover_timer >= 0.005f)
        {
            transparent++;
            gameover_timer = 0.0f;
        }
        
        if (scene_timer >= 8.0f)
        {
            finished = true;
        }
    }

    // 仮の条件：スペースキーを押したらステージ終了
    if (CheckHitKey(KEY_INPUT_N))
    {
        finished = true;
    }

    if (is_clear == true && result_started == false)
    {
        clear_wait_timer += delta;
        if (clear_wait_timer >= 5.0f)
        {
            result_started = true;
            result_timer = 0.0f; // スコア演出タイマーリセット
        }
    }
}

//リザルト
void Stage3::ResultDraw(float delta)
{
    if (!result_started) return;

    result_timer += delta * 60.0f;

    const int cx = D_WIN_MAX_X / 2;
    const int cy = D_WIN_MAX_Y / 2 - 20;

    // フェード：透明度60まで
    int fade_alpha = (result_timer * 12.0f < 60.0f) ? static_cast<int>(result_timer * 12.0f) : 60;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
    DrawBox(cx - 350, 0, cx + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // フェード＆スライド演出設定
    const int fade_duration = 30;
    const int slide_distance = 60;

    auto GetAlpha = [&](int delay) -> int {
        int t = static_cast<int>(result_timer) - delay;
        if (t < 0) return 0;
        if (t >= fade_duration) return 255;
        return (255 * t) / fade_duration;
    };

    auto GetSlideY = [&](int base_y, int delay) -> int {
        int t = static_cast<int>(result_timer) - delay;
        if (t < 0) return base_y + slide_distance;
        if (t >= fade_duration) return base_y;
        return base_y + slide_distance - (slide_distance * t) / fade_duration;
    };

    // スコア取得
    ScoreData* score = Singleton<ScoreData>::GetInstance();
    const auto& scores = score->GetScoreData();
    float base_score = 0.0f;
    for (float s : scores) base_score += s;
    int life_bonus = player->life * 1000;
    total_score = base_score + life_bonus;

    // 表示ライン
    struct ResultLine {
        int delay;
        int y_offset;
        std::string label;
        std::string format;  // 空なら固定テキスト
    };

    std::vector<ResultLine> lines = {
        {  30, -80, "RESULT", "" },
        {  70, -20, "BASE SCORE", "BASE SCORE : %.0f" },
        { 110,  20, "LIFE BONUS", "LIFE BONUS : %d" },
        { 160,  60, "TOTAL SCORE", "TOTAL SCORE : %.0f" },
    };

    for (const auto& line : lines)
    {
        int alpha = GetAlpha(line.delay);
        int y = GetSlideY(cy + line.y_offset, line.delay);
        int color = GetColor(255, 255, 255);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        // 表示文字生成
        char buf[128];
        if (line.format.empty())
        {
            sprintf_s(buf, "%s", line.label.c_str());
        }
        else if (line.label == "BASE SCORE")
        {
            sprintf_s(buf, line.format.c_str(), base_score);
        }
        else if (line.label == "LIFE BONUS")
        {
            sprintf_s(buf, line.format.c_str(), life_bonus);
        }
        else if (line.label == "TOTAL SCORE")
        {
            sprintf_s(buf, line.format.c_str(), total_score);
        }

        // 中央寄せ位置を計算
        int width = GetDrawStringWidthToHandle(buf, strlen(buf), font_digital);
        DrawStringToHandle(cx - width / 2, y, buf, color, font_digital);

        if (line.label == "TOTAL SCORE" && alpha == 255)
        {
            result_displayed = true;
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // スコア表示完了後の待機タイマー
    if (result_displayed && !glitch_started && !glitch_done)
    {
        post_result_wait_timer += delta;
        if (post_result_wait_timer >= 8.0f) // 5秒待機後
        {
            glitch_started = true;
            glitch_timer = 0.0f;
        }
    }

    // グリッチ演出（強化バージョン）
    if (glitch_started && !glitch_done)
    {
        for (int i = 0; i < 50; ++i) // ← 数を増やして密度アップ（元は8）
        {
            int x = (rand() % 700) + (D_WIN_MAX_X / 2 - 350);
            int y = rand() % D_WIN_MAX_Y;
            int w = 80 + rand() % 150; // ← 幅広く
            int h = 8 + rand() % 40;   // ← 高さも太く
            int r = 150 + rand() % 106;  // 暗めの色も混ぜるとノイズ感が出る
            int g = 150 + rand() % 106;
            int b = 255;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220); // ← 不透明度強めで覆う
            DrawBox(x, y, x + w, y + h, GetColor(r, g, b), TRUE);
        }

        glitch_timer += delta;
        if (glitch_timer > 2.0f)
        {
            glitch_done = true;
            finished = true;
        }
    }


    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}


void Stage3::DrawScrollBackground() const
{
    // === 背景の塗りつぶし ===
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(20, 5, 30), TRUE);

    // === 奥のグリッド描画 ===
    const int grid_size_back = 40;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(100, 0, 100));

    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back) {
        int sy = y - static_cast<int>(scroll_back) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(100, 0, 100));
    }

    // === 前面の太いグリッド ===
    const int grid_size_front = 80;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(200, 40, 200), TRUE);

    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front) {
        int sy = y - static_cast<int>(scroll_front) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(200, 40, 200), TRUE);
    }

    // === 粒子描画 ===
    for (const auto& p : star_particles)
    {
        int a = static_cast<int>(p.alpha);
        if (a <= 0) continue;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, a);

        DrawLine(static_cast<int>(p.pos.x), static_cast<int>(p.pos.y),
            static_cast<int>(p.pos.x), static_cast<int>(p.pos.y + p.length),
            GetColor(180, 130, 255));
    }

    // === ノイズエフェクト ===
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    for (int i = 0; i < 5; ++i) {
        if (rand() % 60 == 0) {
            int nx = rand() % D_WIN_MAX_X;
            int ny = rand() % D_WIN_MAX_Y;
            DrawBox(nx, ny, nx + 3, ny + 3, GetColor(255, 100, 50), TRUE);
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void Stage3::ScrollEffectUpdate(float delta)
{
        scroll_back -= delta * 220.2f;   // 奥グリッドは遅く
        scroll_front -= delta * 420.0f;  // 前面グリッドは速く
        time += delta;                 // ノイズ等に使うタイマー

        // 粒子の更新
        for (auto& p : star_particles)
        {
            p.pos.y += p.velocity.y * delta;
            p.alpha -= delta * 30.0f;
        }

        star_particles.erase(
            std::remove_if(star_particles.begin(), star_particles.end(),
                [](const StarParticle& p)
                { return (p.pos.y > D_WIN_MAX_Y || p.alpha <= 0); }),
            star_particles.end());

        // 粒子追加（負荷軽減のため毎フレーム追加しない）
        if (star_particles.size() < 60)
        {
            StarParticle p;
            p.pos = Vector2D(GetRand(D_WIN_MAX_X), GetRand(D_WIN_MAX_Y));
            p.velocity = Vector2D(0, 60.0f + GetRand(30));
            p.alpha = 100.0f + GetRand(100);
            p.length = 10.0f + GetRand(10);
            p.life = 2.0f + GetRand(100) / 50.0f;
            star_particles.push_back(p);
        }
}

void Stage3::HandleZako1_LR(float delta)
{
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    float spawn_interval = my_max(2.0f - stage_timer / 5.0f, 0.5f);

    if (enemy_spawn_timer < spawn_interval) return;

    const int num = 3;
    const float spacing = 60.0f;
    const float appear_y = -100.0f;
    const float target_y = 300.0f;

    bool from_left = (enemy_group_index % 2 == 0);
    float base_x = from_left ? 60.0f : (D_WIN_MAX_X - 60.0f);
    float dx = from_left ? spacing : -spacing;
    float end_base_x = from_left ? D_WIN_MAX_X / 2 + 120.0f : D_WIN_MAX_X / 2 - 240.0f;

    for (int i = 0; i < num; ++i)
    {
        Vector2D appear_pos(base_x + dx * i, appear_y);
        Vector2D end_pos(end_base_x + spacing * i, target_y);
        float delay = i * 0.5f;

        auto zako = objm->CreateObject<Zako6>(appear_pos);
        zako->SetMode(ZakoMode::Zako3);
        zako->SetAppearParams(appear_pos, end_pos, 1.3f + delay, from_left);
    }

    enemy_group_index++;
    enemy_spawn_timer = 0.0f;
}
void Stage3::HandleZako1_Center(float delta)
{
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    const float spawn_interval = 2.0f;
    if (enemy_spawn_timer < spawn_interval) return;

    const int num = 3;
    const float spacing = 60.0f;
    const float appear_y = -100.0f;
    const float target_y = 250.0f;

    float base_x = D_WIN_MAX_X / 2 + 180.0f;
    float dx = -spacing;
    float end_base_x = D_WIN_MAX_X / 2 - spacing;

    for (int i = 0; i < num; ++i)
    {
        float x = base_x + dx * i;
        float end_x = end_base_x + spacing * i;

        Vector2D appear_pos(x, appear_y);
        Vector2D end_pos(end_x, target_y);
        float delay = i * 0.2f;

        auto zako = objm->CreateObject<Zako6>(appear_pos);
        zako->SetMode(ZakoMode::Zako3);
        zako->SetAppearParams(appear_pos, end_pos, 1.3f + delay, true);
    }

    enemy_group_index++;
    enemy_spawn_timer = 0.0f;
}

void Stage3::HandleZako2_Upward(float delta)
{
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    const float spawn_interval = 4.0f;
    if (zako2_spawn_timer < spawn_interval) return;

    const int num = 3;
    const float spacing = 80.0f;
    const float base_y = D_WIN_MAX_Y + 50.0f;
    const float target_y = D_WIN_MAX_Y / 2;
    const float delay_per = 0.3f;

    float start_x = D_WIN_MAX_X / 2 - spacing;

    for (int i = 0; i < num; ++i)
    {
        float x = start_x + spacing * i;
        float delay = 2.0f + delay_per * i;

        Vector2D appear_pos(x, base_y);
        Vector2D target_pos(x, target_y);

        auto zako = objm->CreateObject<Zako6>(appear_pos);
        zako->SetMode(ZakoMode::Zako2);
        zako->SetAppearParams(appear_pos, target_pos, delay, true);
    }

    zako2_spawn_timer = 0.0f;
}
void Stage3::SpawnBossAndItems()
{
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    if (boss2_spawned) return;

    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 - 60, 120));
    objm->CreateObject<Shield>(Vector2D(D_WIN_MAX_X / 2 + 60, 120));

    boss2 = objm->CreateObject<Boss2>(Vector2D(670, -200));
    boss2_spawned = true;
    is_warning = false;
}





