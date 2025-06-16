#include "BossRotatingPart.h"
#include "Stage2BOSS.h"
#include "../../../../Utility/ResourceManager.h"

#include"../../../../Object/Character/Shot/EnemyBeam1.h"
#include"../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include <cmath>

constexpr float PI = 3.1415926f;

void BossRotatingPart::SetUp(Stage2Boss* boss, float radius, float angle_offset)
{
    this->boss = boss;
    this->radius = radius;
    this->angle_offset = angle_offset;
    this->angle = 0.0f;
}


void BossRotatingPart::Initialize()
{
    z_layer = 3;
    box_size = 20;

    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemyPart;

    // �e�ɂ���������i�v���C���[�ɓ�����Ȃ��I�j
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images = rm->GetImages("Resource/Image/Object/Enemy/Boss/s1_Boss/anime_enemy75_a_swapped.png", 6, 6, 1, 40, 40);
}

void BossRotatingPart::Update(float delta_second)
{
    if (!boss) return;

    angle += delta_second * 0.5f;

    float theta = angle + angle_offset;
    Vector2D center = boss->GetLocation();

    location.x = center.x + std::cos(theta) * radius;
    location.y = center.y + std::sin(theta) * radius;

    // �A�j���[�V��������
    anim_timer += delta_second;
    if (anim_timer >= 0.1f) // 0.1�b���ƂɎ��̃t���[����
    {
        image_index = (image_index + 1) % images.size();
        anim_timer = 0.0f;
    }

    shot_timer += delta_second;

    if (shot_timer >= 2.0f)
    {
        shot_timer = 0.0f;

        if (use_beam)
        {
            auto beam = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyBeam1>(location);
            beam->SetVelocity({ 0.0f, 100.0f });
            // �{�X2 or 3 �Ƃ��Č����������ɉ����ČĂѕ���
            beam->SetBoss2(nullptr); // �܂��� beam->SetBoss3(nullptr);
        }
        else
        {
            auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot1>(location);

            Vector2D dir = boss->GetLocation() - location; // �� �C���ς݁I
            float len = dir.Length();
            if (len > 0.0f) dir /= len;

            shot->SetVelocity(dir * 200.0f); // �C�ӂ̑���
        }

    }

    __super::Update(delta_second);
}


void BossRotatingPart::Draw(const Vector2D& screen_offset) const
{
    if (!images.empty())
    {
        DrawRotaGraph(location.x, location.y, 1.5f, angle, images[image_index], TRUE);
    }
    else
    {
        DrawCircle(static_cast<int>(location.x), static_cast<int>(location.y), 20, GetColor(255, 0, 0), TRUE);
    }

    //DrawCircle(static_cast<int>(location.x), static_cast<int>(location.y), box_size, GetColor(255, 0, 0), TRUE); // �� ����
}

void BossRotatingPart::OnHitCollision(GameObjectBase* hit_object)
{
    // �v���C���[�̒e�ɂ����������ď����i�����͖����j
    hit_object->SetDestroy();
}



