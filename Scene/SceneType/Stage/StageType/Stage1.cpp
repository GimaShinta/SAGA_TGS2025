#include "Stage1.h"
#include "Stage2.h" // ���̃X�e�[�W������ꍇ

#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot3.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako3.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage1.h"
#include <math.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()

Stage1::Stage1(Player* player)
    : StageBase(player),
    zako2(nullptr),
    boss(nullptr),
    e_shot1(nullptr),
    e_shot2(nullptr),
    enemy_spawn_timer(0.0f)
{}

Stage1::~Stage1()
{}

void Stage1::Initialize()
{
    // ����������
    distance = STAGE_DISTANCE;
}

void Stage1::Finalize()
{
    // �I������
}

void Stage1::Update(float delta)
{
    // �G����ʊO�ɏo���ꍇ�ɍ폜
    for (auto it = enemy_list.begin(); it != enemy_list.end(); )
    {
        Vector2D enemy_pos = (*it)->GetLocation();

        if (enemy_pos.x < 100 || enemy_pos.x > 1100)
        {
            (*it)->SetDestroy();
            it = enemy_list.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // �^�C�}�[���J�E���g
    timer++;

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    // �v���C���[���e��ł������ł��Ă�����e�𐶐�
    if (player->GetIsShot())
    {
        Vector2D p_location = player->GetLocation();
        player->SetIsShot();

        if (player->GetShotFlip() == false)
        {
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
            shot->SetShotFlip(false);
        }
        else
        {
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x, p_location.y + D_OBJECT_SIZE));
            shot->SetShotFlip(true);
        }
    }
    // �v���C���[���r�[�������������ł��Ă�����r�[���𐶐�
    else if (player->GetBeamOn())
    {
        Vector2D p_location = player->GetLocation();

        if (player->GetShotFlip() == false)
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y - D_OBJECT_SIZE) - 848));
            beam->SetBeamFlip(false);
        }
        else
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y + D_OBJECT_SIZE) + 848));
            beam->SetBeamFlip(true);
        }

        beam->SetPlayer(player);
        player->SetBeamOn();
    }

    // delta_second �����Z
    stage_timer += delta;

    if (stage_timer >= 0.01f)
    {
        if (distance > 0)
        {
            distance--;
        }
        else
        {
            distance = 0;
        }
        stage_timer = 0;
    }

    // �G�̏o��
    EnemyAppearance();

    // �G�̍U��
    EnemyShot(delta);

    // �Q�[���N���A�E�I�[�o�[����
    if (boss && !boss->GetIsAlive() && !is_over)
    {
        boss->SetDestroy();
        is_clear = true;
    }

    if (boss2 && !boss2->GetIsAlive() && !is_over)
    {
        boss2->SetDestroy();
        is_clear = true;
    }

    if (player && !player->GetIsAlive() && !is_clear)
    {
        player->SetDestroy();
        is_over = true;
    }


    //stage�J��
    stage_timer += delta;

    if (stage_timer >= 10.0f)
    {
        finished = true;
    }


    // ���̏����F�X�y�[�X�L�[����������X�e�[�W�I��
    if (CheckHitKey(KEY_INPUT_N))
    {
        finished = true;
    }
}

void Stage1::Draw()
{
    // �w�i�𔒂œh��i�ŏ��ɕ`��j
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Draw();

    // ���̍���
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // �E�̍���
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "�Q�[�����C��", GetColor(255, 255, 255));
    DrawString(0, 300, "������@\n\n���X�e�B�b�N\n�\���{�^��\nWASD�L�[ : �ړ�\n\nA�{�^��\n�X�y�[�X�L�[ : ����\n\nB�{�^��\nB�L�[ : ���[�U�[\n\nRB�{�^��\nL�L�[ : �ˏo���]", GetColor(255, 255, 255));
    DrawFormatString(0, 20, GetColor(255, 255, 0), "�G��: %d", enemy_list.size());

    // �X�e�[�W�`��
    if (stage <= 1)
    {
        if (is_warning)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, brend);
            DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(255, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        }
    }
    else if (stage >= 2)
    {
        DrawBox((D_WIN_MAX_X / 2) - 250, 0, (D_WIN_MAX_X / 2) + 250, D_WIN_MAX_Y, GetColor(100, 100, 100), TRUE);
    }

    if (is_clear)
    {
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "�Q�[���N���A", GetColor(0, 0, 0));
    }
    else if (is_over)
    {
        DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "�Q�[���I�[�o�[", GetColor(0, 0, 0));
    }
}

bool Stage1::IsFinished()
{
    return finished;
}

bool Stage1::IsClear()
{
    return is_clear;
}

bool Stage1::IsOver()
{
    return is_over;
}

StageBase* Stage1::GetNextStage(Player* player)
{
    return new Stage2(player); // ���̃X�e�[�W��
}

void Stage1::EnemyAppearance()
{
    enemy_spawn_timer += 1.0f / 60.0f; // 1�t���[�����Ƃɉ��Z�i60FPS�z��j

    if (enemy_spawn_timer >= 30.0f)
    {
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

        const int NUM_LANES = 3;
        float lane_y[NUM_LANES] = {
            D_WIN_MAX_Y / 8.0f,         // �ヌ�[��
            D_WIN_MAX_Y / 4.0f,         // �������[��
            (D_WIN_MAX_Y / 8.0f) * 3.0f // �����[���i�㔼���܂Łj
        };

        bool lane_occupied[NUM_LANES] = { false, false, false };

        // ���݂̓G�̈ʒu���烌�[����L��Ԃ��`�F�b�N
        for (const auto& enemy : enemy_list)
        {
            float e_y = enemy->GetLocation().y;
            for (int i = 0; i < NUM_LANES; i++)
            {
                if (fabs(e_y - lane_y[i]) < 30.0f) // 30px�ȓ��ɂ���Ȃ瓯�ꃌ�[��
                {
                    lane_occupied[i] = true;
                }
            }
        }

        // �g�p�\�ȃ��[�������W
        std::vector<int> available_lanes;
        for (int i = 0; i < NUM_LANES; i++)
        {
            if (!lane_occupied[i])
                available_lanes.push_back(i);
        }

        // �o���\�ȃ��[�������邩�m�F
        if (!available_lanes.empty())
        {
            int chosen_index = rand() % available_lanes.size(); // �g�p�\�ȃ��[���̒����烉���_��
            int lane_id = available_lanes[chosen_index];
            float spawn_y = lane_y[lane_id];

            int random_direction = rand() % 2; // �����炩�E���炩
            float base_x = (random_direction == 0)
                ? (rand() % ((D_WIN_MAX_X / 2) - 400))         // ��
                : ((D_WIN_MAX_X / 2) + 400 + rand() % (D_WIN_MAX_X - ((D_WIN_MAX_X / 2) + 400))); // �E

            Vector2D velocity(random_direction == 0 ? 60 : -60, 0);

            for (int i = 0; i < 4; i++)
            {
                float offset = i * 50.0f; // ����50px�����炵�Ĕz�u
                float spawn_x = (random_direction == 0)
                    ? base_x + offset
                    : base_x - offset;

                auto enemy = objm->CreateObject<Zako1>(Vector2D(spawn_x, spawn_y));
                enemy->SetVelocity(velocity);
                enemy_list.push_back(enemy);
            }
        }

        enemy_spawn_timer = 0.0f;
    }
}



void Stage1::EnemyShot(float delta_second)
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // �G���e��ł������ł��Ă�����e�𔭎˂���
    for (int i = 0; i < enemy_list.size(); i++)
    {
        if (enemy_list[i]->GetIsShot())
        {
            int enemy_type = enemy_list[i]->GetEnemyType();

            // �G���G���P��������ʏ�e�𔭎˂���
            if (enemy_type == ENE_ZAKO1)
            {
                Vector2D e_location = enemy_list[i]->GetLocation();
                objm->CreateObject<EnemyShot1>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                objm->CreateObject<EnemyShot1>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                enemy_list[i]->SetIsShot();
            }
            // �G���G���Q��������v���C���[��_�����e�𔭎˂���
            else if (enemy_type == ENE_ZAKO2)
            {
                // �e�L�̈ʒu����v���C���[�ւ̃x�N�g�������߂�
                Vector2D b = player->GetLocation() - enemy_list[i]->GetLocation();
                float c = sqrt(pow(b.x, 2) + pow(b.y, 2));

                // �v���C���[�Ɍ������Ēe��ł�
                e_shot2 = objm->CreateObject<EnemyShot2>(enemy_list[i]->GetLocation());
                e_shot2->SetVelocity(Vector2D(b.x / c, b.y / c));
                enemy_list[i]->SetIsShot();
            }
            // �{�X�̍U���p�^�[��
            else if (enemy_type == ENE_BOSS)
            {
                Vector2D e_location = enemy_list[i]->GetLocation();
                objm->CreateObject<EnemyShot3>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                objm->CreateObject<EnemyShot3>(Vector2D(e_location.x, e_location.y + D_OBJECT_SIZE));
                enemy_list[i]->SetIsShot();
            }
        }
    }
}
