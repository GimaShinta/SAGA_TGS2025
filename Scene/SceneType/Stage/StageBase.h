#pragma once

#include "../../../Object/Character/Player/Player.h"
#include "../../../Utility/ProjectConfig.h"

class StageBase {
protected:
    bool is_clear = false;
    bool is_over = false;

public:
    StageBase(Player* player) : player(player) {}
    virtual ~StageBase() {}
    virtual void Initialize() = 0;
    virtual void Update(float delta) = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;
    virtual bool IsFinished() = 0;
    virtual bool IsClear() = 0;
    virtual bool IsOver() = 0;
    virtual StageBase* GetNextStage(Player* player) = 0;

protected:
    class Player* player; // 共有するプレイヤー情報

protected:
    float bg_scroll_speed_layer1 = 100.0f;  // 背面のスクロール速度
    float bg_scroll_speed_layer2 = 200.0f;  // 前面のスクロール速度
    float bg_scroll_offset_layer1 = 0.0f;
    float bg_scroll_offset_layer2 = 0.0f;
    bool is_scroll_enabled = true;

public:
    //スクロール量
    virtual void UpdateBackgroundScroll(float delta)
    {
        if (is_scroll_enabled) 
        {
            bg_scroll_offset_layer1 -= bg_scroll_speed_layer1 * delta;
            bg_scroll_offset_layer2 -= bg_scroll_speed_layer2 * delta;

            if (bg_scroll_offset_layer1 >= D_WIN_MAX_Y) bg_scroll_offset_layer1 -= D_WIN_MAX_Y;
            if (bg_scroll_offset_layer2 >= D_WIN_MAX_Y) bg_scroll_offset_layer2 -= D_WIN_MAX_Y;
        }
    }

    virtual void DrawScrollBackground() const = 0; // 各ステージで具体実装



};