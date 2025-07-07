#pragma once
#include "../../../../Object/GameObjectBase.h"

class WarningBeam : public GameObjectBase
{
public:
    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;

    void SetUp(const Vector2D& direction, float length, float lifetime);

    void SetFollowTarget(GameObjectBase* target)
    {
        follow_target = target;
    }

    void SetFollowAndLookTarget(GameObjectBase* follow, GameObjectBase* look)
    {
        follow_target = follow;
        look_target = look;
    }

private:
    Vector2D dir;
    float beam_length = 800.0f;
    float elapsed_time = 0.0f;
    float life_time = 0.5f; // 0.5�b�Ŏ�������
    // WarningBeam.h �ɒǉ�
    GameObjectBase* follow_target = nullptr;  // ���{
    GameObjectBase* look_target = nullptr;    // �Ə�

};
