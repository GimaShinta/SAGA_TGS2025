#include "FollowBeam.h"
#include "DxLib.h"
#include "../../../../Object/GameObjectManager.h"

void FollowBeam::Initialize()
{
    z_layer = 2;
    is_destroy = false;

    box_size = Vector2D(8.0f, 8.0f);  // �����蔻��T�C�Y
    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemyShot;
    collision.hit_object_type.push_back(eObjectType::ePlayer);
}

void FollowBeam::SetFollowTarget(GameObjectBase* target, const Vector2D& direction)
{
    follow_target = target;
    dir = direction;
}

void FollowBeam::SetBeamTarget(Vector2D target_position)
{
    target_pos = target_position;
    Vector2D delta = target_pos - location;
    length = delta.Length();
}

void FollowBeam::Update(float delta_second)
{
    elapsed_time += delta_second;
    if (elapsed_time >= life_time)
    {
        is_destroy = true;
        return;
    }

    if (follow_target)
    {
        location = follow_target->GetLocation();  // ���{�̓p�[�c�ɒǏ]
    }

    if (look_target)
    {
        Vector2D to = look_target->GetLocation() - location;
        float len = to.Length();
        if (len > 0.01f)
        {
            dir = to / len;
            length = len;  // �I�v�V�����ŐL�k�\
        }
    }

    __super::Update(delta_second);

    // �����蔻��`�F�b�N�i����ɉ��z�q�b�g�{�b�N�X��ݒu�j
    constexpr int hitbox_count = 10;
    Vector2D beam_origin = location;

    for (int i = 0; i < hitbox_count; ++i)
    {
        float t = (i + 1) / static_cast<float>(hitbox_count + 1);
        Vector2D check_pos = beam_origin + dir * (length * t);

        BoxCollision temp_collision;
        temp_collision.point[0] = check_pos - Vector2D(6.0f, 6.0f);
        temp_collision.point[1] = check_pos + Vector2D(6.0f, 6.0f);
        temp_collision.object_type = collision.object_type;
        temp_collision.is_blocking = true;
        temp_collision.hit_object_type = collision.hit_object_type;

        for (GameObjectBase* obj : Singleton<GameObjectManager>::GetInstance()->GetGameObjects())
        {
            const auto& obj_col = obj->GetCollision();
            if (obj_col.object_type == eObjectType::ePlayer)
            {
                BoxCollision obj_copy = obj_col;
                obj_copy.point[0] += obj->GetLocation() - obj->GetBoxSize();
                obj_copy.point[1] += obj->GetLocation() + obj->GetBoxSize();

                if (IsCheckCollision(temp_collision, obj_copy))
                {
                    obj->OnHitCollision(this);
                    this->OnHitCollision(obj);
                    return;
                }
            }
        }
    }
}

void FollowBeam::Draw(const Vector2D& screen_offset) const
{
    Vector2D start = location + screen_offset;
    Vector2D end = start + dir * length;

    SetDrawBlendMode(DX_BLENDMODE_ADD, 160);
    DrawLine(start.x, start.y, end.x, end.y, GetColor(100, 255, 255), 3);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // �f�o�b�O�p�̃q�b�g�~
    DrawCircle(static_cast<int>(location.x), static_cast<int>(location.y), static_cast<int>(box_size.x), GetColor(255, 255, 0), FALSE);
}

void FollowBeam::OnHitCollision(GameObjectBase* hit_object)
{
    // �K�v�Ȃ瓖���莞����
}
