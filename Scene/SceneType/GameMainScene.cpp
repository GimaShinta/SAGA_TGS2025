#include "GameMainScene.h"
#include "../../Scene/SceneType/Stage/StageType/Stage2.h"
#include <algorithm>   // std::min を使うために必要



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

    current_stage = new Stage2(player);
    current_stage->Initialize();
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
                    current_stage = next_stage;
                    current_stage->Initialize();
                }
                else
                {
                    // 最終ステージが終わったらリザルトへ
                    return eSceneType::eTitle;
                }
            }
            else if (current_stage->IsOver() == true)
            {
                current_stage->Finalize();
                delete current_stage;
                current_stage = nullptr;

                // 最終ステージが終わったらリザルトへ
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
        sprintf_s(buf, sizeof(buf), "敵撃破 +%.0f", new_score);

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
        current_stage->Draw();
    }
    int base_x = 30;
    int base_y = 50;
    int line_height = 20;

    int count = static_cast<int>(score_logs.size());
    for (int i = 0; i < count; ++i)
    {
        const auto& log = score_logs[count - 1 - i]; // 上に新しいログ
        int draw_y = base_y + static_cast<int>(i * line_height + log.y_offset);

        DrawFormatString(base_x, draw_y, GetColor(255, 255, 0), "%s", log.text.c_str());
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

