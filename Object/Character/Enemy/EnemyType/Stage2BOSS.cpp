#include "Stage2BOSS.h"
#include"BossRotatingPart.h"
#include "../../../../Utility/ResourceManager.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../Shot/EnemyShot/EnemyShot2.h"
#include "BossRotatingPart.h"

Stage2Boss::Stage2Boss()
{}

Stage2Boss::~Stage2Boss()
{}

void Stage2Boss::Initialize()
{
    hp = 100;
    box_size = 50;
    z_layer = 2;
    enemy_type = ENE_ZAKO1;

    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    location = { 640.0f, 360.0f }; // ‰æ–Ê’†‰›
    velocity = { 0.0f, 0.0f };

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images = rm->GetImages("Resource/Image/Object/Enemy/Boss/s1_Boss/anime_enemy75_a_swapped.png", 6, 6, 1, 40, 40);
    image = images[0];

    auto* manager = Singleton<GameObjectManager>::GetInstance();
    for (int i = 0; i < 6; ++i)
    {
        float offset = i * 2.0f * 3.1415926f / 6.0f; // “™ŠÔŠui120“x‚¸‚Âj
        auto part = manager->CreateObject<BossRotatingPart>(this->GetLocation());
        part->SetUp(this, 300.0f, offset); // radius=80px, Šp“x‚¸‚ç‚·
    }


}

void Stage2Boss::Update(float delta_second)
{
    if (hp <= 0)
    {
        is_alive = false;
        is_destroy = true;

        auto* manager = Singleton<AnimationManager>::GetInstance();
        int anim_id = manager->PlayerAnimation(EffectName::eExprotion, location, 0.05f, false);
        manager->SetScale(anim_id, 1.0f);
    }

   /* Shot(delta_second);*/
    GameObjectBase::AnimationControl(delta_second, images, { 0 }, 5.0f);
    __super::Update(delta_second);
}

void Stage2Boss::Draw(const Vector2D& screen_offset) const
{
    DrawRotaGraph(location.x, location.y, 4.0f, 0.0f, image, TRUE);
}

void Stage2Boss::Finalize()
{}

void Stage2Boss::Shot(float delta_second)
{
    if (!player) return;

    shot_timer += delta_second;

    if (shot_timer >= 1.0f)
    {
        Vector2D dir = player->GetLocation() - location;
        float len = dir.Length();
        if (len > 0) dir /= len;

        auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot2>(location);
        shot->SetVelocity(dir);

        shot_timer = 0.0f;
    }
}
