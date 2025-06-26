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

#include "../../../../Object/Item/PowerUp/PowerUp.h"
#include "../../../../Object/Item/Shield/Shield.h"

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


    player->SetMobility(false); // ����֎~
    entry_start_pos = Vector2D(D_WIN_MAX_X / 2, 360);             // ���������肩��
    entry_end_pos = Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y - 120); // �ʏ�̒�ʒu
    player->SetLocation(entry_start_pos); // �ŏ��̈ʒu��ݒ�
    player_entry_timer = 0.0f; // �^�C�}�[���Z�b�g
    player->ForceNeutralAnim(true); // �� �A�j�����Œ�


    font_warning = CreateFontToHandle("Orbitron", 48, 6, DX_FONTTYPE_ANTIALIASING);
    font_orbitron = CreateFontToHandle("Orbitron", 22, 6, DX_FONTTYPE_ANTIALIASING);
    font_digital = CreateFontToHandle("���C���I", 28, 6, DX_FONTTYPE_ANTIALIASING);


    stage_id = StageID::Stage1;

   // ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
   // bg_image = rm->GetImages("Resource/Image/BackGround/Main/Stage1/bg_01.png")[0];

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();

    // ======== Zako�i�G���G�j���\�[�X�̃v�����[�h ========
    // �A�j���[�V�����摜�i�p�^�[��A�EB�j
    rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_a.png", 4, 4, 1, 32, 32);
    rm->GetImages("Resource/Image/Object/Enemy/Zako1/anime_enemy30_b.png", 12, 12, 1, 24, 24);

    // �T�E���h�i���S���E�o�ꉹ�j
    rm->GetSounds("Resource/sound/se/se_effect/kill_4.mp3");
    rm->GetSounds("Resource/sound/se/effect/buon.mp3");

    preload_done = false;  // ���������Ƀ��Z�b�g

    // =====================================================

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
    // �t�F�[�h�C�����Ɉ�x��������
    if (is_fading_in && !preload_done)
    {
        preload_done = true;

        // �����G�t�F�N�g�E�V���b�g���o�����O�Ăяo��
        auto anim = Singleton<AnimationManager>::GetInstance();

        // ��ʊO�ɒe�ƓG�����ɐ����i��\���j
        auto objm = Singleton<GameObjectManager>::GetInstance();

        player->SetIsShot();  // �A�j���p�t���O�i�A�j�������Đ��j
    }

    if (entry_effect_playing)
    {
        entry_effect_timer += delta;
        if (entry_effect_timer >= 1.0f) // 1�b�ԕ\��
        {
            entry_effect_playing = false;
        }
    }


    UpdateBackgroundScroll(delta);

    if (is_player_entering)
    {
        const float duration = 2.5f; // �������2.5�b�����č~��
        player_entry_timer += delta;

        float t = player_entry_timer / duration;
        if (t >= 1.0f)
        {
            t = 1.0f;
            is_player_entering = false;
            player->SetMobility(true);
            player->ForceNeutralAnim(false); // �� �A�j���[�V��������
        }


        // �ɂ₩�ɉ��~����C�[�W���O�ieaseOutQuad�j
        float eased = 1.0f - (1.0f - t) * (1.0f - t);
        Vector2D new_pos = entry_start_pos + (entry_end_pos - entry_start_pos) * eased;
        player->SetLocation(new_pos);
    }

    enemy_list.erase(
        std::remove_if(enemy_list.begin(), enemy_list.end(),
            [](EnemyBase* e) { return (e == nullptr || e->is_destroy); }),
        enemy_list.end());


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
    if (player->GetIsShot() == true)
    {
        Vector2D p_location = player->GetLocation();
        player->SetIsShot();
        // �㉺���]���Ă��Ȃ������牺�����ɐ���
        if (player->GetShotFlip() == false)
        {
            if (player->GetPowerd() <= 1)
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));

            }
            else if (player->GetPowerd() == 2)
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 30, p_location.y));
            }
            else
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 50, p_location.y + D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 50, p_location.y + D_OBJECT_SIZE));

            }
            shot->SetShotFlip(false);
        }
        // ���]���Ă����������ɐ���
        else
        {
            Vector2D p_location = player->GetLocation();
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x, p_location.y + D_OBJECT_SIZE));
            shot->SetShotFlip(true);
        }
    }
    // �v���C���[���r�[�������������ł��Ă�����r�[���𐶐�
    else if (player->GetBeamOn() == true)
    {
        Vector2D p_location = player->GetLocation();
        // �㉺���]���Ă��Ȃ������牺�����ɐ���
        if (player->GetShotFlip() == false)
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y - D_OBJECT_SIZE) - 848));
            beam->SetBeamFlip(false);
        }
        // ���]���Ă����������ɐ���
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

    // �~�b�V�������x�����o�i�J�n3�b�ԁj
   // �X�e�[�W�J�n���o�i�o���h�ƕ����j
    if (stage_timer < 5.0f && !warning_label_shown)
    {
        switch (warning_label_state)
        {
        case WarningLabelState::None:
            warning_label_state = WarningLabelState::Expanding;
            warning_label_timer = 0.0f;
            warning_label_band_height = 0.0f;
            slide_out_timer = 0.0f;
            break;

        case WarningLabelState::Expanding:
            warning_label_band_height += warning_label_expand_speed * delta;
            if (warning_label_band_height >= warning_label_max_height)
            {
                warning_label_band_height = warning_label_max_height;
                warning_label_state = WarningLabelState::Displaying;
                warning_label_timer = 0.0f;
            }
            break;

        case WarningLabelState::Displaying:
            warning_label_timer += delta;
            if (warning_label_timer >= warning_label_display_duration)
            {
                warning_label_state = WarningLabelState::SlideOut;
                slide_out_timer = 0.0f;
            }
            break;

        case WarningLabelState::SlideOut:
            slide_out_timer += delta;
            if (slide_out_timer >= 0.5f) // �X���C�h�A�E�g����
            {
                warning_label_state = WarningLabelState::Shrinking;
            }
            break;

        case WarningLabelState::Shrinking:
            warning_label_band_height -= warning_label_expand_speed * delta;
            if (warning_label_band_height <= 0.0f)
            {
                warning_label_band_height = 0.0f;
                warning_label_state = WarningLabelState::None;
                warning_label_shown = true;
            }
            break;
        }
    }





    if (distance_timer >= 0.01f)
    {
        if (distance > 0)
        {
            distance--;
        }
        else
        {
            distance = 0;
            //is_clear = true;
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

    enemy_delay_timer += delta;
    if (enemy_delay_timer >= 5.0f) // 0.5�b�o�ߌ�ɏo��
    {
        EnemyAppearance(delta);
        enemy_spawned = true;
    }


    //// �G���|���ꂽ���͌o���l�𐶐��B
    //for (auto& enemy : enemy_list)
    //{
    //    if (enemy->is_destroy == true && !enemy->is_exp_generated)
    //    {
    //        Exp* exp = Singleton<GameObjectManager>::GetInstance()->CreateObject<Exp>(enemy->GetLocation());
    //        exp->SetPlayer(player);
    //        enemy->is_exp_generated = true;
    //    }
    //}

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



    if ((is_clear || is_over) && !glitch_started)
    {
        StartGlitchEffect(); // �� �t�F�[�h�̑���ɊJ�n
    }

    UpdateGlitchEffect(delta);

    if (glitch_done)
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

    // -------- �X�e�[�W���o�FNeural Grid --------
    if (warning_label_state != WarningLabelState::None && warning_label_band_height > 1.0f)
    {
        int y_top = static_cast<int>(360 - warning_label_band_height);
        int y_bottom = static_cast<int>(360 + warning_label_band_height);

        // �p���X���C�e�B���O�i���Ń��C���j
        float pulse = (sinf(stage_timer * 6.0f) + 1.0f) * 0.5f; // 0.0?1.0
        int pulse_alpha = static_cast<int>(pulse * 180);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, pulse_alpha);
        DrawLine(0, y_top, 1280, y_top, GetColor(120, 220, 255));
        DrawLine(0, y_bottom, 1280, y_bottom, GetColor(120, 220, 255));
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // �O���b�`���̃����_���X�L�������C���i��������o�j
        for (int i = 0; i < 8; ++i)
        {
            int glitch_y = y_top + rand() % (2 * static_cast<int>(warning_label_band_height));
            int glitch_len = 50 + rand() % 100;
            int glitch_x = rand() % (1280 - glitch_len);

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100 + rand() % 100);
            DrawBox(glitch_x, glitch_y, glitch_x + glitch_len, glitch_y + 2, GetColor(200, 255, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
        if (
            warning_label_state == WarningLabelState::Displaying ||
            warning_label_state == WarningLabelState::SlideOut
            )
        {
            int center_x = D_WIN_MAX_X / 2;

            // �X���C�h�C��
            float slide_in_t = warning_label_timer / 0.5f;
            if (slide_in_t > 1.0f) slide_in_t = 1.0f;
            float slide_in = 1.0f - powf(1.0f - slide_in_t, 3.0f);

            // �X���C�h�A�E�g�i�E�����j
            float slide_out_t = slide_out_timer / 0.5f;
            if (slide_out_t > 1.0f) slide_out_t = 1.0f;
            float slide_out = 1.0f + slide_out_t;  // ? ���֗����␳

            float t = (warning_label_state == WarningLabelState::Displaying) ? slide_in : slide_out;

            int stage_name_x = static_cast<int>((1.0f - t) * 1280 + t * (center_x - 150));
            int sub_text_x = static_cast<int>((1.0f - t) * 1280 + t * (center_x - 150));

            DrawStringToHandle(stage_name_x, 320,
                "Neural Grid", GetColor(255, 255, 255), font_warning);

            DrawStringToHandle(sub_text_x, 370,
                "Eliminate all hostile units.", GetColor(120, 255, 255), font_orbitron);
        }

    }



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

       
            int base_x = (D_WIN_MAX_X / 2) - 80;
            int base_y = D_WIN_MAX_Y / 2 - 40;

            int time = GetNowCount();
            float scale = 1.0f + 0.1f * sinf(time / 5.0f);  // �X�P�[�����O

            int shake_x = GetRand(6) - 3;
            int shake_y = GetRand(6) - 3;

            int red = 180 + GetRand(75);  // �_�ł���ԐF
            int green = GetRand(50);
            int blue = GetRand(50);

            // ���C���̗h���WARNING�����i���j
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
            DrawExtendStringToHandle(
                base_x + shake_x, base_y + shake_y,
                scale, scale,
                "WARNING!!",
                GetColor(255, 255, 255),
                font_digital
            );

            // �w��̐Ԃ������ŉ��s��
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
            DrawExtendStringToHandle(
                base_x, base_y,
                scale, scale,
                "WARNING!!",
                GetColor(red, green, blue),
                font_digital
            );

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        

    }

    if (entry_effect_playing)
    {
        float t = entry_effect_timer / 1.0f;
        if (t > 1.0f) t = 1.0f;
        int alpha = static_cast<int>((1.0f - t) * 255);

        for (int i = 0; i < 30; ++i)
        {
            int x = GetRand(D_WIN_MAX_X);
            int y = GetRand(D_WIN_MAX_Y);
            int w = 40 + GetRand(100);
            int h = 5 + GetRand(20);
            int col = GetColor(200 + GetRand(55), 200 + GetRand(55), 255);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            DrawBox(x, y, x + w, y + h, col, TRUE);
        }

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha / 3);
        DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE); // �t���b�V������
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }


    DrawGlitchEffect(); // �� �őO�ʂɃO���b�`�\��

}

bool Stage1::IsFinished()
{
    return finished;
}

void Stage1::SetFinished()
{
    finished = true;
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

StageBase* Stage1::GetNowStage()
{
    return this;
}

//void Stage1::EnemyAppearance(float delta)
//{
//    enemy_spawn_timer += delta;
//
//    // �o�ߎ��Ԃɉ����ăX�|�[���Ԋu�𒲐��i�Œ�0.5�b�j
//    float spawn_interval = 2.0f - (stage_timer / 5.0f);
//    if (spawn_interval < 0.5f)
//    {
//        spawn_interval = 0.5f;
//    }
//
//    if (enemy_spawn_timer >= spawn_interval)
//    {
//        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
//
//        if (stage_timer < 24.0f)
//        {
//            // �����_�����[����Zako1��1�̏o��
//            const int num_lanes = 7;
//            const int lane_x[num_lanes] = { 250, 350, 450, 550, 650, 750, 850 };
//            int lane_index = GetRand(num_lanes);
//            float x = static_cast<float>(lane_x[lane_index]);
//            Vector2D spawn_pos(x, 0.0f);
//            Zako* zako = objm->CreateObject<Zako>(spawn_pos);
//            zako->SetPattern(ZakoPattern::MoveStraight);
//            zako->SetPlayer(player);
//            enemy_list.push_back(zako);
//
//
//        }
//
//        else if (stage_timer < 25.0f)
//        {
//        } //�C���^�[�o��
//
//        else if (stage_timer < 48.0f)
//        {
//            // �����_����Y�ʒu�ō��E����Zako1������ + ��������Zako4���o��
//            const int num_lanes = 3;
//            const int lane_y[num_lanes] = { 20, 100, 180 };
//            int lane_index_y = GetRand(num_lanes);
//            float y = static_cast<float>(lane_y[lane_index_y]);
//
//            bool from_left = fmod(stage_timer, 20.0f) < 10.0f;
//            Vector2D spawn_pos = from_left ? Vector2D(270.0f, y) : Vector2D(1000.0f, y);
//            ZakoPattern pattern = from_left ? ZakoPattern::RightMove : ZakoPattern::LeftMove;
//
//            Zako* zako = objm->CreateObject<Zako>(spawn_pos);
//            zako->SetPattern(pattern);
//            zako->SetPlayer(player);
//            enemy_list.push_back(zako);
//
//            /*      if (stage_timer >= 20.0f && !zako4_spawned)
//                  {
//                      Zako4* zako4 = objm->CreateObject<Zako4>(Vector2D(60, 20));
//                      zako4->SetPlayer(player);
//                      enemy_list.push_back(zako4);
//
//                      zako4_spawned = true;
//                  }*/
//
//        }
//
//        else if (stage_timer < 49.0f)
//        {
//        } //�C���^�[�o��
//
//        else if (stage_timer < 74.0f)
//        {
//            // �����_�����[����Zako1��1�̏o��
//            const int num_lanes = 6;
//            const int lane_x[num_lanes] = { 350, 450, 550, 650, 750, 850 };
//            int lane_index = GetRand(num_lanes);
//            float x = static_cast<float>(lane_x[lane_index]);
//            Vector2D spawn_pos(x, 0.0f);
//            Zako* zako = objm->CreateObject<Zako>(spawn_pos);
//
//            // �����_���Ƀp�^�[����I��
//            int r = GetRand(2); // 0��1�������_���Ŏ擾
//            if (r == 0)
//                zako->SetPattern(ZakoPattern::MoveAndStopShoot);
//            else
//                zako->SetPattern(ZakoPattern::DiveOnce);
//
//            zako->SetPlayer(player);
//            enemy_list.push_back(zako);
//        }
//
//        else if (stage_timer < 77.0f)
//        {
//            for (auto& enemy : enemy_list)
//            {
//                if (enemy == nullptr || enemy->is_destroy) continue;
//
//                // ���S�ȃL���X�g�iRTTI���L���ŉ��z�֐�����������`����Ă���O��j
//                Zako* zako = dynamic_cast<Zako*>(enemy);
//                if (zako == nullptr) continue;
//
//                ZakoPattern current = zako->GetPattern();
//                if (current == ZakoPattern::MoveAndStopShoot || current == ZakoPattern::DiveOnce)
//                {
//                    zako->SetPattern(ZakoPattern::RetreatUp);
//                }
//            }
//        }
//
//
//
//
//        else
//        {
//            if (!boss_spawned)
//            {
//                GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
//
//                // �p���[�A�b�v�A�C�e��
//                objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 - 60, 120));
//
//                // �V�[���h�A�C�e��
//                objm->CreateObject<Shield>(Vector2D(D_WIN_MAX_X / 2 + 60, 120));
//
//                item_spawned = true;
//
//                boss1 = objm->CreateObject<Stage1Boss>(Vector2D(670, -200));
//                boss1->SetPattern(BossPattern::Entrance);
//                boss1->SetPlayer(player);
//                enemy_list.push_back(boss1);
//                boss_spawned = true;
//                is_warning = false;  // �x���I��
//            }
//        }
//
//
//
//        enemy_spawn_timer = 0.0f;
//    }
//}
void Stage1::EnemyAppearance(float delta)
{
    enemy_spawn_timer += delta;

    float spawn_interval = 2.0f - (stage_timer / 5.0f);
    if (spawn_interval < 0.5f) spawn_interval = 0.5f;

    if (enemy_spawn_timer >= spawn_interval)
    {
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

        // �y0?24�b�z�F�c���璼���I�ɏo��
        if (stage_timer < 24.0f)
        {
            const int lane_x[] = { 250, 350, 450, 550, 650, 750, 850 };
            float x = static_cast<float>(lane_x[GetRand(7)]);
            Zako* zako = objm->CreateObject<Zako>(Vector2D(x, -40));
            zako->SetPattern(ZakoPattern::MoveStraight);
            zako->SetPlayer(player);
            enemy_list.push_back(zako);
        }

        // �y25?48�b�z�F���E�����荞�݈ړ�
        else if (stage_timer < 48.0f)
        {
            const int lane_y[] = { 40, 120, 200 };
            float y = static_cast<float>(lane_y[GetRand(3)]);
            bool from_left = fmod(stage_timer, 20.0f) < 10.0f;
            Vector2D pos = from_left ? Vector2D(270.0f, y) : Vector2D(1000.0f, y);
            ZakoPattern pattern = from_left ? ZakoPattern::RightMove : ZakoPattern::LeftMove;

            Zako* zako = objm->CreateObject<Zako>(pos);
            zako->SetPattern(pattern);
            zako->SetPlayer(player);
            enemy_list.push_back(zako);
        }

        // �y49?59�b�z�F�ˌ��n�p�^�[���i�~�܂��Č��� or �ːi�j
        else if (stage_timer < 60.0f)
        {
            const int lane_x[] = { 350, 450, 550, 650, 750, 850 };
            float x = static_cast<float>(lane_x[GetRand(6)]);
            Zako* zako = objm->CreateObject<Zako>(Vector2D(x, -40));
            ZakoPattern pattern = (GetRand(2) == 0)
                ? ZakoPattern::MoveAndStopShoot
                : ZakoPattern::DiveOnce;
            zako->SetPattern(pattern);
            zako->SetPlayer(player);
            enemy_list.push_back(zako);
        }

        else if (stage_timer < 65.0f) // �y60?65�b�z�FZIgzag + MoveStraight ����
        {
            for (int i = 0; i < 5; ++i)
            {
                float x = 300.0f + GetRand(400);
                ZakoPattern pattern = (i % 2 == 0) ? ZakoPattern::ZIgzag : ZakoPattern::MoveStraight;

                Zako* zako = objm->CreateObject<Zako>(Vector2D(x, -60.0f - i * 30));
                zako->SetPattern(pattern);
                zako->SetPlayer(player);
                enemy_list.push_back(zako);
            }
        }

        else if (stage_timer < 70.0f) // �y65?70�b�z�F���E���珇�Ԃɓo��
        {
            const int lane_y[] = { 40, 120, 200 };
            float y = static_cast<float>(lane_y[GetRand(3)]);
            bool from_left = fmod(stage_timer, 20.0f) < 10.0f;
            Vector2D pos = from_left ? Vector2D(270.0f, y) : Vector2D(1000.0f, y);
            ZakoPattern pattern = from_left ? ZakoPattern::RightMove : ZakoPattern::LeftMove;

            Zako* zako = objm->CreateObject<Zako>(pos);
            zako->SetPattern(pattern);
            zako->SetPlayer(player);
            enemy_list.push_back(zako);
        }

        else if (stage_timer < 74.0f) // �y70?74�b�z�F�����������߂�悤�Ɉ�C�ɓːi
        {
            for (int i = 0; i < 3; ++i)
            {
                bool from_left = (GetRand(2) == 0);
                float y = 100.0f + i * 80.0f;

                Vector2D pos = from_left
                    ? Vector2D(-60.0f, y)
                    : Vector2D(D_WIN_MAX_X + 60.0f, y);

                ZakoPattern pattern = from_left ? ZakoPattern::RightMove : ZakoPattern::LeftMove;

                Zako* zako = objm->CreateObject<Zako>(pos);
                zako->SetPattern(pattern);
                zako->SetPlayer(player);
                enemy_list.push_back(zako);
            }
        }







        else if (stage_timer < 77.0f) // �y74?77�b�z�F�O���b�`���o �� �S�U�R���ޔ�
        {
            for (auto& enemy : enemy_list)
            {
                if (!enemy || enemy->is_destroy) continue;

                Zako* zako = dynamic_cast<Zako*>(enemy);
                if (zako == nullptr) continue;

                ZakoPattern current = zako->GetPattern();

                if (current == ZakoPattern::MoveAndStopShoot ||
                    current == ZakoPattern::DiveOnce ||
                    current == ZakoPattern::RotateAndShoot ||
                    current == ZakoPattern::PauseThenRush ||
                    current == ZakoPattern::LeftMove ||               
                    current == ZakoPattern::RightMove ||              
                    current == ZakoPattern::Formation)                
                {
                    zako->SetPattern(ZakoPattern::RetreatUp);
                }
            }
        }


        // �y77�b�ȍ~�z�F�{�X��˓� + �A�C�e�����o
        else
        {
            if (!boss_spawned)
            {
                objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 - 60, 120))->SetPlayer(player);
                objm->CreateObject<Shield>(Vector2D(D_WIN_MAX_X / 2 + 60, 120))->SetPlayer(player);

                boss1 = objm->CreateObject<Stage1Boss>(Vector2D(670, -200));
                boss1->SetPattern(BossPattern::Entrance);
                boss1->SetPlayer(player);
                enemy_list.push_back(boss1);

                boss_spawned = true;
                is_warning = false;
            }
        }

        enemy_spawn_timer = 0.0f;
    }
}


//void Stage1::DrawScrollBackground() const
//{
//    // === �w�i�F�F��▾��߂ɕύX ===
//    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(30, 30, 60), TRUE);
//
//    // ==== �O���f�[�V�����w�i�i�����j + �ȈՂ�炬�m�C�Y ====
//    for (int y = 0; y < D_WIN_MAX_Y; ++y)
//    {
//        int grad = 40 + (y * 80 / D_WIN_MAX_Y); // ���ɍs���قǖ��邭
//        int r = grad / 2 + GetRand(5); // �����_���ł�炬��ǉ�
//        int g = 0;
//        int b = grad + GetRand(5);     // �����ɐ�����
//
//        DrawLine(0, y, D_WIN_MAX_X, y, GetColor(r, g, b));
//    }
//
//    // === �f�[�^������i���߁E���Ȃ߁j ===
//    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 90); // ���ߓx����ɔ���
//
//    static std::vector<float> data_cols;
//    static std::vector<float> scale_factors;
//    static std::vector<float> fall_speeds;
//
//    const int data_interval = 80;               // ��̊Ԋu���L���Đ������炷
//    const int base_line_height = 30;
//    const int string_len = 10;                  // ������������Z��
//
//    if (data_cols.empty()) {
//        for (int x = 0; x < D_WIN_MAX_X; x += data_interval) {
//            data_cols.push_back(static_cast<float>(GetRand(D_WIN_MAX_Y)));
//            scale_factors.push_back(0.9f + GetRand(50) / 100.0f);  // 0.9?1.4�{
//            fall_speeds.push_back(0.04f + GetRand(20) / 300.0f);   // 0.04?0.1���炢
//        }
//    }
//
//    for (size_t i = 0; i < data_cols.size(); ++i)
//    {
//        int x = static_cast<int>(i) * data_interval;
//        float& y = data_cols[i];
//        float scale = scale_factors[i];
//        float speed = fall_speeds[i];
//
//        y += speed;
//        if (y > D_WIN_MAX_Y + string_len * base_line_height * scale)
//            y = -GetRand(200);
//
//        for (int j = 0; j < string_len; ++j)
//        {
//            char str[2] = { '0' + GetRand(1), '\0' };
//
//            int draw_y = static_cast<int>(y) - j * static_cast<int>(base_line_height * scale);
//            if (draw_y < 0 || draw_y > D_WIN_MAX_Y) continue;
//
//            int g = 130 - j * (60 / string_len);  // �����i�O���[���T���߁j
//            int b = 160;
//
//            DrawExtendStringToHandle(
//                x, draw_y,
//                scale, scale,
//                str,
//                GetColor(0, g, b),
//                font_digital
//            );
//        }
//    }
//
//    // ==== ���̃O���b�h�i�X�N���[���j====
//    static float grid_scroll_back = 0.0f;
//    grid_scroll_back += 0.1f; // ������蓮��
//    const int grid_size_back = 80;
//    int offset_back = static_cast<int>(grid_scroll_back) % grid_size_back;
//
//    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60); // ���Ȃ蔖��
//
//    for (int x = -grid_size_back; x < D_WIN_MAX_X + grid_size_back; x += grid_size_back)
//        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(180, 100, 255)); // �W����
//
//    for (int y = -grid_size_back + offset_back; y < D_WIN_MAX_Y; y += grid_size_back)
//        DrawLine(0, y, D_WIN_MAX_X, y, GetColor(180, 100, 255));
//
//    // ==== ��O�̃O���b�h�i�����X�N���[���j====
//    static float grid_scroll_front = 0.0f;
//    grid_scroll_front += 0.25f;
//    const int grid_size_front = 40;
//    int offset_front = static_cast<int>(grid_scroll_front) % grid_size_front;
//
//    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100); // ���Z����
//
//    for (int x = -grid_size_front; x < D_WIN_MAX_X + grid_size_front; x += grid_size_front)
//        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(255, 150, 255)); // �s���N���
//
//    for (int y = -grid_size_front + offset_front; y < D_WIN_MAX_Y; y += grid_size_front)
//        DrawLine(0, y, D_WIN_MAX_X, y, GetColor(255, 150, 255));
//
//    // === �n�C���C�g���C�� ===
//    if (GetNowCount() % 120 < 60)
//    {
//        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
//        int flash_y = 40 + (GetNowCount() / 2) % (D_WIN_MAX_Y - 80);
//        DrawLine(0, flash_y, D_WIN_MAX_X, flash_y, GetColor(100, 255, 255));
//    }
//
//    // === ���q���� ===
//    while (star_particles.size() < 100)
//    {
//        StarParticle p;
//        p.pos = Vector2D(GetRand(D_WIN_MAX_X), GetRand(D_WIN_MAX_Y));
//        p.velocity = Vector2D(0, 40.0f + GetRand(60)); // ��⑬��
//        p.alpha = 150.0f + GetRand(100); // ������
//        p.length = 10.0f + GetRand(10);
//        p.life = 2.0f + (GetRand(100) / 50.0f);
//        star_particles.push_back(p);
//    }
//
//    // === ���q�`�� ===
//    for (auto& p : star_particles)
//    {
//        p.pos.y += p.velocity.y * 0.016f;
//        p.alpha -= 0.5f;
//
//        if (p.length > 0.0f)
//        {
//            int a = static_cast<int>(p.alpha);
//            if (a < 0) a = 0;
//            if (a > 255) a = 255;
//
//            SetDrawBlendMode(DX_BLENDMODE_ALPHA, a);
//
//            // ��薾�邢�F
//            DrawLine(static_cast<int>(p.pos.x),
//                static_cast<int>(p.pos.y),
//                static_cast<int>(p.pos.x),
//                static_cast<int>(p.pos.y + p.length),
//                GetColor(200, 255, 255));
//        }
//    }
//
//    star_particles.erase(
//        std::remove_if(star_particles.begin(), star_particles.end(), [](const StarParticle& p)
//            {
//                return (p.pos.y > D_WIN_MAX_Y || p.alpha <= 0);
//            }),
//        star_particles.end()
//    );
//    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); 
//
//}


void Stage1::DrawScrollBackground() const
{
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(20, 20, 40), TRUE);

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

    const int grid_size_back = 40;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, 100, 255));
    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back)
    {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(0, 100, 255));
    }

    const int grid_size_front = 80;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(180, 0, 255), TRUE);
    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front)
    {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(180, 0, 255), TRUE);
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}



