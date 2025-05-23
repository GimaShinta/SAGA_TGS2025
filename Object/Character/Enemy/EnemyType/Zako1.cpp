#include "Zako1.h"
#include "../../Player/Player.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../../../Utility/ScoreData.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

// �R���X�g���N�^�i�����_���������j
Zako::Zako()
{
    srand(static_cast<unsigned int>(time(nullptr)));
}

// �f�X�g���N�^
Zako::~Zako()
{}

// ����������
void Zako::Initialize()
{
    enemy_type = ENE_ZAKO1;
    z_layer = 2;
    box_size = 12;
    hp = 0;

    // �Փ˔���̐ݒ�
    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;
    start_location = location;
    is_returning = false;
    has_shot = false;

    // �摜�ǂݍ���
    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images_a = rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_a.png", 4, 4, 1, 32, 32);
    images_b = rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_b.png", 12, 12, 1, 24, 24);

    images = images_a;
    image = images[0];

    // �s���p�^�[���������_���Ɍ���
    ChangePatternRandomly();
}

// �X�V����
void Zako::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

    // �A�j���[�V��������
    GameObjectBase::AnimationControl(delta_second, images, anim_indices, 10.0f);

    // ���݂̍s���p�^�[���ɉ����Ĉړ��E�U�����������s
    switch (pattern)
    {
        case ZakoPattern::MoveStraight:
            velocity = { 0, 120 };
            break;

        case ZakoPattern::RightMove:
            velocity = { 120, 0 };
            break;

        case ZakoPattern::LeftMove:
            velocity = { -120, 0 };
            break;

        case ZakoPattern::ZIgzag:
            // �W�O�U�O�ړ����U��
            velocity.x = sinf(pattern_timer * 1.5f) * 320;
            velocity.y = 100;
            Shot(delta_second);
            break;

        case ZakoPattern::MoveAndStopShoot:
            // ���ʒu�Œ�~��ɍU�����A���
            if (!has_shot)
            {
                if (location.y < 300)
                {
                    velocity = { 0, 100 };
                }
                else
                {
                    velocity = { 0, 0 };
                    shot_timer += delta_second;

                    if (shot_timer >= 2.0f && player)
                    {
                        Shot(delta_second);
                        has_shot = true;
                        shot_timer = 0.0f;
                    }
                }
            }
            else
            {
                velocity = { 0, 0 };
                after_shot_timer += delta_second;

                if (after_shot_timer >= 2.0f)
                {
                    velocity = { 0, -150 }; // ���
                }
            }
            break;

        case ZakoPattern::MoveThenDiagonal:
            // �ŏ��͒��i�A�̂��Ɏ΂߈ړ�
            if (pattern_timer < 1.0f)
            {
                velocity = { 0, 100 };
            }
            else
            {
                velocity = { 100, 100 };
            }
            break;
    }

    // �ʒu�X�V
    location += velocity * delta_second;

    // HP��0�ȉ��Ȃ�j�󏈗�
    if (hp <= 0)
    {
        is_destroy = true;

        // �����G�t�F�N�g�Đ�
        AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
        anim_id = manager->PlayerAnimation(EffectName::eExprotion, location, 0.05f, false);
        manager->SetScale(anim_id, 0.5f); // �X�P�[������

        // �X�R�A���Z
        Singleton<ScoreData>::GetInstance()->SetScoreData(100);
    }

    __super::Update(delta_second);
}

// �`�揈��
void Zako::Draw(const Vector2D& screen_offset) const
{
    // HP�\���i�f�o�b�O�p�\���j
    DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
    DrawRotaGraph(location.x, location.y, 1.5f, 0.0f, image, TRUE);
    DrawFormatString(location.x - 10, location.y - 40, GetColor(255, 255, 255), "%.0f", hp);
}

// �I�������i���g�p�����I�[�o�[���C�h�̂��ߒ�`�j
void Zako::Finalize()
{}

// �G�̍U�������i�v���C���[�̕����ɒe�𔭎ˁj
void Zako::Shot(float delta_second)
{
    shot_timer += delta_second;

    if (shot_timer >= 2.0f && player)
    {
        // �v���C���[�̕������v�Z
        Vector2D dir = player->GetLocation() - location;
        float len = dir.Length();
        if (len > 0) dir /= len;

        // �e����
        auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot2>(location);
        shot->SetVelocity(dir);

        shot_timer = 0.0f;
    }
}

// �s���p�^�[���������_���ɑI��
void Zako::ChangePatternRandomly()
{
    int r = rand() % 6;
    SetPattern(static_cast<ZakoPattern>(r));
}

// �s���p�^�[���̐ݒ�Ə�����
void Zako::SetPattern(ZakoPattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f;
    has_shot = false;
    after_shot_timer = 0.0f;

    // �p�^�[�����Ƃ�HP�E�A�j���E�摜�ݒ�
    switch (pattern)
    {
        case ZakoPattern::MoveStraight:
            hp = 10;
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            break;

        case ZakoPattern::RightMove:
        case ZakoPattern::LeftMove:
            hp = 15;
            images = images_a;
            anim_indices = { 0, 1, 2, 3 };
            break;

        case ZakoPattern::ZIgzag:
        case ZakoPattern::MoveThenDiagonal:
            hp = 20;
            images = images_b;
            anim_indices = { 0,1,2,3,4,5,6,7,8,9,10,11 };
            break;

        case ZakoPattern::MoveAndStopShoot:
            hp = 30;
            images = images_a;
            anim_indices = { 0, 1, 2, 3 };
            break;
    }

    // �����t���[���ɐݒ�
    image = images[0];
}
