#include "Stage3.h"
#include "Stage2.h" 
#include "../../../../Scene/SceneType/TitleScene.h"

#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot3.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot4.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot5.h"
#include "../../../../Object/Character/Shot/EnemyBeam1.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako3.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Object/Item/PowerUp/PowerUp.h"
#include "../../../../Object/Item/Shield/Shield.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage3.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage4.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../../../Utility/ResourceManager.h"
#include "../../../../Utility/InputManager.h"
#include <math.h>
#include <memory>

Stage3::Stage3(Player* player) : StageBase(player), zako2(nullptr), boss(nullptr), e_shot1(nullptr), e_shot2(nullptr)
{
    bg_scroll_speed_layer1 = 300.0f; // ��
    bg_scroll_speed_layer2 = 600.0f; // ��O
}
Stage3::~Stage3() {}

void Stage3::Initialize()
{
    // ����������
    // �y�L�q�ʒu�͉��z�X�e�[�W�̒�������
    distance = STAGE3_DISTANCE;

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) - 100.0f));
    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 + 100, (D_WIN_MAX_Y / 2) - 100.0f));
    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 + 150, (D_WIN_MAX_Y / 2) - 100.0f));
    objm->CreateObject<Shield>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) + 100.0f));
    objm->CreateObject<Shield>(Vector2D(D_WIN_MAX_X / 2 - 200.0f, (D_WIN_MAX_Y / 2) + 100.0f));
    objm->CreateObject<Shield>(Vector2D(D_WIN_MAX_X / 2 - 200.0f, (D_WIN_MAX_Y / 2)));


#if 0
    //-----------�A�j���[�V���������p�^�[���P-----------
    // �摜��ǂݍ���ł��琶��
    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    image_handles = rm->GetImages("Resource/Image/Effect/E_Explosion.png", 54, 9, 6, 517, 517);

    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->PlayerAnimation(image_handles, Vector2D(player->GetLocation().x, player->GetLocation().y), 0.05f, false);
    //---------------------------------------------------
#else
    //-----------�A�j���[�V���������p�^�[���Q-----------
    // �����������G�t�F�N�g���w�肷��i�G�t�F�N�g�̂݁j
    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    anim_id = manager->PlayerAnimation(EffectName::eExprotion, Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2), 0.05f, false);
    //--------------------------------------------------
#endif

    // �A�j���[�V�����̒ǉ��ݒ�
    manager->SetAlpha(anim_id, 255);       // ������
    manager->SetScale(anim_id, 5.0f);      // 1.5�{�g��
    manager->SetZLayer(anim_id, 1);        // �`�揇�w��

    //�t�H���g
    // font_digital = CreateFontToHandle("DS-Digital", 28, 6, DX_FONTTYPE_ANTIALIASING);
    font_digital = CreateFontToHandle("Orbitron", 28, 6, DX_FONTTYPE_ANTIALIASING);
    font_orbitron = CreateFontToHandle("Orbitron", 22, 6, DX_FONTTYPE_ANTIALIASING);

}

void Stage3::Finalize()
{
    // �I������
    // �G���X�g�����ׂč폜
    for (auto& enemy : enemy_list)
    {
        enemy->SetDestroy();
    }
    enemy_list.clear();
    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->RemoveAnimation(anim_id);
}

void Stage3::Update(float delta)
{
    // ����������A�j���[�V�����̐���
    InputManager* input = Singleton<InputManager>::GetInstance();
    if (input->GetKeyDown(KEY_INPUT_X))
    {
        AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
        // �A�j���[�V�����쐬
        manager->PlayerAnimation(EffectName::eExprotion, Vector2D(player->GetLocation().x, player->GetLocation().y), 0.05f, false);
    }

    UpdateBackgroundScroll(delta);

    //// �Q�[���̍��g�݂ƂȂ鏈�����A�����ɋL�q����
    //int spd = 1; // �X�N���[���̑���
    //if (distance == 0)
    //{
    //    spd = 0; // �{�X��̓X�N���[����~
    //}
    //scrollBG(spd); // �w�i�̃X�N���[��
    //moveEnemy(); // �G�@�̐���
    //moveBullet(); // �e�̐���
    //moveItem(); // �A�C�e���̐���
    //drawEffect(); // �G�t�F�N�g
    //stageMap(); // �X�e�[�W�}�b�v
    //drawParameter(); // ���@�̃V�[���h�Ȃǂ̃p�����[�^�[��\��

    timer++; // �^�C�}�[���J�E���g

    // delta_second �����Z
    stage_timer += delta;
    delta_draw = delta;  // �� delta ��ۑ�

    // ��莞�Ԃ��Ƃ� distance �����炷�i��F0.1�b���Ƃ�1���炷�j
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

    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    // ���t���[��
    manager->Update(delta);       // �X�V�ideltaTime�͑O�̃t���[���Ƃ̌o�ߎ��ԁj

    // �A�j���[�V�������I���������m�F�i�폜���K�v�ȏꍇ�j
    if (manager->GetAnimationFinished(anim_id) == true)
    {
        manager->RemoveAnimation(anim_id);  // �s�v�ɂȂ�����폜
    }

    // �v���C���[�̍U��
    PlayerShot();

    // �G�̏o��
    EnemyAppearance();

    // �G�̍U��
    EnemyShot(delta);

    // �x���\��
    DisplayWarning(delta);

    // �N���A����
    UpdateGameStatus(delta);

    if (is_clear)
    {
        if (result_started)
        {
            ResultDraw(delta);  
        }
    }

}

void Stage3::Draw()
{
    // �w�i�𔒂œh��i�ŏ��ɕ`��j
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    //�X�N���[���w�i
    DrawScrollBackground();  

    // ���̍���
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // �E�̍���
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "Stage3", GetColor(255, 255, 255));
    DrawFormatString(0, 20, GetColor(255, 255, 0), "�G��: %d", enemy_list.size());

    // �X�e�[�W�`��
    if (stage <= 1)
    {
        if (is_warning == true)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, brend);
            DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(255, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        }
    }

    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Draw();

    if (is_clear == true)
    {
        if (result_started)
        {
            // �����Ó]
            int fade_alpha = (result_timer * 12.0f < 60.0f) ? static_cast<int>(result_timer * 12.0f) : 60;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
            DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

            ResultDraw(delta_draw);  // �� �����ŕ`�揈�����Ăяo���I
        }

        // �uNEXT STAGE...�v�͏펞�\��
        DrawFormatStringToHandle((D_WIN_MAX_X / 2) - 100.0f, (D_WIN_MAX_Y / 2) + 120, GetColor(255, 255, 255), font_digital, "GAME CLEAR");
    }




    else if (is_over == true)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
        DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawFormatStringToHandle((D_WIN_MAX_X / 2) - 100.0f, (D_WIN_MAX_Y / 2), GetColor(255, 255, 255), font_digital, "GAME OVER");

        //DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "�Q�[���I�[�o�[", GetColor(0, 0, 0));
    }

    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    manager->Draw();                  // �`��
}

bool Stage3::IsFinished()
{
    return finished;
}

bool Stage3::IsClear()
{
    return is_clear;
}

bool Stage3::IsOver()
{
    return is_over;
}

StageBase* Stage3::GetNextStage(Player* player)
{
    return new Stage4(player); // ���̃X�e�[�W��
}

void Stage3::DisplayWarning(float delta_second)
{
    // �x���\���i�Ԃ�����j
    if (is_warning == true)
    {
        // �J�E���g���Z
        warning_timer += delta_second;
        if (warning_timer >= 0.0005f)
        {
            // �����x���O����Q�T�T�܂ŉ��Z���Z���J��Ԃ�
            if (is_brend == false)
            {
                if (brend < 255)
                {
                    brend++;
                }
                else
                {
                    is_brend = true;
                }
            }
            else
            {
                if (brend > 0)
                {
                    brend--;
                }
                else
                {
                    // ���B�J�E���g�����Z
                    reach_count++;
                    is_brend = false;
                }
            }
            // �J�E���g���Z�b�g
            warning_timer = 0;
        }
        // �R��x�����I�������S�ă��Z�b�g
        if (reach_count >= 3)
        {
            is_warning = false;
            brend = 255;
            warning_timer = 0;
            reach_count = 0;
        }
    }
}

void Stage3::PlayerShot()
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

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
}

// �G�̏o��
void Stage3::EnemyAppearance()
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // �G���P�ƎG���Q�̏o��
    if (300 < distance && distance % 1000 == 0)
    {
        // �o���͈͂���ʒ����̕�200�ɐݒ�
        int center_x = D_WIN_MAX_X / 2;
        int half_width = 250; // �o���͈͂̔����̕�
        int x = center_x - half_width + rand() % (half_width * 2);
        int y = -50;
        int e = 1 + rand() % 2;
        if (e == ENE_ZAKO1)
        {
            enemy_list.push_back(objm->CreateObject<Zako>(Vector2D(x, y)));
        }
        if (e == ENE_ZAKO2)
        {
            int vx = 0;
            if (player->GetLocation().x < x)
            {
                vx = -60;
            }
            if (player->GetLocation().x > x)
            {
                vx = 60;
            }
        }
    }
    // �U�R3�̏o��
    if (300 < distance && distance < 900 && distance % 400 == 0)
    {
        // �o���͈͂���ʒ����̕�200�ɐݒ�
        int center_x = D_WIN_MAX_X / 2;
        int half_width = 250; // �o���͈͂̔����̕�
        int x = center_x - half_width + rand() % (half_width * 2);
        int y = -50;
        enemy_list.push_back(objm->CreateObject<Zako3>(Vector2D(x, -100)));
    }
    // �x���\��
    if (distance == 150)
    {
        is_warning = true;
    }
    // �{�X�o��
    if (distance == 1 && boss2 == nullptr)
    {
        //enemy_list.push_back(boss = objm->CreateObject<Boss>(Vector2D(D_WIN_MAX_X / 2 + 200, D_WIN_MAX_Y + 200)));
        enemy_list.push_back(boss2 = objm->CreateObject<Boss2>(Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2)));
        
    }
}

// �G�̍U��
void Stage3::EnemyShot(float delta_second)
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // �G���e��ł������ł��Ă�����e�𔭎˂���
    for (int i = 0; i < enemy_list.size(); i++)
    {
        if (enemy_list[i]->GetIsShot() == true)
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
                //�e�L�̈ʒu����v���C���[�ւ̃x�N�g�������߂�
                Vector2D b = player->GetLocation() - enemy_list[i]->GetLocation();
                float c = sqrt(pow(b.x, 2) + pow(b.y, 2));
                //�v���C���[�Ɍ������Ēe��ł�
                e_shot2 = objm->CreateObject<EnemyShot2>(enemy_list[i]->GetLocation());
                e_shot2->SetVelocity(Vector2D(b.x / c, b.y / c));
                enemy_list[i]->SetIsShot();
            }
            else if (enemy_type == ENE_BOSS)
            {
                // �{�X�̍U���p�^�[�����擾
                bs_attack_pattrn = boss->GetAttackPattrn();
                Vector2D e_location = enemy_list[i]->GetLocation();

                // �U���p�^�[���P�i�������ɂR���j
                if (bs_attack_pattrn == 1)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        objm->CreateObject<EnemyShot1>(Vector2D((e_location.x - 30) + (30 * i), e_location.y + D_OBJECT_SIZE));
                    }
                    enemy_list[i]->SetIsShot();
                }
                // �U���p�^�[���Q�i�S�����ɂR���j
                else if (bs_attack_pattrn == 2)
                {
                    // �c�Q����
                    for (int i = 0; i < 2; i++)
                    {
                        for (int j = 0; j < 5; j++)
                        {
                            e_shot1 = objm->CreateObject<EnemyShot1>(Vector2D((e_location.x - 60) + (30 * j), e_location.y));
                            if (i == 0)
                            {
                                e_shot1->SetVelocity(Vector2D(0, 120));
                            }
                            else if (i == 1)
                            {
                                e_shot1->SetVelocity(Vector2D(0, -120));
                            }
                        }
                    }
                    // ���Q����
                    for (int i = 0; i < 2; i++)
                    {
                        for (int j = 0; j < 5; j++)
                        {
                            e_shot1 = objm->CreateObject<EnemyShot1>(Vector2D(e_location.x, (e_location.y - 60) + (30 * j)));
                            if (i == 0)
                            {
                                e_shot1->SetVelocity(Vector2D(-120, 0));
                            }
                            else if (i == 1)
                            {
                                e_shot1->SetVelocity(Vector2D(120, 0));
                            }
                        }
                    }
                    enemy_list[i]->SetIsShot();
                }
                // �U���p�^�[���R�i�O�i�K�̂�j
                else if (bs_attack_pattrn == 3)
                {
                    // �c�ɂR�悱�ɂT�����i�Е��j
                    for (int j = 1; j < 4; j++)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            e_shot3 = objm->CreateObject<EnemyShot3>(Vector2D(boss->GetLocation()));
                            e_shot3->SetPlayer(player);
                            e_shot3->SetPlayerLocation(player->GetLocation());
                            // +�P���Ȃ�������O�b�Ŕ��˂����
                            e_shot3->SetStepShot(j);
                            if (i < 5)
                            {
                                e_shot3->SetVelocity(Vector2D(240, 0));
                                e_shot3->SetPurposeLocation(Vector2D((e_location.x + 150) + (30 * i), (e_location.y + 40) - (50 * j)));
                            }
                            else
                            {
                                e_shot3->SetVelocity(Vector2D(-240, 0));
                                e_shot3->SetPurposeLocation(Vector2D(e_location.x - (30 * i), (e_location.y + 40) - (50 * j)));
                            }
                        }
                    }
                    enemy_list[i]->SetIsShot();
                }
            }
            // �{�X�Q�̍U���p�^�[��
            else if (enemy_type == ENE_BOSS2)
            {
            }
        }
    }
}

// �N���A����
void Stage3::UpdateGameStatus(float delta)
{
    // �{�X���|�ꂽ��N���A
    if (boss != nullptr && boss->GetIsAlive() == false && is_over == false)
    {
        boss->SetDestroy();
        is_clear = true;
    }

    // �{�X�Q���|�ꂽ��N���A
    if (boss2 != nullptr && boss2->GetIsAlive() == false && is_over == false)
    {
        boss2->SetDestroy();
        is_clear = true;
    }

    // �v���C���[���|�ꂽ��Q�[���I�[�o�[
    if (player != nullptr && player->GetIsAlive() == false && is_clear == false)
    {
        // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
        objm->Finalize();
        is_over = true;
    }

    if (is_clear == true || is_over == true)
    {
        scene_timer += delta;
        gameover_timer += delta;

        if (gameover_timer >= 0.005f)
        {
            transparent++;
            gameover_timer = 0.0f;
        }
        
     /*   if (scene_timer >= 8.0f)
        {
            finished = true;
        }*/
    }

    // ���̏����F�X�y�[�X�L�[����������X�e�[�W�I��
    if (CheckHitKey(KEY_INPUT_N))
    {
        finished = true;
    }

    if (is_clear == true && result_started == false)
    {
        clear_wait_timer += delta;
        if (clear_wait_timer >= 5.0f)
        {
            result_started = true;
            result_timer = 0.0f; // �X�R�A���o�^�C�}�[���Z�b�g
        }
    }


}

//���U���g
void Stage3::ResultDraw(float delta)
{
    if (!result_started) return;

    result_timer += delta * 60.0f;

    const int cx = D_WIN_MAX_X / 2;
    const int cy = D_WIN_MAX_Y / 2 - 20;

    // �t�F�[�h�F�����x60�܂�
    int fade_alpha = (result_timer * 12.0f < 60.0f) ? static_cast<int>(result_timer * 12.0f) : 60;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
    DrawBox(cx - 350, 0, cx + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // �t�F�[�h���X���C�h���o�ݒ�
    const int fade_duration = 30;
    const int slide_distance = 60;

    auto GetAlpha = [&](int delay) -> int {
        int t = static_cast<int>(result_timer) - delay;
        if (t < 0) return 0;
        if (t >= fade_duration) return 255;
        return (255 * t) / fade_duration;
    };

    auto GetSlideY = [&](int base_y, int delay) -> int {
        int t = static_cast<int>(result_timer) - delay;
        if (t < 0) return base_y + slide_distance;
        if (t >= fade_duration) return base_y;
        return base_y + slide_distance - (slide_distance * t) / fade_duration;
    };

    // �X�R�A�擾
    ScoreData* score = Singleton<ScoreData>::GetInstance();
    const auto& scores = score->GetScoreData();
    float base_score = 0.0f;
    for (float s : scores) base_score += s;
    int life_bonus = player->life * 1000;
    total_score = base_score + life_bonus;

    // �\�����C��
    struct ResultLine {
        int delay;
        int y_offset;
        std::string label;
        std::string format;  // ��Ȃ�Œ�e�L�X�g
    };

    std::vector<ResultLine> lines = {
        {  30, -80, "RESULT", "" },
        {  70, -20, "BASE SCORE", "BASE SCORE : %.0f" },
        { 110,  20, "LIFE BONUS", "LIFE BONUS : %d" },
        { 160,  60, "TOTAL SCORE", "TOTAL SCORE : %.0f" },
    };

    for (const auto& line : lines)
    {
        int alpha = GetAlpha(line.delay);
        int y = GetSlideY(cy + line.y_offset, line.delay);
        int color = GetColor(255, 255, 255);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        // �\����������
        char buf[128];
        if (line.format.empty())
        {
            sprintf_s(buf, "%s", line.label.c_str());
        }
        else if (line.label == "BASE SCORE")
        {
            sprintf_s(buf, line.format.c_str(), base_score);
        }
        else if (line.label == "LIFE BONUS")
        {
            sprintf_s(buf, line.format.c_str(), life_bonus);
        }
        else if (line.label == "TOTAL SCORE")
        {
            sprintf_s(buf, line.format.c_str(), total_score);
        }

        // �����񂹈ʒu���v�Z
        int width = GetDrawStringWidthToHandle(buf, strlen(buf), font_digital);
        DrawStringToHandle(cx - width / 2, y, buf, color, font_digital);

        if (line.label == "TOTAL SCORE" && alpha == 255)
        {
            result_displayed = true;
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // �X�R�A�\��������̑ҋ@�^�C�}�[
    if (result_displayed && !glitch_started && !glitch_done)
    {
        post_result_wait_timer += delta;
        if (post_result_wait_timer >= 8.0f) // 5�b�ҋ@��
        {
            glitch_started = true;
            glitch_timer = 0.0f;
        }
    }

    // �O���b�`���o�i�����o�[�W�����j
    if (glitch_started && !glitch_done)
    {
        for (int i = 0; i < 50; ++i) // �� ���𑝂₵�Ė��x�A�b�v�i����8�j
        {
            int x = (rand() % 700) + (D_WIN_MAX_X / 2 - 350);
            int y = rand() % D_WIN_MAX_Y;
            int w = 80 + rand() % 150; // �� ���L��
            int h = 8 + rand() % 40;   // �� ����������
            int r = 150 + rand() % 106;  // �Â߂̐F��������ƃm�C�Y�����o��
            int g = 150 + rand() % 106;
            int b = 255;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220); // �� �s�����x���߂ŕ���
            DrawBox(x, y, x + w, y + h, GetColor(r, g, b), TRUE);
        }

        glitch_timer += delta;
        if (glitch_timer > 2.0f)
        {
            glitch_done = true;
            finished = true;
        }
    }


    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}


//�X�N���[��
void Stage3::DrawScrollBackground() const
{
    static float time = 0.0f;
    time += 0.05f;

    // �w�i�F�F���Ԃ݂��������O���[
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(30, 10, 10), TRUE);

    // === �w�ʃ��C���[�i���E�ׂ����E�Â߁j ===
    const int grid_size_back = 40;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(100, 0, 0));

    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(100, 0, 0));
    }

    // === �O�ʃ��C���[�i�����E���邭�E�x�����j ===
    const int grid_size_front = 80;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(255, 40, 40), TRUE);

    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(255, 40, 40), TRUE);
    }

    // === �m�C�Y���o�i���󊴁j ===
    for (int i = 0; i < 5; ++i) {
        if (rand() % 70 == 0) {
            int nx = rand() % D_WIN_MAX_X;
            int ny = rand() % D_WIN_MAX_Y;
            DrawBox(nx, ny, nx + 3, ny + 3, GetColor(255, 100, 50), TRUE); // �I�����W�x���h�b�g
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
