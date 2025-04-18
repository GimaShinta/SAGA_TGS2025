#pragma once
#include "../StageBase.h"
#include "../../../../Object/Character/Player/Player.h"

class Stage2 :
    public StageBase
{
private:
    class Shot* shot;
    class Beam* beam;

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
};

