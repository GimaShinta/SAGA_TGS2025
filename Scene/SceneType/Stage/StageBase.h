#pragma once

#include "../../../Object/Character/Player/Player.h"
#include "../../../Utility/ProjectConfig.h"

// StageBase.h
enum class StageID {
    Stage1,
    Stage2,
    Stage3,
    Stage4,
    Unknown
};

class StageBase {
protected:
    bool is_clear = false;
    bool is_over = false;
    int se;


public:
    StageBase(Player* player) : player(player) {}
    virtual ~StageBase() {}
    virtual void Initialize() = 0;
    virtual void Update(float delta) = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;
    virtual bool IsFinished() = 0;
    virtual void SetFinished() = 0;
    virtual bool IsClear() = 0;
    virtual bool IsOver() = 0;
    virtual StageBase* GetNextStage(Player* player) = 0;
    virtual StageBase* GetNowStage() = 0;

protected:
    StageID stage_id;
public:
    StageID GetStageID() const { return stage_id; }

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

    // StageBase.h の protected 部分に追加
protected:
    // フェード制御
    bool is_fading_in = true;     // 初期状態でフェードイン
    bool is_fading_out = false;   // クリア／オーバー時にフェードアウト開始
    float fade_alpha = 255.0f;    // 0: 完全表示, 255: 完全黒

public:
    // 毎フレーム呼び出すフェード更新
    virtual void UpdateFade(float delta)
    {
        // ステージクリア／オーバーでフェードアウトへ
        if (!is_fading_out && (is_clear || is_over))
        {
            is_fading_out = true;
        }

        // フェードイン中：alpha→0
        if (is_fading_in)
        {
            fade_alpha -= 300.0f * delta;
            if (fade_alpha <= 0.0f)
            {
                fade_alpha = 0.0f;
                is_fading_in = false;
            }
        }
        // フェードアウト中：alpha→255
        else if (is_fading_out)
        {
            fade_alpha += 300.0f * delta;
            if (fade_alpha >= 255.0f)
            {
                fade_alpha = 255.0f;
            }
        }
    }

    // 描画の最後に呼ぶフェード描画
    virtual void DrawFadeOverlay() const
    {
        if (is_fading_in || is_fading_out)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(fade_alpha));
            DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }


};