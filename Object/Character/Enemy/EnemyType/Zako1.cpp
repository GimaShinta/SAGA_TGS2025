#include "Zako1.h"
#include "../../Player/Player.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>

Zako1::Zako1()
{
    srand(static_cast<unsigned int>(time(nullptr))); // ����������
}

Zako1::~Zako1()
{}

void Zako1::Initialize()
{
    enemy_type = ENE_ZAKO1;
    z_layer = 2;
    box_size = 12;
    hp = 10;

    // �Փ˔���ݒ�
    collision.is_blocking = true;
    collision.object_type = eObjectType::eEnemy;
    collision.hit_object_type.push_back(eObjectType::eShot);
    collision.hit_object_type.push_back(eObjectType::eBeam);

    is_mobility = true;
    start_location = location;
    is_returning = false;
   

    ChangePatternRandomly(); // �����p�^�[���������_���Ō���
}

void Zako1::Update(float delta_second)
{
    spawn_delay_timer -= delta_second;
    pattern_timer += delta_second;

   

    // �p�^�[���ɉ����Ĉړ�������ݒ�
    switch (pattern)
    {
        case Zako1Pattern::MoveStraight: velocity = { 0, 120 };  break;
        case Zako1Pattern::RightMove:    velocity = { 120, 0 };  break;
        case Zako1Pattern::LeftMove:     velocity = { -120, 0 }; break;
        case Zako1Pattern::ZIgzag:       velocity.x = sinf(pattern_timer * 1.5f) * 160;
                                         velocity.y = 100;       break;

    }

    location += velocity * delta_second; // �ړ�����

    if (hp <= 0)
    {
        is_destroy = true; // ���j�t���O
        Singleton<ScoreData>::GetInstance()->SetScoreData(100); // �X�R�A���Z
    }

    __super::Update(delta_second);
}

void Zako1::Draw(const Vector2D& screen_offset) const
{
    // �G�̓����蔻���HP�\��
    DrawBox(location.x - box_size.x, location.y - box_size.y,
            location.x + box_size.x, location.y + box_size.y, GetColor(0, 0, 255), TRUE);

    DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);
}

void Zako1::Finalize()
{
    // �㏈�����K�v�ȏꍇ�ɋL�q
}

void Zako1::Shot(float delta_second)
{
    shot_timer += delta_second;

    if (shot_timer >= 2.0f && player)
    {
        // �v���C���[�����ɔ���
        Vector2D dir = player->GetLocation() - location;
        float len = dir.Length();
        if (len > 0) dir /= len;

        auto shot = Singleton<GameObjectManager>::GetInstance()->CreateObject<EnemyShot2>(location);
        shot->SetVelocity(dir); // �����ݒ�

        shot_timer = 0.0f;
    }
}

void Zako1::ChangePatternRandomly()
{
    int r = rand() % 4; 

    switch (r)
    {
        case 0: pattern = Zako1Pattern::MoveStraight; break;
        case 1: pattern = Zako1Pattern::RightMove;    break;
        case 2: pattern = Zako1Pattern::LeftMove;     break;
        case 3: pattern = Zako1Pattern::ZIgzag;       break;
    }

    pattern_timer = 0.0f;
}

void Zako1::SetPattern(Zako1Pattern new_pattern)
{
    pattern = new_pattern;
    pattern_timer = 0.0f;
}
