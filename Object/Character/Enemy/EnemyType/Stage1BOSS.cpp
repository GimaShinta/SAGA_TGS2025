#include "Stage1BOSS.h"
#include "../../Player/Player.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../../../Utility/ScoreData.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

// �R���X�g���N�^�i�����_���������j
Stage1Boss::Stage1Boss()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

// �f�X�g���N�^
Stage1Boss::~Stage1Boss()
{}

void Stage1Boss::Initialize()
{
    enemy_type = ENE_ZAKO1;
    z_layer = 2;
    box_size = 12;
    hp = 2000;

    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;
    start_location = location;
    is_returning = false;
    has_shot = false;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images_a = rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_a.png", 4, 4, 1, 32, 32);
    images_b = rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_b.png", 12, 12, 1, 24, 24);

    images = images_a;
    image = images[0];

    ChangePatternRandomly();

    floating_center_initialized = false; // �������t���OOFF
}

void Stage1Boss::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

    GameObjectBase::AnimationControl(delta_second, images, anim_indices, 10.0f);

    switch (pattern)
    {
        case BossPattern::Entrance:
        {
            const float move_down_duration = 3.0f;   // �~��Ă��鎞�ԁi�b�j
            const float target_y = 150.0f;            // �~��I���Y�ʒu
            const float amplitude_x = 250.0f;         // ���U�蕝
            const float frequency_x = 1.0f;           // ���U�葬��
            const float amplitude_y = 50.0f;          // �c��炬��
            const float frequency_y = 2.0f;           // �c��炬����

            if (pattern_timer < move_down_duration && location.y < target_y)
            {
                // ���ɍ~���t�F�[�Y
                velocity = { 0, 100 };
                location += velocity * delta_second;

                // �~�肷���h�~
                if (location.y > target_y)
                {
                    location.y = target_y;
                }
            }
            else
            {
                // ���V�t�F�[�Y

                // ���V����W����x����������
                if (!floating_center_initialized)
                {
                    floating_center_x = location.x; // �~��I��莞��X���W����ɂ���
                    floating_center_y = target_y;   // �Œ��Y���W����ɂ���i�K�v�Ȃ� location.y �ł��j
                    floating_center_initialized = true;
                }

                float t = pattern_timer - move_down_duration;

                // ���̓T�C���g�ō��E�ɂ����i���X�^�[�g�j
                location.x = floating_center_x + amplitude_x * sinf(t * frequency_x + 3.14f);

                // �c�͂����㉺�ɕ��V
                location.y = floating_center_y + amplitude_y * sinf(t * frequency_y);

                velocity = { 0, 0 };
            }

            break;
        }
    }

    location += velocity * delta_second;

    if (hp <= 0)
    { 
        is_alive = false;
        is_destroy = true;
      

        AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
        anim_id = manager->PlayerAnimation(EffectName::eExprotion, location, 0.05f, false);
        manager->SetScale(anim_id, 0.5f);

        Singleton<ScoreData>::GetInstance()->SetScoreData(100);
    }

    __super::Update(delta_second);


}

void Stage1Boss::Draw(const Vector2D& screen_offset) const
{
    DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
    DrawRotaGraph(location.x, location.y, 5.0f, 0.0f, image, TRUE);
    DrawFormatString(location.x - 10, location.y - 40, GetColor(255, 255, 255), "%.0f", hp);
}

void Stage1Boss::Finalize()
{}

void Stage1Boss::Shot(float delta_second)
{
    shot_timer += delta_second;

    if (shot_timer >= 2.0f && player)
    {
        Vector2D dir = player->GetLocation() - location;
        float len = dir.Length();
        if (len > 0) dir /= len;

        auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot2>(location);
        shot->SetVelocity(dir);

        shot_timer = 0.0f;
    }
}

void Stage1Boss::ChangePatternRandomly()
{
}

bool Stage1Boss::GetIsAlive() const
{
    return is_alive;
}

void Stage1Boss::SetPattern(BossPattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f;
    has_shot = false;
    after_shot_timer = 0.0f;
    floating_center_initialized = false; // �p�^�[���ؑ֎��Ɋ���Z�b�g

    switch (pattern)
    {
        case BossPattern::Entrance:
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            break;
    }

    image = images[0];
}

