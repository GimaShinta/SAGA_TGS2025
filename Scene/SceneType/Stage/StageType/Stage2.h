#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako5.h"

class Stage2 : public StageBase
{
private:
    class Shot* shot;
    class Beam* beam;
    class Zako6* zako6;
    class Zako4* zako4;
    class Zako* zako;
    class Stage2Boss* boss;
    std::vector<EnemyBase*> enemy_list;

    float stage_timer = 0.0f;
    float enemy_spawn_timer = 0.0f;
    float scene_timer = 0.0f;

    bool finished = false;
    bool is_clear = false;
    bool is_over = false;

    int stage = 2;
    int brend = 0;

    bool zako4_spawned = false;
    bool zako5_spawned = false;

    int zako5_spawn_index = 0;
    const int MAX_ZAKO5_COUNT = 2;
    std::vector<Zako5*> pending_zako5s;
    bool is_waiting_zako5 = true;
    Vector2D location;
    float boss_appearance_timer = 0.0f;
    bool is_player_dead = false;
    float player_death_timer = 0.0f;


    // ----------- 演出用変数 ----------
    enum class Stage2Phase
    {
        Normal,
        Warning,
        BossBump1, // ←追加
        BossBump2, // ←追加
        BossBump3, // ←追加
        BossDescending,
        BossBattle
    };


    Stage2Phase phase = Stage2Phase::Normal;
    float warning_timer = 0.0f;
    bool is_warning = false;

    bool is_brend = false;
    int flash_counter = 0;

public:
    Stage2(Player* player);
    ~Stage2();

    void Initialize() override;
    void Finalize() override;
    void Update(float delta) override;
    void Draw() override;
    bool IsFinished() override;
    void SetFinished() override;              // ステージが終了したかどうか
    bool IsClear() override;
    bool IsOver() override;
    void SetLocation(const Vector2D& pos)
    {
        location = pos;
    }

    StageBase* GetNextStage(Player* player) override;
    StageBase* GetNowStage() override;

private:
    void EnemyAppearance(float delta);
    void DrawScrollBackground() const override;
};
