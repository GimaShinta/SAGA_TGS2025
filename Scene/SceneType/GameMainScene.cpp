#include "GameMainScene.h"
#include "../../Scene/SceneType/Stage/StageType/Stage1.h"
#include "../../Scene/SceneType/Stage/StageType/Stage2.h"
#include <algorithm>   // std::min を使うために必要
#include "Stage/StageType/Stage3.h"



GameMainScene::GameMainScene() : player(nullptr), current_stage(nullptr), next_scene_timer(0.0f)
{
}

GameMainScene::~GameMainScene()
{
}

// 初期化処理
void GameMainScene::Initialize()
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    player = objm->CreateObject<Player>(Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2));

    current_stage = new Stage3(player);
    current_stage->Initialize();

    // BGM読み込み（初回のみ）
    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    stage_bgm1 = rm->GetSounds("Resource/sound/bgm/stage/Magical World.mp3");
    stage_bgm3 = rm->GetSounds("Resource/sound/bgm/stage/Cryonic Pulse.mp3");

    AnimationManager* anim = Singleton<AnimationManager>::GetInstance();
    anim->LoadAllEffects();

    // ステージ1用BGMを再生
    current_bgm_handle = stage_bgm1;
    ChangeVolumeSoundMem(255 * 60 / 100, current_bgm_handle);
    PlaySoundMem(current_bgm_handle, DX_PLAYTYPE_LOOP);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="delta_second">１フレーム当たりの時間</param>
/// <returns></returns>
eSceneType GameMainScene::Update(float delta_second)
{
    if (current_stage)
    {
        AnimationManager* anim = Singleton<AnimationManager>::GetInstance();
        anim->Update(delta_second);

        current_stage->Update(delta_second);

        if (current_stage->IsFinished())
        {
            if (current_stage->IsClear() == true)
            {
                StageBase* next_stage = current_stage->GetNextStage(player);

                current_stage->Finalize();
                delete current_stage;
                current_stage = nullptr;

                if (next_stage != nullptr)
                {
                    // === ステージの切替とBGM処理 ===
                    current_stage = next_stage;
                    current_stage->Initialize();

                    // ステージ3に到達した場合のみBGM切替
                    if (dynamic_cast<Stage3*>(current_stage) != nullptr)
                    {
                        StopSoundMem(current_bgm_handle); // 現在のBGMを停止
                        current_bgm_handle = stage_bgm3;  // ステージ3用BGMに切り替え
                        ChangeVolumeSoundMem(255 * 60 / 100, current_bgm_handle);
                        PlaySoundMem(current_bgm_handle, DX_PLAYTYPE_LOOP);
                    }
                }
                else
                {
                    return eSceneType::eTitle;
                }
            }
            else if (current_stage->IsOver() == true)
            {
                current_stage->Finalize();
                delete current_stage;
                current_stage = nullptr;

                return eSceneType::eTitle;
            }
        }

    }

    // ======= スコアログのスライド演出更新 =======
    for (auto& log : score_logs)
    {
        if (log.y_offset < 0)
        {
            log.y_offset += delta_second * 40;  // 下にスライド（40px/sec）
            if (log.y_offset > 0)
                log.y_offset = 0; // オーバー補正
        }
    }

    // ======= 新しいスコアの追加検出とログ生成 =======
    ScoreData* score = Singleton<ScoreData>::GetInstance();
    const auto& all_scores = score->GetScoreData();

    while (previous_score_count < all_scores.size())
    {
        float new_score = all_scores[static_cast<size_t>(previous_score_count)];

        // スコアログ用メッセージ作成
        char buf[64];
        sprintf_s(buf, sizeof(buf), "Enemy +%.0f", new_score);

        // ★ 先に最大行数チェック（10行超えたら古いのから削除）
        if (score_logs.size() >= 10)
        {
            score_logs.erase(score_logs.begin()); // 一番古いログを削除
        }

        // 既存ログを下に押し出す
        for (auto& log : score_logs)
        {
            log.y_offset = -20.0f; // スライド開始位置
        }

        // 新しいログを追加（スライドなし）
        ScoreLog new_log;
        new_log.text = buf;
        new_log.y_offset = 0.0f;
        score_logs.push_back(new_log);

        previous_score_count += 1.0f;
    }

	return GetNowSceneType();
}

/// <summary>
/// 描画処理
/// </summary>
/// <returns></returns>
void GameMainScene::Draw()
{
    if (current_stage) 
    {
        AnimationManager* anim = Singleton<AnimationManager>::GetInstance();
        anim->Draw();

        current_stage->Draw();
    }

    DrawFormatString(D_WIN_MAX_X - 150, 0, GetColor(255, 255, 255), "プレイヤーライフ : %d", player->life);

    // ==== スコアログ（左下） ====
    int log_base_x = 30;
    int log_base_y = D_WIN_MAX_Y - 220;
    int line_height = 20;

    int count = static_cast<int>(score_logs.size());
    for (int i = 0; i < count; ++i)
    {
        const auto& log = score_logs[count - 1 - i];
        int draw_y = log_base_y + static_cast<int>(i * line_height + log.y_offset);

        DrawBox(log_base_x - 10, draw_y - 2, log_base_x + 200, draw_y + 18, GetColor(10, 10, 30), TRUE);
        DrawLine(log_base_x - 10, draw_y - 2, log_base_x + 200, draw_y - 2, GetColor(0, 255, 255)); // 上ライン
        DrawFormatString(log_base_x, draw_y, GetColor(0, 255, 255), "%s", log.text.c_str());
    }

// ==== 必殺技ゲージ（右上テキストボックス形式） ====
   // 必殺技ゲージ（右上テキストボックス形式）
    if (player)
    {
        float rate = player->GetChargeRate(); // 0.0?1.0
        int gauge_x = D_WIN_MAX_X - 260;
        int gauge_y = 30;
        int gauge_w = 200;
        int gauge_h = 60; // ラベル＋ゲージの高さをカバー

        // テキストボックス風背景
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(gauge_x, gauge_y, gauge_x + gauge_w, gauge_y + gauge_h, GetColor(10, 10, 30), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawBox(gauge_x, gauge_y, gauge_x + gauge_w, gauge_y + gauge_h, GetColor(0, 255, 255), FALSE);

        // ラベル
        DrawString(gauge_x + 10, gauge_y + 4, "CHARGE", GetColor(0, 255, 255));

        // ゲージ本体
        int bar_x = gauge_x + 80;
        int bar_y = gauge_y + 30;
        int bar_w = gauge_w - 90;
        int bar_h = 14;

        DrawBox(bar_x, bar_y, bar_x + bar_w, bar_y + bar_h, GetColor(30, 30, 30), TRUE); // 背景
        DrawBox(bar_x, bar_y, bar_x + static_cast<int>(bar_w * rate), bar_y + bar_h, GetColor(0, 255, 255), TRUE); // 本体

        // READY! 点滅
        if (player->CanUseSpecial())
        {
            int pulse = static_cast<int>(GetNowCount() % 100) > 50 ? 255 : 100;
            DrawFormatString(bar_x + 10, bar_y - 20, GetColor(255, pulse, pulse), "READY!");
        }
    }


    // ==== 操作UI（右下） ====
    int panel_x = D_WIN_MAX_X - 260;
    int panel_y = D_WIN_MAX_Y - 150;
    int panel_w = 230;
    int panel_h = 130;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(10, 10, 30), TRUE); // 背景
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(0, 255, 255), FALSE); // 枠線

    DrawString(panel_x + 10, panel_y + 10, "【Manual】", GetColor(0, 255, 255));
    DrawString(panel_x + 10, panel_y + 40, "Move : WASD / R-Stick", GetColor(255, 255, 255));
    DrawString(panel_x + 10, panel_y + 60, "Shot : Space", GetColor(255, 255, 255));
    DrawString(panel_x + 10, panel_y + 80, "Beam : B", GetColor(255, 255, 255));
    DrawString(panel_x + 10, panel_y + 100,"Flip : RB / L", GetColor(255, 255, 255));
}


// 終了時処理（使ったインスタンスの削除とか）
void GameMainScene::Finalize()
{
    if (current_stage) 
    {
        current_stage->Finalize();
        delete current_stage;
        current_stage = nullptr;
    }
    if (current_bgm_handle != -1)
    {
        StopSoundMem(current_bgm_handle);
        current_bgm_handle = -1;
    }
}

/// <summary>
/// 現在のシーン情報
/// </summary>
/// <returns>現在はリザルトシーンです</returns>
eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::eGameMain;
}


void GameMainScene::AddScoreLog(const std::string& text)
{
    // 新しいログを上に追加（スライド無し）
    ScoreLog log;
    log.text = text;
    log.y_offset = 0.0f;
    score_logs.push_back(log);

    // 最大10行保持 → 古いものから削除
    if (score_logs.size() >= 10)
    {
        score_logs.erase(score_logs.begin()); // 先頭（古い）を削除
    }

    // 既存ログはスライド開始（下に押し出される）
    for (auto& log : score_logs)
    {
        log.y_offset = -20.0f;  // 毎フレームで補正される
    }


}

