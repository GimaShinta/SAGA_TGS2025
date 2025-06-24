#pragma once
#include "../SceneBase.h"
#include "../../Utility/ScoreData.h"
#include <vector>
#include <string>

struct ScoreEntry
{
    float score;
    std::string date;
};

class RankingScene : public SceneBase
{
private:
    int font;
    std::vector<ScoreEntry> entries;
    float total_score = 0.0f;
    int wait_timer = 0;
    int start_time = 0;
    bool confirm_reset = false;
    int cursor_index = 0;  // カーソル位置の追加


    bool LoadRankingFromFile();
    void SaveRankingToFile();
    void ResetRankingFile();
    std::string GetTodayString();
    void DrawNeonText(int x, int y, const char* text, int baseColor, int glowColor, int glowPulse);

public:
    RankingScene();
    ~RankingScene();

    void Initialize() override;
    eSceneType Update(float delta_second) override;
    void Draw() override;
    void Finalize() override;
    eSceneType GetNowSceneType() const override;
};
