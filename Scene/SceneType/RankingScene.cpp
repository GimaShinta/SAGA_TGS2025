#include "RankingScene.h"
#include <DxLib.h>
#include <fstream>
#include <algorithm>
#include <ctime>

RankingScene::RankingScene() : font(-1) {}
RankingScene::~RankingScene() {}

void RankingScene::Initialize()
{
    font = CreateFontToHandle("Orbitron", 32, 6, DX_FONTTYPE_ANTIALIASING);
    total_score = 0.0f;
    start_time = GetNowCount();
    confirm_reset = false;

    auto scores = Singleton<ScoreData>::GetInstance()->GetScoreData();
    for (float s : scores)
        total_score += s;

    LoadRankingFromFile();

    // スコアが0より大きければ追加・保存
    if (total_score > 0.0f)
    {
        entries.push_back({ total_score, GetTodayString() });
        std::sort(entries.begin(), entries.end(), [](auto& a, auto& b) {
            return a.score > b.score;
            });
        if (entries.size() > 10)
            entries.resize(10);
        SaveRankingToFile();
    }
}



bool RankingScene::LoadRankingFromFile()
{
    entries.clear();
    std::ifstream file("ranking.txt");

    if (!file.is_open())
    {
        // ファイルがなければ空のまま返す（ダミーデータを入れない）
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        ScoreEntry entry;
        size_t p = line.find(",");
        if (p == std::string::npos) continue;

        entry.score = std::stof(line.substr(0, p));
        entry.date = line.substr(p + 1);
        entries.push_back(entry);
    }

    file.close();
    return true;
}



void RankingScene::SaveRankingToFile()
{
    std::ofstream file("ranking.txt", std::ios::trunc);
    for (const auto& e : entries)
        file << e.score << "," << e.date << "\n";
    file.close();
}

void RankingScene::ResetRankingFile()
{
    std::ofstream file("ranking.txt", std::ios::trunc);  // 空ファイル書き込み
    file.close();
    LoadRankingFromFile();
}


std::string RankingScene::GetTodayString()
{
    time_t now = time(nullptr);
    tm t;
    localtime_s(&t, &now);
    char buf[32];
    sprintf_s(buf, "%04d-%02d-%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
    return std::string(buf);
}

void RankingScene::DrawNeonText(int x, int y, const char* text, int baseColor, int glowColor, int glowPulse)
{
    int pulse = (GetNowCount() % 100 > 50) ? glowPulse : 255;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, pulse);
    DrawStringToHandle(x + 2, y + 2, text, glowColor, font);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    DrawStringToHandle(x, y, text, baseColor, font);
}

eSceneType RankingScene::Update(float delta)
{
    wait_timer += static_cast<int>(delta * 60);
    InputManager* input = Singleton<InputManager>::GetInstance();

        if (input->GetKeyDown(KEY_INPUT_DOWN) || input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN)) // 十字↓
            cursor_index = (cursor_index + 1) % entries.size();

        if (input->GetKeyDown(KEY_INPUT_UP) || input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))   // 十字↑
            cursor_index = (cursor_index + entries.size() - 1) % entries.size();

        if (input->GetKeyDown(KEY_INPUT_Z) || input->GetKeyDown(KEY_INPUT_RETURN) || input->GetButtonDown(XINPUT_BUTTON_B))
            return eSceneType::eTitle;

        if (input->GetKeyDown(KEY_INPUT_X) || input->GetButtonDown(1))
            confirm_reset = true;
    

        if (input->GetKeyDown(KEY_INPUT_Z) || input->GetButtonDown(XINPUT_BUTTON_Y))
        {
            ResetRankingFile();
            confirm_reset = false;
        }
        else if (input->GetKeyDown(KEY_INPUT_X) || input->GetButtonDown(1))
        {
            confirm_reset = false;
        }


    //if (wait_timer > 30)
    //{
    //    if (input->GetKeyDown(KEY_INPUT_Z) || input->GetKeyDown(KEY_INPUT_RETURN) || input->GetButtonDown(0))
    //        return eSceneType::eTitle;

    //    if (input->GetKeyDown(KEY_INPUT_X) || input->GetButtonDown(1))
    //        confirm_reset = true;
    //}

    return GetNowSceneType();
}

void RankingScene::Draw()
{
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(10, 10, 30), TRUE);

    for (int y = 0; y < D_WIN_MAX_Y; y += 4)
    {
        int bright = 10 + (y + GetNowCount() / 3) % 40;
        DrawLine(0, y, D_WIN_MAX_X, y, GetColor(0, bright, bright));
    }

    int cx = D_WIN_MAX_X / 2;
    int elapsed = GetNowCount() - start_time;
    float scale = (elapsed < 60) ? 1.5f - (elapsed / 60.0f) * 0.5f : 1.0f;

    if (entries.empty())
    {
        DrawNeonText(cx - 100, 300, "NO DATA", GetColor(100, 100, 100), GetColor(0, 100, 255), 120);
        return;
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawExtendStringToHandle(cx - 180 * scale, 50, scale, scale, "HIGH SCORE RANKING", GetColor(0, 255, 255), font);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // 整列描画
    int panel_w = 640;
    int bx = cx - panel_w / 2;
    int rank_x = bx + 30;
    int score_x = bx + 130;
    int date_x = bx + 400;

    for (int i = 0; i < static_cast<int>(entries.size()); ++i)
    {
        const auto& e = entries[i];
        int y = 140 + i * 48;

        // 背景
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawBox(bx, y - 4, bx + panel_w, y + 40, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawBox(bx, y - 4, bx + panel_w, y + 40, GetColor(0, 255, 255), FALSE);

        // 色
        int textColor = (i == 0) ? GetColor(255, 215, 0) : GetColor(255, 255, 255);
        if (i == 0 && GetNowCount() % 100 > 50)
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

        // 整列テキスト
        DrawFormatStringToHandle(rank_x, y, textColor, font, "%2d:", i + 1);
        DrawFormatStringToHandle(score_x, y, textColor, font, "%7.0f", e.score);
        DrawStringToHandle(date_x, y, e.date.c_str(), textColor, font);

        if (i == 0)
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    for (int i = 0; i < static_cast<int>(entries.size()); ++i)
    {
        const auto& e = entries[i];
        int y = 140 + i * 48;

        // 背景枠
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawBox(bx, y - 4, bx + panel_w, y + 40, GetColor(0, 0, 0), TRUE);

        // 選択行なら色変更 or 枠追加
        if (i == cursor_index)
        {
            DrawBox(bx - 4, y - 8, bx + panel_w + 4, y + 44, GetColor(255, 255, 0), FALSE);
            DrawStringToHandle(bx - 30, y, "", GetColor(255, 255, 0), font);  // カーソルアイコン
        }
        else
        {
            DrawBox(bx, y - 4, bx + panel_w, y + 40, GetColor(0, 255, 255), FALSE);
        }

        int textColor = (i == 0) ? GetColor(255, 215, 0) : GetColor(255, 255, 255);
        if (i == 0 && GetNowCount() % 100 > 50)
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

        DrawFormatStringToHandle(rank_x, y, textColor, font, "%2d:", i + 1);
        DrawFormatStringToHandle(score_x, y, textColor, font, "%7.0f", e.score);
        DrawStringToHandle(date_x, y, e.date.c_str(), textColor, font);

        if (i == 0)
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    // --- 操作ガイド ---
    const int msg_y = 640;
    if (confirm_reset)
    {
        DrawNeonText(cx - 260, msg_y,
            "Reset ranking?   A = Yes     B = No",
            GetColor(255, 120, 120), GetColor(255, 0, 0), 180);
    }
    else
    {
        DrawNeonText(cx - 260, msg_y,
            "Press A to return     Y to reset",
            GetColor(100, 255, 255), GetColor(0, 150, 255), 150);
    }


}

void RankingScene::Finalize()
{
    if (font != -1)
    {
        DeleteFontToHandle(font);
        font = -1;
    }
}

eSceneType RankingScene::GetNowSceneType() const
{
    return eSceneType::eRanking;
}
