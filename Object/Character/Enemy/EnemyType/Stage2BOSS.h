#pragma once

#include "../EnemyBase.h"
#include "../../Player/Player.h"

class Stage2Boss : public EnemyBase
{
public:
    Stage2Boss();
    ~Stage2Boss();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

    void SetPlayer(Player* p)
    {
        player = p;
    }
    bool GetIsAlive() const
    {
        return is_alive;
    }

private:
    Player* player = nullptr;

    float shot_timer = 0.0f;
    std::vector<int> images;
    int image = -1;

    bool is_alive = true; // Åö í«â¡

    void Shot(float delta_second);
};
