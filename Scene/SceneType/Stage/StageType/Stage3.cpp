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

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    se_warning = rm->GetSounds("Resource/sound/se/battle/Warning.mp3"); // 任意の警告音


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
    font_warning = CreateFontToHandle("Orbitron", 48, 6, DX_FONTTYPE_ANTIALIASING);

    stage_id = StageID::Stage3;
}

void Stage3::Finalize()
{
    // 終了処理
    // 敵リストをすべて削除
    for (auto& enemy : enemy_list)
    {
        enemy->SetDestroy();
    }
    //enemy_list.clear();
    //AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    //manager->RemoveAnimation(anim_id);
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

     //アニメーションが終了したか確認（削除が必要な場合）
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

    if (is_clear && result_ended && !se_warning_played)
    {
        request_stop_bgm = true;
        request_play_warning_se = true;
        se_warning_played = true;
    }

    //if (boss2->is_destroy && boss2 != nullptr)
    //{
   
    //}

    // 遷移タイマー
    //if (se_warning_played && !finished)
    //{
    //    post_result_wait_timer += delta;

    //    if (post_result_wait_timer >= 5.0f)
    //    {
    //        SetFinished();  
    //    }
    //}


}

void Stage3::Draw()
{
    // 背景などの描画
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();

    // === 背景の塗りつぶし ===
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(20, 5, 30), TRUE);

    if (draw_animation_first)  // ← このフラグで順序を切り替える
    {
        DrawScrollBackground(); // 背景 & 背面グリッド（奥）

        // グリッドの裏に描画するタイミング：未生成 or 墜落中
        if (boss2 != nullptr && (!boss2->GetGenerate()))
        {
            objm->DrawBoss();
        }

        DrawFrontGrid();  // 前面グリッド
        manager->Draw();       // 先にアニメーション

        // 通常戦闘中：生成済み & 墜落していない → グリッドの前に描画
        if (boss2 != nullptr && boss2->GetGenerate())
        {
            objm->DrawBoss();
        }

        // オブジェクト描画の前に必ずブレンドモードを戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        objm->DrawWithoutBoss();      // プレイヤー・ザコなど
    }
    else
    {

        // グリッドの裏に描画するタイミング：未生成 or 墜落中
        if (boss2 != nullptr && (!boss2->GetGenerate()))
        {
            objm->DrawBoss();
        }

        DrawScrollBackground(); // 背景 & 背面グリッド（奥）

        DrawFrontGrid();  // 前面グリッド

        // 通常戦闘中：生成済み & 墜落していない → グリッドの前に描画
        if (boss2 != nullptr && boss2->GetGenerate())
        {
            objm->DrawBoss();
        }
        // オブジェクト描画の前に必ずブレンドモードを戻す
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        objm->DrawWithoutBoss();      // プレイヤー・ザコなど
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


    if (warning_state != WarningState::None)
    {
        int y_top = static_cast<int>(band_center_y - band_half_height);
        int y_bottom = static_cast<int>(band_center_y + band_half_height);


        // ★ここでラインを band_half_height > 0 のときだけ描画
        if (band_half_height > 1.0f)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawBox(0, y_top, 1280, y_bottom, GetColor(255, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            DrawLine(0, y_top, 1280, y_top, GetColor(255, 255, 255));
            DrawLine(0, y_bottom, 1280, y_bottom, GetColor(255, 255, 255));
        }

        if (warning_state == WarningState::Displaying)
        {
            DrawStringToHandle(static_cast<int>(warning_text_x), band_center_y - 20,
                "!! WARNING !!", GetColor(255, 255, 255), font_warning);
        }
    }

    //if (is_warning_finished) return;

    //const int cx = 640;  // 画面中央X
    //const int cy = 360;  // 画面中央Y

    //// ===== ノイズ背景（DrawBoxで表現）=====
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

    //const int block_size = 4;  // 1セルのサイズ
    //for (int y = 0; y < 720; y += block_size) {
    //    for (int x = 0; x < 1280; x += block_size) {
    //        int gray = GetRand(255);
    //        int color = GetColor(gray, gray, gray);
    //        DrawBox(x, y, x + block_size, y + block_size, color, TRUE);
    //    }
    //}

    //SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //// ===== フェーズ分岐 =====
    //if (warning_timer < 1.5f) {
    //    // フェーズ1：システムアラート
    //    DrawStringToHandle(cx - 200, cy - 40, ">> SYSTEM ALERT <<", GetColor(255, 255, 255), font_orbitron);
    //}
    //else if (warning_timer < 3.5f) {
    //    // フェーズ2：WARNING点滅
    //    if (((int)(warning_timer * 4)) % 2 == 0) {
    //        DrawStringToHandle(cx - 160, cy - 40, "!! WARNING !!", GetColor(255, 0, 0), font_orbitron);
    //    }
    //}
    //else if (warning_timer < 5.0f) {
    //    // フェーズ3：ボス名タイプ風表示
    //    std::string boss_name = "CODE-Ω: DATALORD";
    //    int chars = (int)((warning_timer - 3.5f) * 15);
    //    if (chars > boss_name.length()) chars = boss_name.length();

    //    std::string visible = boss_name.substr(0, chars);
    //    DrawStringToHandle(cx - 200, cy + 40, visible.c_str(), GetColor(0, 255, 255), font_orbitron );
    //}
}

bool Stage3::IsFinished()
{
    return finished;
}

void Stage3::SetFinished()
{
    finished = true;
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

StageBase* Stage3::GetNowStage()
{
    return this;
}

void Stage3::DisplayWarning(float delta_second)
{
    if (warning_state == WarningState::None) return;

    warning_timer += delta_second;

    switch (warning_state)
    {
    case WarningState::Expanding:
        band_half_height += band_expand_speed * delta_second;
        if (band_half_height >= band_max_half_height)
        {
            band_half_height = band_max_half_height;
            warning_state = WarningState::Displaying;
            warning_timer = 0.0f;
        }
        break;

    case WarningState::Displaying:
        warning_text_x -= warning_scroll_speed * delta_second;
        if (warning_text_x < -200)
            warning_text_x = 1020;
        if (warning_timer >= warning_duration)
        {
            warning_state = WarningState::Shrinking;
        }
        break;

    case WarningState::Shrinking:
        band_half_height -= band_expand_speed * delta_second;
        if (band_half_height <= 0.0f)
        {
            band_half_height = 0.0f;
            warning_state = WarningState::None;
            // ボス登場処理などへ
        }
        break;

    default:
        break;
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

    if (stage_timer < 5.0f)
    {
        SpawnBossAndItems();

        if (!is_zako7_group_spawned)
        {
            const float base_y = -50.0f;
            const float center_y = 320.0f;         // 中央基準（少し上に変更して階段の効果を出す）
            const float delay_step = 0.3f;
            const float step_y = 50.0f;            // 1段あたりのY段差（好きな値に調整可）

            const float left_start_x = 490.0f;
            const float right_start_x = 790.0f;
            const float spacing_x = 30.0f;

            // 左に3体（→中央→左上）
            for (int i = 0; i < 3; ++i)
            {
                float x = left_start_x + spacing_x * i;
                float delay = 1.0f + delay_step * i;
                float target_y = center_y + i * step_y;  // ← 階段状に下へ

                auto zako = objm->CreateObject<Zako6>(Vector2D(x, base_y));
                zako->SetMode(ZakoMode::Zako7);
                zako->SetAppearParams(Vector2D(x, base_y), Vector2D(x, target_y), delay, true);
                zako->SetPlayer(player);
            }

            // 右に3体（→中央→右上）
            for (int i = 0; i < 3; ++i)
            {
                float x = right_start_x - spacing_x * i;
                float delay = 1.0f + delay_step * i;
                float target_y = center_y + i * step_y;

                auto zako = objm->CreateObject<Zako6>(Vector2D(x, base_y));
                zako->SetMode(ZakoMode::Zako7);
                zako->SetAppearParams(Vector2D(x, base_y), Vector2D(x, target_y), delay, false);
                zako->SetPlayer(player);
            }

            is_zako7_group_spawned = true;
        }
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
            zako->SetPlayer(player);
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
            zako->SetPlayer(player);
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
        //    zako->SetPlayer(player);

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
        
        if (scene_timer >= 100.0f)
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

    // タイマー進行
    if (!result_fadeout_started)
        result_timer += delta * 60.0f;
    else
        result_fadeout_timer += delta * 60.0f;

    const int cx = D_WIN_MAX_X / 2;
    const int cy = D_WIN_MAX_Y / 2 - 20;

    // 背景フェード（固定）
    int fade_alpha = (result_timer * 12.0f < 60.0f) ? static_cast<int>(result_timer * 12.0f) : 60;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
    DrawBox(cx - 350, 0, cx + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // フェード／スライド定数
    const int fade_duration = 60;
    const int slide_distance = 60;

    // 表示補助関数
    auto GetAlpha = [&](int delay, bool is_fadeout = false) -> int {
        int t = static_cast<int>(result_fadeout_started ? result_fadeout_timer : result_timer) - delay;
        if (t < 0) return is_fadeout ? 255 : 0;
        if (t >= fade_duration) return is_fadeout ? 0 : 255;
        return is_fadeout ? 255 - (255 * t) / fade_duration : (255 * t) / fade_duration;
        };

    auto GetSlideY = [&](int base_y, int delay, bool is_fadeout = false) -> int {
        int t = static_cast<int>(result_fadeout_started ? result_fadeout_timer : result_timer) - delay;
        if (t < 0) return base_y + (is_fadeout ? 0 : slide_distance);
        if (t >= fade_duration) return base_y + (is_fadeout ? slide_distance : 0);
        int offset = (slide_distance * t) / fade_duration;
        return is_fadeout ? base_y + offset : base_y + slide_distance - offset;
        };

    // スコア集計
    ScoreData* score = Singleton<ScoreData>::GetInstance();
    const auto& scores = score->GetScoreData();
    float base_score = 0.0f;
    for (float s : scores) base_score += s;
    int life_bonus = player->life * 1000;
    total_score = base_score + life_bonus;

    // 表示ライン設定
    struct ResultLine {
        int delay;
        int y_offset;
        std::string label;
        std::string format;
    };

    std::vector<ResultLine> lines = {
        {  30, -80, "RESULT", "" },
        {  70, -20, "BASE SCORE", "BASE SCORE : %.0f" },
        { 110,  20, "LIFE BONUS", "LIFE BONUS : %d" },
        { 160,  60, "TOTAL SCORE", "TOTAL SCORE : %.0f" },
    };

    bool fading_out = result_fadeout_started;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        const auto& line = lines[i];

        // フェードアウト中は delay を逆順にする
        int delay = line.delay;
        if (result_fadeout_started)
        {
            delay = lines.back().delay - line.delay;
        }

        int alpha = GetAlpha(delay, result_fadeout_started);
        int y = GetSlideY(cy + line.y_offset, delay, result_fadeout_started);
        int color = GetColor(255, 255, 255);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        char buf[128];
        if (line.format.empty()) {
            sprintf_s(buf, "%s", line.label.c_str());
        }
        else if (line.label == "BASE SCORE") {
            sprintf_s(buf, line.format.c_str(), base_score);
        }
        else if (line.label == "LIFE BONUS") {
            sprintf_s(buf, line.format.c_str(), life_bonus);
        }
        else if (line.label == "TOTAL SCORE") {
            sprintf_s(buf, line.format.c_str(), total_score);
        }

        int width = GetDrawStringWidthToHandle(buf, strlen(buf), font_digital);
        DrawStringToHandle(cx - width / 2, y, buf, color, font_digital);

        if (line.label == "TOTAL SCORE" && alpha == 255 && !result_fadeout_started) {
            result_displayed = true;
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 表示後のグリッチ待機
    if (result_displayed && !glitch_started && !glitch_done) {
        post_result_wait_timer += delta;
        if (post_result_wait_timer >= 8.0f) {
            glitch_started = true;
            glitch_timer = 0.0f;
        }
    }

    // グリッチ演出
    if (glitch_started && !glitch_done) {
        //for (int i = 0; i < 50; ++i) {
        //    int x = (rand() % 700) + (D_WIN_MAX_X / 2 - 350);
        //    int y = rand() % D_WIN_MAX_Y;
        //    int w = 80 + rand() % 150;
        //    int h = 8 + rand() % 40;
        //    int r = 150 + rand() % 106;
        //    int g = 150 + rand() % 106;
        //    int b = 255;
        //    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
        //    DrawBox(x, y, x + w, y + h, GetColor(r, g, b), TRUE);
        //}

        glitch_timer += delta;
        if (glitch_timer > 2.0f) {
            glitch_done = true;
        }
    }

    // フェードアウト開始
    if (glitch_done && !result_fadeout_started) {
        result_fadeout_started = true;
        result_fadeout_timer = 0.0f;
    }

    // フェードアウト完了で終了
    if (result_fadeout_started && !result_ended) {
        int last_delay = lines.back().delay;
        if (result_fadeout_timer >= fade_duration + last_delay) {
            result_ended = true;
            finished = true;
            // 必要であれば次のシーンへ切り替えなど
        }
    }

    //// フェードアウト完了で暗転フェードイン開始
    //if (result_ended && !black_fade_started) {
    //    black_fade_started = true;
    //    black_fade_timer = 0.0f;
    //}

    //// 黒い暗転フェード
    //if (black_fade_started)
    //{
    //    black_fade_timer += delta;

    //    static int alpha = 0;
    //    if (black_fade_timer >= 0.01f)
    //    {
    //        black_fade_timer = 0.0f;
    //        alpha++;
    //    }
    //    //int alpha = static_cast<int>((black_fade_timer / black_fade_duration) * 255.0f);
    //    if (alpha > 255) alpha = 255;

    //    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    //    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
    //    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //    if (alpha >= 255)
    //    {
    //        finished = true;

    //        // 暗転完了 → 次のシーンへ切り替えなど
    //        // SceneManager::ChangeScene(...);
    //    }
    //}


    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}


void Stage3::DrawScrollBackground() const
{

    // === 奥のグリッド描画 ===
    const int grid_size_back = 40;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(100, 0, 100));

    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back) {
        int sy = y - static_cast<int>(scroll_back) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(100, 0, 100));
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);

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
        scroll_front -= delta * 620.0f;  // 前面グリッドは速く
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
        zako->SetPlayer(player);

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
        zako->SetPlayer(player);
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
        zako->SetPlayer(player);
    }

    zako2_spawn_timer = 0.0f;
}
void Stage3::SpawnBossAndItems()
{
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    if (boss2_spawned) return;

    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 - 60, 120))->SetPlayer(player);
    objm->CreateObject<Shield>(Vector2D(D_WIN_MAX_X / 2 + 60, 120))->SetPlayer(player);

    boss2 = objm->CreateObject<Boss2>(Vector2D(670, -200));
    boss2->SetPlayer(player);
    boss2_spawned = true;
    is_warning = false;
}

void Stage3::DrawFrontGrid() const
{
    const int grid_size_front = 80;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(200, 40, 200), TRUE);

    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front) {
        int sy = y - static_cast<int>(scroll_front) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(200, 40, 200), TRUE);
    }
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void Stage3::StartWarning()
{
    warning_state = WarningState::Expanding;
    warning_timer = 0.0f;
    warning_text_x = 1280.0f;
    band_half_height = 0.0f;
}





