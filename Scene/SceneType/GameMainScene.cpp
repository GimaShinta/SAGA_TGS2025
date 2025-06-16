#include "GameMainScene.h"
#include "../../Scene/SceneType/Stage/StageType/Stage1.h"
#include "../../Scene/SceneType/Stage/StageType/Stage2.h"
#include <algorithm>   // std::min を使うために必要
#include "Stage/StageType/Stage3.h"
#include "Stage/StageType/Stage4.h"



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
    player = objm->CreateObject<Player>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) + 220.0f));


    current_stage = new Stage3(player);

    current_stage->Initialize();

    // BGM読み込み（初回のみ）
    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    stage_bgm1 = rm->GetSounds("Resource/sound/bgm/stage/Magical World.mp3");
    stage_bgm3 = rm->GetSounds("Resource/sound/bgm/stage/Cryonic Pulse.mp3");

    //サイドパネル画像
    obi_handle = rm->GetImages("Resource/Image/BackGround/Main/Obi_1.png")[0];

    //フォント
    font_digital = CreateFontToHandle("DS-Digital", 28, 6, DX_FONTTYPE_ANTIALIASING);
   // font_digital = CreateFontToHandle("Orbitron", 28, 6, DX_FONTTYPE_ANTIALIASING);
    //font_orbitron = CreateFontToHandle("Orbitron", 22, 6, DX_FONTTYPE_ANTIALIASING);
    font_orbitron = CreateFontToHandle("DS-Digital", 28, 6, DX_FONTTYPE_ANTIALIASING);
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

    //// 左の黒帯
    //DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    //// 右の黒帯
    //DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // === 左の黒帯 ===
    DrawGraph(0, 0, obi_handle, TRUE);

    // === 右の黒帯（左右反転）===
    DrawTurnGraph(D_WIN_MAX_X - 290, 0, obi_handle, TRUE);


    // === 左のサイドパネル（サイバー風） ===
    {
        int left_x1 = 0;
        int left_x2 = (D_WIN_MAX_X / 2) - 350;
        int panel_color = GetColor(10, 10, 30);
        int neon_color = GetColor(0, 255, 255);

        // 背景
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(left_x1, 0, left_x2, D_WIN_MAX_Y, panel_color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

        // 上下ネオンライン
        DrawLine(left_x1, 0, left_x2, 0, neon_color);
        DrawLine(left_x1, D_WIN_MAX_Y - 1, left_x2, D_WIN_MAX_Y - 1, neon_color);

        // スキャンライン風アニメ
        int scan_y = (GetNowCount() / 5) % D_WIN_MAX_Y;
        DrawLine(left_x1, scan_y, left_x2, scan_y, GetColor(0, 150, 255));
    }

    // === 右のサイドパネル（サイバー風） ===
    {
        int right_x1 = (D_WIN_MAX_X / 2) + 350;
        int right_x2 = D_WIN_MAX_X;
        int panel_color = GetColor(10, 10, 30);
        int neon_color = GetColor(0, 255, 255);

        // 背景
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(right_x1, 0, right_x2, D_WIN_MAX_Y, panel_color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

        // 上下ネオンライン
        DrawLine(right_x1, 0, right_x2, 0, neon_color);
        DrawLine(right_x1, D_WIN_MAX_Y - 1, right_x2, D_WIN_MAX_Y - 1, neon_color);

        // スキャンライン風アニメ
        int scan_y = (GetNowCount() / 5) % D_WIN_MAX_Y;
        DrawLine(right_x1, scan_y, right_x2, scan_y, GetColor(0, 150, 255));
    }



    DrawFormatString(D_WIN_MAX_X - 140, 0, GetColor(255, 255, 255), "Life:%d", player->life);

    // ==== 合計スコア（キルログの上） ====
  {
    ScoreData* score = Singleton<ScoreData>::GetInstance();
    const auto& all_scores = score->GetScoreData();

    float total_score = 0.0f;
    for (float s : all_scores)
    {
        total_score += s;
    }

    // 表示位置とサイズ
    int score_box_x = 30;
    int score_box_y = D_WIN_MAX_Y - 640;
    int score_box_w = 220;
    int score_box_h = 32;

    // 点滅フレームカラー
    int pulse = static_cast<int>(GetNowCount() % 100) > 50 ? 255 : 100;
    int frame_color = GetColor(pulse, 255, pulse);

    // 背景と枠
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    //DrawBox(score_box_x, score_box_y, score_box_x + score_box_w, score_box_y + score_box_h, GetColor(10, 10, 30), TRUE);
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    //DrawBox(score_box_x, score_box_y, score_box_x + score_box_w, score_box_y + score_box_h, frame_color, FALSE);

    // スコアテキスト
    DrawFormatStringToHandle(score_box_x - 20, score_box_y + 8, frame_color, font_digital, "TOTAL SCORE");
    DrawFormatStringToHandle(score_box_x, score_box_y + 40, frame_color, font_digital, "%.0f", total_score);

    // 通常描画に戻す（念のため）
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
   }


    // ==== スコアログ（左下） ====
    int log_base_x = 30;
    int log_base_y = D_WIN_MAX_Y - 400;
    int line_height = 40;

    int count = static_cast<int>(score_logs.size());
    for (int i = 0; i < count; ++i)
    {
        const auto& log = score_logs[count - 1 - i];
        int draw_y = log_base_y + static_cast<int>(i * line_height + log.y_offset);

       // DrawBox(log_base_x - 10, draw_y - 2, log_base_x + 200, draw_y + 18, GetColor(10, 10, 30), TRUE);
        DrawLine(log_base_x - 10, draw_y - 2, log_base_x + 200, draw_y - 2, GetColor(0, 255, 255)); // 上ライン
        DrawFormatStringToHandle(log_base_x, draw_y, GetColor(0, 255, 255), font_digital, "%s", log.text.c_str());

    }

    // ==== LIFE - STOCK 表示（右上、Digital風＋残機アイコン） ====
    if (player)
    {
        int life_box_x = D_WIN_MAX_X - 230;  
        int life_box_y = 80;                 
        int life_box_w = 200;
        int life_box_h = 60;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(life_box_x, life_box_y, life_box_x + life_box_w, life_box_y + life_box_h, GetColor(10, 10, 30), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawBox(life_box_x, life_box_y, life_box_x + life_box_w, life_box_y + life_box_h, GetColor(0, 255, 255), FALSE);

        // ラベル
        DrawStringToHandle(life_box_x + 10, life_box_y + 4, "LIFE - STOCK", GetColor(0, 255, 255), font_orbitron);

        // 残機表示（△を横に並べる）
        int icon_base_x = life_box_x + 20;
        int icon_y = life_box_y + 32;
        int icon_size = 14;
        int icon_gap = 20;

        for (int i = 0; i < player->life; ++i)
        {
            int x = icon_base_x + i * icon_gap;

            // △ 三角形（小型ドローン風）
            DrawTriangle(
                x, icon_y + icon_size,           // 下
                x + icon_size / 2, icon_y,       // 上
                x + icon_size, icon_y + icon_size, // 下
                GetColor(255, 100, 100), TRUE     // 塗りつぶし
            );
        }
    }


    // ==== 必殺技ゲージ（LIFEの下、点滅＆レイアウト調整済） ====
    if (player)
    {
        float rate = player->GetChargeRate(); // 0.0 ～ 1.0
        int gauge_x = D_WIN_MAX_X - 230;
        int gauge_y = 150; 
        int gauge_w = 200;
        int gauge_h = 50;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(gauge_x, gauge_y, gauge_x + gauge_w, gauge_y + gauge_h, GetColor(10, 10, 30), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawBox(gauge_x, gauge_y, gauge_x + gauge_w, gauge_y + gauge_h, GetColor(0, 255, 255), FALSE);

        DrawStringToHandle(gauge_x + 10, gauge_y + 2, "CHARGE", GetColor(0, 255, 255), font_orbitron);

        int bar_x = gauge_x + 10;
        int bar_y = gauge_y + 25;
        int bar_w = gauge_w - 20;
        int bar_h = 12;

        int fill_color = player->CanUseSpecial()
            ? GetColor(0, (GetNowCount() % 100 > 50) ? 255 : 100, 255)  // 点滅
            : GetColor(0, 255, 255); // 通常色

        DrawBox(bar_x, bar_y, bar_x + bar_w, bar_y + bar_h, GetColor(30, 30, 30), TRUE); // 背景
        DrawBox(bar_x, bar_y, bar_x + static_cast<int>(bar_w * rate), bar_y + bar_h, fill_color, TRUE); // 本体
    }

    // ==== SPECIAL READY UI（プレイヤー下に真ん中表示） ====
    if (player && player->CanUseSpecial())
    {
        Vector2D pos = player->GetLocation();
        int ui_x = static_cast<int>(pos.x) - 40;  
        int ui_y = static_cast<int>(pos.y) + 40;
        int pulse = static_cast<int>(GetNowCount() % 100) > 50 ? 255 : 100;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawFormatStringToHandle(ui_x, ui_y, GetColor(255, pulse, pulse), font_orbitron, "Press B!!");
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

    DrawStringToHandle(panel_x + 10, panel_y + 10, "Manual", GetColor(0, 255, 255), font_orbitron);

    DrawStringToHandle(panel_x + 10, panel_y + 35, "Move : L-Stick", GetColor(255, 255, 255), font_orbitron);
    DrawStringToHandle(panel_x + 10, panel_y + 55, "Shot : Space / A", GetColor(255, 255, 255), font_orbitron);
    DrawStringToHandle(panel_x + 10, panel_y + 75, "Beam : B", GetColor(255, 255, 255), font_orbitron);
    DrawStringToHandle(panel_x + 10, panel_y + 95, "Flip : RB / L", GetColor(255, 255, 255), font_orbitron);


    // ==== サイドパネルとの境界に「流れるライン」エフェクト（Stage3は色変更） ====
    {
        const int line_width = 4;
        const int flow_speed = 1;
        const int gap = 70;
        int offset = (GetNowCount() * flow_speed) % gap;

        int pulse = (GetNowCount() % 100 > 50) ? 255 : 100;

        // ==== 色切り替え：Stage3かどうかで分岐 ====
        int line_color = GetColor(0, pulse, 255); // 通常：ネオンブルー

        if (dynamic_cast<Stage3*>(current_stage) != nullptr)
        {
            // Stage3：ボス戦中 → ネオンレッド系に切り替え
            //line_color = GetColor(255, 100, 150);
        }

        // ==== 左サイドライン ====
        int left_x = (D_WIN_MAX_X / 2) - 350;
        for (int y = -gap; y < D_WIN_MAX_Y + gap; y += gap)
        {
            int y0 = y + offset;
            DrawBox(left_x, y0, left_x + line_width, y0 + 20, line_color, TRUE);
        }

        // ==== 右サイドライン ====
        int right_x = (D_WIN_MAX_X / 2) + 350 - line_width;
        for (int y = -gap; y < D_WIN_MAX_Y + gap; y += gap)
        {
            int y0 = y + offset;
            DrawBox(right_x, y0, right_x + line_width, y0 + 20, line_color, TRUE);
        }
    }



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
    if (font_digital != -1) {
        DeleteFontToHandle(font_digital);
        font_digital = -1;
    }
    if (font_orbitron != -1) {
        DeleteFontToHandle(font_orbitron);
        font_orbitron = -1;
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

