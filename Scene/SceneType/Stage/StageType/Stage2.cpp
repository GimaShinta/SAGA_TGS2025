#include "Stage3.h"
#include "Stage2.h"
#include "Stage1.h"

#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako6.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako5.h"
#include"../../../../Object/Character/Enemy/EnemyType/Stage2BOSS.h"
#include"../../../../Object/Item/Exp/Exp.h"
#include"../../../../Utility/AnimationManager.h"
#include "../../../../Object/Item/PowerUp/PowerUp.h"
#include "../../../../Object/Item/Shield/Shield.h"

#include <cstdlib>
#include <ctime>

Stage2::Stage2(Player* player) : StageBase(player)
{}
Stage2::~Stage2()
{}

void Stage2::Initialize()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // ランダムシード設定
    stage_timer = 0.0f;
    enemy_spawn_timer = 0.0f;
    zako5_spawned = false;

    stage_id = StageID::Stage2;

    //GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    //boss = objm->CreateObject<Stage2Boss>(Vector2D(640.0f, -200.0f)); // 高所
    fade_alpha = 0.0f;

}

void Stage2::Finalize()
{
    for (auto& enemy : enemy_list)
    {
        enemy->SetDestroy();
    }
    enemy_list.clear();

    // ★追加：BossRotatingPart など GameObjectBase*
    for (auto& obj : extra_destroy_list)
    {
        obj->SetDestroy();
    }
    extra_destroy_list.clear();

    if (is_over == true)
    {
        player->SetDestroy();
    }
}


void Stage2::Update(float delta)
{
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->Update(delta);


    if (entry_effect_playing)
    {
        entry_effect_timer += delta;
        if (entry_effect_timer >= 1.0f) // 1秒間表示
        {
            entry_effect_playing = false;
        }
    }

    // 強制的にBossBattleにするテスト
    if (boss != nullptr)
    {
        phase = Stage2Phase::BossBattle;
    }


    // ボス撃破判定（BossBattle中のみ）
    if (phase == Stage2Phase::BossBattle && boss != nullptr && !boss->GetIsAlive())
    {
        boss->SetDestroy();  // 消す場合
        is_clear = true;
    }

    stage_timer += delta;
    EnemyAppearance(delta);
    UpdateBackgroundScroll(delta);

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

    // デバッグ用キー入力
    if (CheckHitKey(KEY_INPUT_N)) finished = true;
    if (CheckHitKey(KEY_INPUT_U)) is_clear = true;
    if (CheckHitKey(KEY_INPUT_I)) is_over = true;

    // タイムで自動クリア

    if (stage_timer >= 120.0f)
    {
        is_clear = true;
    }

    if (player->life == -1)
    {
        is_over = true;
    }

    // プレイヤーの死亡処理
    if (!is_player_dead && player->life == -1)
    {
        is_player_dead = true;
        player_death_timer = 0.0f;

        player->SetDestroy();  // プレイヤーを即削除

        // 爆発アニメーション
        auto* anim_mgr = Singleton<AnimationManager>::GetInstance();
        anim_mgr->PlayerAnimation(EffectName::eExprotion, player->GetLocation(), 0.1f, false);
    }

    // ステージ終了処理
    if (is_clear || is_over)
    {
        for (auto& enemy : enemy_list)
        {
            enemy->SetDestroy();
        }
        enemy_list.clear();

        // フェードアウトを明示的に開始（← これが重要）
        if (!is_fading_out)
        {
            is_fading_out = true;
        }
        scene_timer += delta;
        if (fade_alpha >= 255.0f && scene_timer >= 1.5f)
        {
            finished = true;
        }

    }

    // 死亡後の遷移タイマー処理
    if (is_player_dead)
    {
        player_death_timer += delta;
        if (player_death_timer >= 4.0f)
        {
            is_over = true;
            fade_alpha = 255.0f;

            scene_timer += delta;
            if (scene_timer >= 1.5f)
            {
                finished = true;
            }
        }
    }


    UpdateFade(delta);
    if ((is_clear || is_over) && !glitch_started)
    {
        StartGlitchEffect(); // ← フェードの代わりに開始
    }

    UpdateGlitchEffect(delta);

    if (glitch_done)
    {
        finished = true;
    }

}

void Stage2::Draw()
{
    // 背景塗りつぶし
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);
    DrawScrollBackground();

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Draw();

    // 黒帯
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "STAGE2", GetColor(255, 255, 255));
    DrawFormatString(0, 50, GetColor(255, 255, 0), "Enemy: %d", enemy_list.size());
    // 右側操作説明パネル
    int panel_x = D_WIN_MAX_X - 270;
    int panel_y = 60;
    int panel_w = 250;
    int panel_h = 180;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    // ステージタイマーの表示（右上）

    //DrawFormatString(0, 20, GetColor(255, 255, 255), "Time: %.1f", stage_timer);

  /*  if (is_clear)
    {
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "NEXT STAGE...", GetColor(255, 255, 255));
    }
    else if (is_over)
    {
        DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "ゲームオーバー", GetColor(255, 255, 255));
    }*/


    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->Draw();

    //DrawFormatString(100, 100, GetColor(255, 255, 255), "fade_alpha: %.1f", fade_alpha);


    if (entry_effect_playing)
    {
        float t = entry_effect_timer / 1.0f;
        if (t > 1.0f) t = 1.0f;
        int alpha = static_cast<int>((1.0f - t) * 255);

        for (int i = 0; i < 30; ++i)
        {
            int x = GetRand(D_WIN_MAX_X);
            int y = GetRand(D_WIN_MAX_Y);
            int w = 40 + GetRand(100);
            int h = 5 + GetRand(20);
            int col = GetColor(200 + GetRand(55), 200 + GetRand(55), 255);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            DrawBox(x, y, x + w, y + h, col, TRUE);
        }

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha / 3);
        DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE); // フラッシュ効果
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    DrawGlitchEffect(); // ← 最前面にグリッチ表示


}

bool Stage2::IsFinished()
{
    return finished;
}
void Stage2::SetFinished()
{
    finished = true;
}
bool Stage2::IsClear()
{
    return is_clear;
}
bool Stage2::IsOver()
{
    return is_over;
}

StageBase* Stage2::GetNextStage(Player* player)
{
    return new Stage3(player);
}

StageBase* Stage2::GetNowStage()
{
    return this;
}

void Stage2::EnemyAppearance(float delta)
{
    enemy_spawn_timer += delta;
    const float spawn_interval = 1.0f;
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // --- 0〜10秒：お山フォーメーション（3レーン、重複抑制） ---
    if (stage_timer < 20.0f)
    {
        static int previous_lane = -1;  // 前回のレーン記録用

        if (enemy_spawn_timer >= spawn_interval)
        {
            enemy_spawn_timer = 0.0f;

            // 前回と違うレーンを選ぶ
            int lane;
            do
            {
                lane = std::rand() % 3;  // 0:左, 1:中央, 2:右
            } while (lane == previous_lane);
            previous_lane = lane;

            float base_x;
            if (lane == 0)      base_x = 400.0f;
            else if (lane == 1) base_x = 640.0f;
            else                base_x = 900.0f;

            float base_y = 0.0f;
            float offset_x = 50.0f;
            float offset_y = 30.0f;

            Zako* zako_top = objm->CreateObject<Zako>(Vector2D(base_x, base_y - offset_y));
            zako_top->SetPattern(ZakoPattern::MoveStraight);
            zako_top->SetPlayer(player);
            enemy_list.push_back(zako_top);

            Zako* zako_left = objm->CreateObject<Zako>(Vector2D(base_x - offset_x, base_y + offset_y-70));
            zako_left->SetPattern(ZakoPattern::MoveStraight);
            zako_left->SetPlayer(player);
            enemy_list.push_back(zako_left);

            Zako* zako_right = objm->CreateObject<Zako>(Vector2D(base_x + offset_x, base_y + offset_y-70));
            zako_right->SetPattern(ZakoPattern::MoveStraight);
            zako_right->SetPlayer(player);
            enemy_list.push_back(zako_right);
        }
    }

    // --- 10?15秒：右下がり ＼ の階段状に3体を順番に出現（1回限り） ---
   

    if (stage_timer >= 10.0f && stage_timer < 15.0f && !spawned_stair_done)
    {
        stair_timer += delta;

        const float stair_spawn_interval = 0.6f;  // 出現間隔（秒）

        if (stair_index < 3 && stair_timer >= stair_spawn_interval)
        {
            stair_timer = 0.0f;

            float base_x = 1000.0f;
            float base_y = 40.0f;
            float offset_x = 30.0f;
            float offset_y = 60.0f;

            float x = base_x + stair_index * offset_x;
            float y = base_y + stair_index * offset_y;

            Zako* zako = objm->CreateObject<Zako>(Vector2D(x, y));
            if (zako != nullptr)
            {
                zako->SetPattern(ZakoPattern::LeftMove);
                zako->SetPlayer(player);
                enemy_list.push_back(zako);
            }

            stair_index++;
        }

        if (stair_index >= 3)
        {
            spawned_stair_done = true;
        }
    }

    // --- 15?20秒：右上がり ／ の階段状に3体を順番に出現（1回限り） ---
    

    if (stage_timer >= 15.0f && stage_timer < 20.0f && !spawned_slash_done)
    {
        slash_timer += delta;

        const float slash_spawn_interval = 0.6f;  // 出現間隔（秒）

        if (slash_index < 3 && slash_timer >= slash_spawn_interval)
        {
            slash_timer = 0.0f;

            float base_x = 270.0f;
            float base_y = 200.0f;
            float offset_x = 40.0f;
            float offset_y = -50.0f;

            float x = base_x + slash_index * offset_x;
            float y = base_y + slash_index * offset_y;

            Zako* zako = objm->CreateObject<Zako>(Vector2D(x, y));
            zako->SetPattern(ZakoPattern::RightMove);
            zako->SetPlayer(player);
            enemy_list.push_back(zako);

            slash_index++;
        }

        if (slash_index >= 3)
        {
            spawned_slash_done = true;
        }
    }

    // --- 20〜40秒：Zako5を2体だけ1回出現 ---
    if (stage_timer >= 20.0f && stage_timer < 40.0f && !zako5_spawned)
    {
        enemy_spawn_timer = 0.0f;
        zako5_spawned = true;

        Zako5* left = objm->CreateObject<Zako5>(Vector2D(360.0f, 300.0f));
        left->Initialize();
        left->EnableRectangularLoopMove(false);
        left->SetPlayer(player);
        enemy_list.push_back(left);

        Zako5* right = objm->CreateObject<Zako5>(Vector2D(920.0f, 300.0f));
        right->Initialize();
        right->EnableRectangularLoopMove(false);
        right->SetPlayer(player);
        enemy_list.push_back(right);
    }

    // --- 40〜60秒：お山フォーメーション（ランダム位置） ---
    if (stage_timer >= 42.0f && stage_timer < 55.0f)
    {
        if (enemy_spawn_timer >= spawn_interval)
        {
            enemy_spawn_timer = 0.0f;

            float base_x = 320.0f + std::rand() % 640;
            float base_y = 0.0f;
            float offset_x = 40.0f;
            float offset_y = 30.0f;

            Zako* zako_top = objm->CreateObject<Zako>(Vector2D(base_x, base_y - offset_y));
            zako_top->SetPattern(ZakoPattern::MoveStraight);
            zako_top->SetPlayer(player);
            enemy_list.push_back(zako_top);

            Zako* zako_left = objm->CreateObject<Zako>(Vector2D(base_x - offset_x, base_y + offset_y));
            zako_left->SetPattern(ZakoPattern::MoveStraight);
            zako_left->SetPlayer(player);
            enemy_list.push_back(zako_left);

            Zako* zako_right = objm->CreateObject<Zako>(Vector2D(base_x + offset_x, base_y + offset_y));
            zako_right->SetPattern(ZakoPattern::MoveStraight);
            zako_right->SetPlayer(player);
            enemy_list.push_back(zako_right);
        }
    }

    // --- 60〜110秒：ボス出現（1回のみ） ---
   
    if (!stage2boss_spawned && stage_timer >= 60.0f)
    {
        stage2boss_spawned = true;

        boss = objm->CreateObject<Stage2Boss>(Vector2D(640.0f, -200.0f)); // 高所
        boss->Initialize();
        boss->SetPlayer(player);
        enemy_list.push_back(boss);

        // 回転パーツも GameObjectBase* として破棄対象に登録
        for (auto& part : boss->GetRotatingParts())
        {
            extra_destroy_list.push_back(part);
        }

        phase = Stage2Phase::BossDescending;
        is_warning = false;
    }

}


void Stage2::DrawScrollBackground() const
{
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(20, 20, 40), TRUE);

    const int grid_size_back = 40;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, 100, 255));
    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back)
    {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(0, 100, 255));
    }

    const int grid_size_front = 80;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(180, 0, 255), TRUE);
    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front)
    {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(180, 0, 255), TRUE);
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
