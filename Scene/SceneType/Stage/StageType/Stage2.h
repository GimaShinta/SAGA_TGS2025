#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"  // Zako1クラスをインクルード

class Stage2 :
    public StageBase
{
private:
    class Shot* shot;
    class Beam* beam;
    class Zako4* zako4;
    class Zako* zako;
    class Stage2BOSS* boss;
    std::vector<EnemyBase*> enemy_list;
    float stage_timer = 0.0f; // 経過時間（秒）
    bool zako4_spawned = false;
    bool zako5_spawned = false;

    // 敵の出現タイマー
    float enemy_spawn_timer = 0.0f;
    int stage = 2;
    int brend = 0;
    bool is_warning = false;   // 警告表示中かどうか
    bool is_brend = false;     // ブレンド演出中かどうか

public:
    Stage2(Player* player);
    ~Stage2();

    void Initialize() override;
    void Finalize() override;
    void Update(float delta) override;
    void Draw() override;
    bool IsFinished() override;
    bool IsClear() override;
    bool IsOver() override;
    StageBase* GetNextStage(Player* player) override;

private:
    bool finished = false;
    float scene_timer = 0.0f;  // 演出や状態遷移用タイマー

    // 敵出現処理
    void EnemyAppearance(float delta);

    // 背景スクロール
    void DrawScrollBackground() const override;
};
