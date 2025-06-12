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

    font_digital = CreateFontToHandle("���C���I", 28, 6, DX_FONTTYPE_ANTIALIASING);

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

        if (is_over == true)
        {
           player->SetDestroy();
        }
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


    // 74?77�b�ŃO���b�`�����o���s��
    if (stage_timer >= 74.0f && stage_timer < 77.0f)
    {
        if (!is_warning)
        {
            is_warning = true;
            warning_timer = 0.0f;
        }
        warning_timer += delta;
    }
    else
    {
        is_warning = false;
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

    if (is_warning)
    {
        // �O���b�`��`�i�m�C�Y���j
        for (int i = 0; i < 10; ++i)
        {
            int x = GetRand(D_WIN_MAX_X);
            int y = GetRand(D_WIN_MAX_Y);
            int w = GetRand(200) + 20;
            int h = GetRand(30) + 5;

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, GetRand(150) + 50);
            DrawBox(x, y, x + w, y + h, GetColor(GetRand(255), GetRand(255), GetRand(255)), TRUE);
        }

        // WARNING�������Ԃ�Ȃ���`��
        int base_x = (D_WIN_MAX_X / 2) - 80;
        int base_y = D_WIN_MAX_Y / 2 - 40;
        DrawString(base_x, base_y, "WARNING!!", GetColor(255, 0, 0));
        DrawString(base_x + GetRand(4) - 2, base_y + GetRand(4) - 2, "WARNING!!", GetColor(255, 255, 255));
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
            const int lane_y[num_lanes] = { 20, 100, 180 };
            int lane_index_y = GetRand(num_lanes);
            float y = static_cast<float>(lane_y[lane_index_y]);

            bool from_left = fmod(stage_timer, 20.0f) < 10.0f;
            Vector2D spawn_pos = from_left ? Vector2D(270.0f, y) : Vector2D(1000.0f, y);
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

        else if (stage_timer < 77.0f)
        {
            for (auto& enemy : enemy_list)
            {
                Zako* zako = dynamic_cast<Zako*>(enemy);
                if (zako != nullptr)
                {
                    // �Ώۃp�^�[�������t�s������
                    ZakoPattern current = zako->GetPattern(); // �����̊֐���Zako�ɒǉ��i���ɐ�������j

                    if (current == ZakoPattern::MoveAndStopShoot || current == ZakoPattern::DiveOnce)
                    {
                        zako->SetPattern(ZakoPattern::RetreatUp);
                    }
                }
            }
        }


        else
        {
            if (!boss_spawned)
            {
                boss1 = objm->CreateObject<Stage1Boss>(Vector2D(670, -200));
                boss1->SetPattern(BossPattern::Entrance);
                boss1->SetPlayer(player);
                enemy_list.push_back(boss1);
                boss_spawned = true;
                is_warning = false;  // �x���I��
            }
        }



        enemy_spawn_timer = 0.0f;
    }
}

void Stage1::DrawScrollBackground() const
{
    // === �w�i�F�F��▾��߂ɕύX ===
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(30, 30, 60), TRUE);

    // === �n�C���C�g���C�� ===
    if (GetNowCount() % 120 < 60)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
        int flash_y = 40 + (GetNowCount() / 2) % (D_WIN_MAX_Y - 80);
        DrawLine(0, flash_y, D_WIN_MAX_X, flash_y, GetColor(100, 255, 255));
    }

    // === ���q���� ===
    while (star_particles.size() < 100)
    {
        StarParticle p;
        p.pos = Vector2D(GetRand(D_WIN_MAX_X), GetRand(D_WIN_MAX_Y));
        p.velocity = Vector2D(0, 40.0f + GetRand(60)); // ��⑬��
        p.alpha = 150.0f + GetRand(100); // ������
        p.length = 10.0f + GetRand(10);
        p.life = 2.0f + (GetRand(100) / 50.0f);
        star_particles.push_back(p);
    }

    // === ���q�`�� ===
    for (auto& p : star_particles)
    {
        p.pos.y += p.velocity.y * 0.016f;
        p.alpha -= 0.5f;

        if (p.length > 0.0f)
        {
            int a = static_cast<int>(p.alpha);
            if (a < 0) a = 0;
            if (a > 255) a = 255;

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, a);

            // ��薾�邢�F
            DrawLine(static_cast<int>(p.pos.x),
                static_cast<int>(p.pos.y),
                static_cast<int>(p.pos.x),
                static_cast<int>(p.pos.y + p.length),
                GetColor(200, 255, 255));
        }
    }

    star_particles.erase(
        std::remove_if(star_particles.begin(), star_particles.end(), [](const StarParticle& p)
            {
                return (p.pos.y > D_WIN_MAX_Y || p.alpha <= 0);
            }),
        star_particles.end()
    );

    // === �f�[�^������ ===
    //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 130);

    //static std::vector<float> data_cols;
    //static std::vector<float> scale_factors;
    //static std::vector<float> fall_speeds;

    //const int data_interval = 60;
    //const int base_line_height = 30;
    //const int string_len = 12;

    //if (data_cols.empty()) {
    //    for (int x = 0; x < D_WIN_MAX_X; x += data_interval) {
    //        data_cols.push_back(static_cast<float>(GetRand(D_WIN_MAX_Y)));
    //        scale_factors.push_back(0.9f + GetRand(100) / 100.0f);    // 0.9?1.9�{
    //        fall_speeds.push_back(0.06f + GetRand(30) / 300.0f);       // 0.6?0.9���炢
    //    }
    //}

    //for (size_t i = 0; i < data_cols.size(); ++i)
    //{
    //    int x = static_cast<int>(i) * data_interval;
    //    float& y = data_cols[i];
    //    float scale = scale_factors[i];
    //    float speed = fall_speeds[i];

    //    y += speed;
    //    if (y > D_WIN_MAX_Y + string_len * base_line_height * scale)
    //        y = -GetRand(200);

    //    for (int j = 0; j < string_len; ++j)
    //    {
    //        char str[2] = { '0' + GetRand(1), '\0' };

    //        int draw_y = static_cast<int>(y) - j * static_cast<int>(base_line_height * scale);
    //        if (draw_y < 0 || draw_y > D_WIN_MAX_Y) continue;

    //        // ���邭�R���g���X�g����F
    //        int g = 180 - j * (100 / string_len);
    //        int b = 200;

    //        DrawExtendStringToHandle(
    //            x, draw_y,
    //            scale, scale,
    //            str,
    //            GetColor(0, g, b),
    //            font_digital
    //        );
    //    }
    //}

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}






