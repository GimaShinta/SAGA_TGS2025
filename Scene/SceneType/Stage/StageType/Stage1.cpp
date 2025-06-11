#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"  

#include <algorithm>


#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot3.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako3.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Stage1BOSS.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage1.h"

#include"../../../../Object/Item/Exp/Exp.h"
#include"../../../../Utility/AnimationManager.h"
#include <math.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()

Stage1::Stage1(Player* player)
    : StageBase(player),
    zako2(nullptr),
    zako4(nullptr),
    boss(nullptr),
    boss1(nullptr),
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

    // �����������i1�񂾂��s���j
    srand(static_cast<unsigned int>(time(NULL)));

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    bg_image = rm->GetImages("Resource/Image/BackGround/Main/Stage1/bg_01.png")[0];
}

void Stage1::Finalize()
{
    // �I������
        // �G���X�g�����ׂč폜
        for (auto& enemy : enemy_list)
        {
            enemy->SetDestroy();
        }
        enemy_list.clear();

        //BGM���~�߂�
        //StopSoundMem(bgm);
}

void Stage1::Update(float delta)
{   
    UpdateBackgroundScroll(delta);

    // �G����ʊO�ɏo���ꍇ�ɍ폜
    for (auto it = enemy_list.begin(); it != enemy_list.end(); )
    {
        Vector2D enemy_pos = (*it)->GetLocation();

        if (enemy_pos.x < -100 || enemy_pos.x > 1100 || enemy_pos.y > 720)
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
    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->Update(delta);
   


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
   // �^�C�}�[2�ɕ�����
    stage_timer += delta;          // 10�b����p
    distance_timer += delta;       // �X�N���[���p

    //�X�N���[��
    scroll_y += 400.0f * delta;

    if (distance_timer >= 0.01f)
    {
        if (distance > 0)
        {
            distance--;
        }
        else
        {
            distance = 0;
        }
        distance_timer = 0;
    }

    /*�J�ڎ���*/
    if (stage_timer >= 130.0f)
    {
        is_clear = true;
    }

    if (player->life == -1)
    {
        is_over = true; 
    }

    // �G�̏o��
    EnemyAppearance(delta);

  

    // �G���|���ꂽ���͌o���l�𐶐��B
    for (auto& enemy : enemy_list)
    {
        if (enemy->is_destroy == true && !enemy->is_exp_generated)
        {
            Exp* exp = Singleton<GameObjectManager>::GetInstance()->CreateObject<Exp>(enemy->GetLocation());
            exp->SetPlayer(player);
            enemy->is_exp_generated = true;
        }
    }

    // �{�X���|�ꂽ��N���A
    if (boss1 != nullptr && boss1->GetIsAlive() == false && is_over == false)
    {
        boss1->SetDestroy();
        is_clear = true;
    }

    if(boss1 != nullptr )

    if (player && !player->GetIsAlive() && !is_clear)
    {
        player->SetDestroy();
        is_over = true;
    }


    if (is_clear == true || is_over == true)
    {
        for (auto& enemy : enemy_list) {
            enemy->SetDestroy();
        }
        enemy_list.clear();

        // �t�F�[�h�A�E�g�����������ҋ@������I��
        scene_timer += delta;
        if (fade_alpha >= 255.0f && scene_timer >= 1.5f)
        {
            finished = true;
        }
    }

    // ���̏����FN��������X�e�[�W�I��
    if (CheckHitKey(KEY_INPUT_N))
    {
        finished = true;
    }


    UpdateFade(delta);
}

void Stage1::Draw()
{
    // �w�i�𔒂œh��i�ŏ��ɕ`��j
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    DrawScrollBackground();  // ������ scroll_offset ���g���ĕ`��I

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Draw();

    // ���̍���
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // �E�̍���
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "STAGE1", GetColor(255, 255, 255));
    DrawFormatString(0, 50, GetColor(255, 255, 0), "Enemy: %d", enemy_list.size());
    // �E����������p�l��
    int panel_x = D_WIN_MAX_X - 270;
    int panel_y = 60;
    int panel_w = 250;
    int panel_h = 180;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);



    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->Draw();

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
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "NEXT STAGE...", GetColor(255, 255, 255));
    }
    else if (is_over)
    {
        DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "�Q�[���I�[�o�[", GetColor(255, 255, 255));
    }

    // �X�e�[�W�^�C�}�[�̕\���i�E��j
    DrawFormatString(0, 20, GetColor(255, 255, 255), "Time: %.1f", stage_timer);

    DrawFadeOverlay();
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

void Stage1::EnemyAppearance(float delta)
{
    enemy_spawn_timer += delta;

    // �o�ߎ��Ԃɉ����ăX�|�[���Ԋu�𒲐��i�Œ�0.5�b�j
    float spawn_interval = 2.0f - (stage_timer / 5.0f);
    if (spawn_interval < 0.5f)
    {
        spawn_interval = 0.5f;
    }

    if (enemy_spawn_timer >= spawn_interval)
    {
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

        if (stage_timer < 24.0f)
        {
            // �����_�����[����Zako1��1�̏o��
            const int num_lanes = 7;
            const int lane_x[num_lanes] = { 250, 350, 450, 550, 650, 750, 850 };
            int lane_index = GetRand(num_lanes);
            float x = static_cast<float>(lane_x[lane_index]);
            Vector2D spawn_pos(x, 0.0f);
            Zako* zako = objm->CreateObject<Zako>(spawn_pos);
            zako->SetPattern(ZakoPattern::MoveStraight);
            zako->SetPlayer(player);
            enemy_list.push_back(zako);


        }

        else if(stage_timer < 25.0f)
        {} //�C���^�[�o��

        else if (stage_timer < 48.0f)
        {
            // �����_����Y�ʒu�ō��E����Zako1������ + ��������Zako4���o��
            const int num_lanes = 3;
            const int lane_y[num_lanes] = { 100, 220, 340 };
            int lane_index_y = GetRand(num_lanes);
            float y = static_cast<float>(lane_y[lane_index_y]);

            bool from_left = fmod(stage_timer, 20.0f) < 10.0f;
            Vector2D spawn_pos = from_left ? Vector2D(200.0f, y) : Vector2D(1000.0f, y);
            ZakoPattern pattern = from_left ? ZakoPattern::RightMove : ZakoPattern::LeftMove;

            Zako* zako = objm->CreateObject<Zako>(spawn_pos);
            zako->SetPattern(pattern);
            zako->SetPlayer(player);
            enemy_list.push_back(zako);

            if (stage_timer >= 20.0f && !zako4_spawned)
            {
                Zako4* zako4 = objm->CreateObject<Zako4>(Vector2D(60, 20));
                zako4->SetPlayer(player);
                enemy_list.push_back(zako4);

                zako4_spawned = true;
            }

        }

        else if(stage_timer < 49.0f)
        {} //�C���^�[�o��

        else if (stage_timer < 74.0f)
        {
            // �����_�����[����Zako1��1�̏o��
            const int num_lanes = 6;
            const int lane_x[num_lanes] = {350, 450, 550, 650, 750, 850 };
            int lane_index = GetRand(num_lanes);
            float x = static_cast<float>(lane_x[lane_index]);
            Vector2D spawn_pos(x, 0.0f);
            Zako* zako = objm->CreateObject<Zako>(spawn_pos);

            // �����_���Ƀp�^�[����I��
            int r = GetRand(2); // 0��1�������_���Ŏ擾
            if (r == 0)
                zako->SetPattern(ZakoPattern::MoveAndStopShoot);
            else
                zako->SetPattern(ZakoPattern::DiveOnce);

            zako->SetPlayer(player);
            enemy_list.push_back(zako);
        }

        else if (stage_timer < 77.0)
        {
        }

        else 
        {
            if (!boss_spawned)
            {
                boss1 = objm->CreateObject<Stage1Boss>(Vector2D(670, -200));
                boss1->SetPattern(BossPattern::Entrance);
                enemy_list.push_back(boss1); 
                boss_spawned = true;
                boss1->SetPlayer(player);

            }
        }


        enemy_spawn_timer = 0.0f;
    }
}



void Stage1::DrawScrollBackground() const
{
    // �w�i�摜�̃X�N���[��
    int bg_height;
    GetGraphSize(bg_image, nullptr, &bg_height);

    float offset_f = fmod(scroll_y, static_cast<float>(bg_height));
    int offset = static_cast<int>(offset_f);

    DrawGraph(0, offset - bg_height, bg_image, TRUE);
    DrawGraph(0, offset, bg_image, TRUE);

    // === �w�ʃO���b�h�i�[�߂̗΁E�A�j�����ʁj===
    const int grid_back = 160;  // �w�i���̃O���b�h�i�c���j
    const int grid_front = 80; // �O�i�O���b�h�i�����j

// === �w�ʃO���b�h�i�[�߂̗΁E�A�j�����ʁj===
SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
for (int x = 0; x < D_WIN_MAX_X; x += grid_back)
{
    int color = GetColor(0, 160 + (x % 3) * 20, 100);
    DrawLine(x, 0, x, D_WIN_MAX_Y, color);
}
for (int y = -grid_back; y < D_WIN_MAX_Y + grid_back; y += grid_back)
{
    int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_back;
    int color = GetColor(0, 200, 100);
    DrawLine(0, sy, D_WIN_MAX_X, sy, color);
}

// === �����_���ɗ����n�C���C�g���C���i���j===
if (GetNowCount() % 120 < 60)
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    int flash_y = 40 + (GetNowCount() / 2) % (D_WIN_MAX_Y - 80);
    DrawLine(0, flash_y, D_WIN_MAX_X, flash_y, GetColor(0, 255, 255));
}

// === �O�ʃO���b�h�i�l�I���{���F���j===
SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
for (int x = 0; x < D_WIN_MAX_X; x += grid_front)
{
    int intensity = 200 + (x % 3) * 20;
    DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(0, intensity, 255), TRUE);
}

for (int y = -grid_front; y < D_WIN_MAX_Y + grid_front; y += grid_front)
{
    int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_front;
    int blue_shift = 150 + ((sy / 20) % 3) * 30;
    DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(0, blue_shift, 180), TRUE);
}



const int total_lines = D_WIN_MAX_Y / grid_front;

SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

// �����ɃO���f�[�V�����������{�����x�ω�
for (int i = 0; i < total_lines + 2; ++i)
{
    int y = i * grid_front;
    int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_front;

    // ���ߊ������o�F��قǔ����E�ׂ��A���قǔZ���E����
    float depth_ratio = static_cast<float>(i) / static_cast<float>(total_lines);
    int alpha = static_cast<int>(depth_ratio * 255);                  // �����x�F��������
    int thickness = static_cast<int>(1 + depth_ratio * 3);            // ���̑����F�����ׂ�
    int r = static_cast<int>(50 + depth_ratio * 150);                 // R: ������
    int g = static_cast<int>(150 - depth_ratio * 100);                // G: ���炷
    int b = static_cast<int>(200 + depth_ratio * 55);                 // B: ���߂�

    int color = GetColor(r, g, b);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    for (int t = -thickness / 2; t <= thickness / 2; ++t)
    {
        DrawLine(0, sy + t, D_WIN_MAX_X, sy + t, color);
    }
}


    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}





