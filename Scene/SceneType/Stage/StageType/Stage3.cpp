#include "Stage3.h"
#include "Stage2.h" // ���̃X�e�[�W������ꍇ

#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Player/Player.h"
#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot1.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot3.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot4.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot5.h"
#include "../../../../Object/Character/Enemy/EnemyBase.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako3.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Object/Item/PowerUp/PowerUp.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage3.h"
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

}

void Stage3::Finalize()
{
    // �I������
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Finalize();
}

void Stage3::Update(float delta)
{
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

    DrawString(0, 0, "�Q�[�����C��", GetColor(255, 255, 255));
    DrawString(0, 300, "������@\n\n���X�e�B�b�N\n�\���{�^��\nWASD�L�[ : �ړ�\n\nA�{�^��\n�X�y�[�X�L�[ : ����\n\nB�{�^��\nB�L�[ : ���[�U�[\n\nRB�{�^��\nL�L�[ : �ˏo���]", GetColor(255, 255, 255));
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
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "�Q�[���N���A", GetColor(0, 0, 0));
    }
    else if (is_over == true)
    {
        DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "�Q�[���I�[�o�[", GetColor(0, 0, 0));
    }
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
    return nullptr; // ���̃X�e�[�W��
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
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
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
            enemy_list.push_back(objm->CreateObject<Zako1>(Vector2D(x, y)));
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
            enemy_list.push_back(zako2 = objm->CreateObject<Zako2>(Vector2D(x, -100)));
            zako2->SetVelocity(Vector2D(vx, 50));
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
                // �{�X�̍U���p�^�[�����擾
                bs_attack_pattrn = boss2->GetAttackPattrn();
                Vector2D e_location = enemy_list[i]->GetLocation();

                // �p�^�[���P�i�������ɂR���j
                if (bs_attack_pattrn == 1)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 30) + (30 * i), e_location.y + D_OBJECT_SIZE));
                        e_shot4->SetVelocity(Vector2D(0, 120));
                    }
                    enemy_list[i]->SetIsShot();
                }
                // �p�^�[���Q�i�S�����ɂR���j
                else if (bs_attack_pattrn == 2)
                {
                    // �c�Q����
                    for (int i = 0; i < 2; i++)
                    {
                        for (int j = 0; j < 5; j++)
                        {
                            e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 60) + (30 * j), e_location.y));
                            if (i == 0)
                            {
                                e_shot4->SetVelocity(Vector2D(0, 120));
                            }
                            else if (i == 1)
                            {
                                e_shot4->SetVelocity(Vector2D(0, -120));
                            }
                        }
                    }
                    // ���Q����
                    for (int i = 0; i < 2; i++)
                    {
                        for (int j = 0; j < 5; j++)
                        {
                            e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(e_location.x, (e_location.y - 60) + (30 * j)));
                            if (i == 0)
                            {
                                e_shot4->SetVelocity(Vector2D(-120, 0));
                            }
                            else if (i == 1)
                            {
                                e_shot4->SetVelocity(Vector2D(120, 0));
                            }
                        }
                    }
                    enemy_list[i]->SetIsShot();
                }
                // �U���p�^�[�����R�i�i�K�̍U���j
                else if (bs_attack_pattrn == 3)
                {
                    // �c�ɂR�悱�ɂT�����i�Е��j
                    for (int j = 1; j < 4; j++)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            e_shot3 = objm->CreateObject<EnemyShot3>(Vector2D(boss2->GetLocation()));
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
                else if (bs_attack_pattrn == 4)
                {
                    /// <summary>
                    /// �U���p�^�[���S�i�ԉ΁j
                    /// </summary>
                    /// <param name="bullet_num">�e�̐�</param>
                    /// <param name="speed">�e�̑��x</param>
                    /// <param name="spiral_interval">���ˊԊu</param>
                    /// <param name="spiral_duration_limit">�U�����鎞��</param>
                    /// <param name="generate_location">��������ʒu</param>
                    /// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
                    Pattrn4(24, 150.0f, 3.0f, 0.3f, boss2->GetLocation(), delta_second);
                }
                // �U���p�^�[���T�i�Q�����j
                else if (bs_attack_pattrn == 5)
                {
#if 0
                    static float spiral_timer = 0.0f;            // �e���˂̃C���^�[�o���Ǘ�
                    static float spiral_angle = 0.0f;            // ��]�p�x
                    static float spiral_total_time = 0.0f;       // �p�^�[��5�ɓ����Ă���̗݌v����
                    const float spiral_interval = 0.1f;
                    const float spiral_duration_limit = 3.0f;    // �ő唭�ˎ���

                    // 3�b�o�߂����牽�����Ȃ�
                    if (spiral_total_time >= spiral_duration_limit) return;

                    spiral_timer += delta_second;
                    spiral_total_time += delta_second;

                    if (spiral_timer >= spiral_interval)
                    {
                        spiral_timer = 0.0f;

                        float rad = spiral_angle * DX_PI / 180.0f;
                        float speed = 160.0f;
                        Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

                        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
                        EnemyShot4* shot = objm->CreateObject<EnemyShot4>(boss2->GetLocation());
                        shot->SetVelocity(velocity);

                        spiral_angle += 15.0f;
                        if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
                    }
#else

                    /// <summary>
                    /// �U���p�^�[���T�i�Q�����j
                    /// </summary>
                    /// <param name="spiral_interval">���ˊԊu</param>
                    /// <param name="spiral_duration_limit">�U�����鎞��</param>
                    /// <param name="spiral_speed">�e�̑��x</param>
                    /// <param name="generate_location">��������ʒu</param>
                    /// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
                    Pattrn5(0.3f, 3.0f, 160.0f, boss2->GetLocation(), delta_second);
#endif
                }
                else if (bs_attack_pattrn == 6)
                {
                    /// <summary>
                    /// �U���p�^�[���U�i�o���o����^�j
                    /// </summary>
                    /// <param name="fan_angle_range">��̊p�x�i60�Ȃ�}30���j</param>
                    /// <param name="bullet_speed">�e�̑��x</param>
                    /// <param name="fan_interval">���ˊԊu</param>
                    /// <param name="fan_duration_limit">�U�����鎞��</param>
                    /// <param name="generate_location">��������ʒu</param>
                    /// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
                    Pattrn6(0.0f, 180.0f, 0.2f, 3.0f, boss2->GetLocation(), delta_second);
                }
                else if (bs_attack_pattrn == 7)
                {
                    /// <summary>
                    /// �U���p�^�[���V�i�i�K��`�j
                    /// </summary>
                    /// <param name="fan_angle_range">��̊p�x�͈́i60�Ȃ�}30���j</param>
                    /// <param name="bullet_speed">�e�̑��x</param>
                    /// <param name="fan_interval">���˂̊Ԋu</param>
                    /// <param name="fan_duration_limit">�U������</param>
                    /// <param name="generate_location">��������ʒu</param>
                    /// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
                    Pattrn7(60.0f, 180.0f, 0.4f, 3.0f, boss2->GetLocation(), delta_second);
                }
                else if (bs_attack_pattrn == 8)
                {
                    /// <summary>
                    /// �U���p�^�[���W�i�Ђ�Q�j
                    /// </summary>
                    /// <param name="wave_interval">���ˊԊu</param>
                    /// <param name="wave_duration_limit">���Ԏ��Ԃ̏��</param>
                    /// <param name="generate_location">��������ʒu</param>
                    /// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
                    Pattrn8(0.1f, 1.0f, boss2->GetLocation(), delta_second);
                }
                else if (bs_attack_pattrn == 9)
                {
                    /// <summary>
                    /// �U���p�^�[���X�i�{�X���S��]�e�j
                    /// </summary>
                    /// <param name="shot_count">�e�̐�</param>
                    /// <param name="radius">�{�X����̋���</param>
                    /// <param name="angular_speed">��]���x</param>
                    /// <param name="bullet_speed">�e��</param>
                    /// <param name="rotation_timer">��]�̎��Ԍv��</param>
                    /// <param name="generate_location">��������ʒu</param>
                    /// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
                    Pattrn9(27, 150.0f, 90.0f, 0.0f, boss2->GetLocation(), delta_second);
                }
                else if (bs_attack_pattrn == 10)
                {
                    /// <summary>
                    /// �U���p�^�[���P�O�i�x�N�g�����S��]�e�j
                    /// </summary>
                    /// <param name="shot_count">�e�̐�</param>
                    /// <param name="radius">�{�X����̋���</param>
                    /// <param name="angular_speed">��]���x</param>
                    /// <param name="bullet_speed">�e��</param>
                    /// <param name="rotation_timer">��]�̎��Ԍv��</param>
                    /// <param name="center_location">��������ꏊ</param>
                    /// <param name="generate_location">��������ꏊ</param>
                    /// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
                    Pattrn10(8, 120.0f, 90.0f, 150.0f, 5.0f, boss2->GetLocation(), delta_second);
                }
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
        if (scene_timer >= 5.0f)
        {
            finished = true;
        }
    }

    // ���̏����F�X�y�[�X�L�[����������X�e�[�W�I��
    if (CheckHitKey(KEY_INPUT_N))
    {
        finished = true;
    }
}

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

/// <summary>
/// �U���p�^�[���S
/// </summary>
/// <param name="bullet_num">�e�̐�</param>
/// <param name="speed">�e�̑��x</param>
/// <param name="spiral_interval">���ˊԊu</param>
/// <param name="spiral_duration_limit">�U�����鎞��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Stage3::Pattrn4(int bullet_num, float speed, float spiral_interval, float spiral_duration_limit, const Vector2D& generate_location, float delta_second)
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static float spiral_timer = 0.0f;
    static float spiral_total_time = 0.0f;

    spiral_timer += delta_second;
    spiral_total_time += delta_second;

    if (spiral_timer >= spiral_interval)
    {
        spiral_timer = 0.0f;

        for (int i = 0; i < bullet_num; i++)
        {
            float angle = (360.0f / bullet_num) * i;
            float rad = angle * DX_PI / 180.0f;

            Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

            EnemyShot4* e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
            e_shot4->SetVelocity(velocity);
        }
    }

    // ���Ԑ����𒴂�����I���i���˂��Ȃ��j
    if (spiral_total_time >= spiral_duration_limit)
    {
        spiral_total_time = 0.0f;
        boss2->SetIsShot();
    }
}
/// <summary>
/// �U���p�^�[���T
/// </summary>
/// <param name="spiral_interval">���ˊԊu</param>
/// <param name="spiral_duration_limit">�U�����鎞��</param>
/// <param name="spiral_speed">�e�̑��x</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Stage3::Pattrn5(float spiral_interval, float spiral_duration_limit, float spiral_speed, const Vector2D& generate_location, float delta_second)
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static float spiral_timer = 0.0f;
    static float spiral_angle = 0.0f;
    static float spiral_total_time = 0.0f;

    spiral_timer += delta_second;
    spiral_total_time += delta_second;

    if (spiral_timer >= spiral_interval)
    {
        spiral_timer = 0.0f;

        for (int dir = 0; dir < 2; dir++)
        {
            float base_angle = (dir == 0) ? 90.0f : 270.0f;
            float current_angle = base_angle + spiral_angle;

            float rad = current_angle * DX_PI / 180.0f;
            Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

            EnemyShot4* shot = objm->CreateObject<EnemyShot4>(generate_location);
            shot->SetVelocity(velocity);
        }

        spiral_angle += 10.0f;
        if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
    }

    // ���Ԑ����𒴂�����I���i���˂��Ȃ��j
    if (spiral_total_time >= spiral_duration_limit)
    {
        spiral_total_time = 0.0f;
        boss2->SetIsShot();
    }
}

/// <summary>
/// �U���p�^�[���U
/// </summary>
/// <param name="fan_angle_range">��̊p�x�i60�Ȃ�}30���j</param>
/// <param name="bullet_speed">�e�̑��x</param>
/// <param name="fan_interval">���ˊԊu</param>
/// <param name="fan_duration_limit">�U�����鎞��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Stage3::Pattrn6(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // �U�������p static �ϐ�
    static float fan_timer = 0.0f;
    static float fan_total_time = 0.0f;

    // �U���p�^�[�����ύX���ꂽ�烊�Z�b�g������ �� boss2���ŏ�Ԏ�������̂�����

    fan_timer += delta_second;
    fan_total_time += delta_second;

    // ���Ԋu���Ƃɔ���
    if (fan_timer >= fan_interval)
    {
        fan_timer = 0.0f;

        // �����_���Ȋp�x�i���S�}60���j
        float base_angle = 90.0f; // ���������S
        float random_angle = base_angle - fan_angle_range / 2.0f + (rand() % (int)fan_angle_range);

        float rad = random_angle * DX_PI / 180.0f;
        Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

        e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
        e_shot4->SetVelocity(velocity);
    }

    // ���Ԑ����𒴂�����I���i���˂��Ȃ��j
    if (fan_total_time >= fan_duration_limit)
    {
        fan_total_time = 0.0f;
        boss2->SetIsShot();
    }
}

/// <summary>
/// �U���p�^�[���V
/// </summary>
/// <param name="fan_angle_range">��̊p�x�͈́i60�Ȃ�}30���j</param>
/// <param name="bullet_speed">�e�̑��x</param>
/// <param name="fan_interval">���˂̊Ԋu</param>
/// <param name="fan_duration_limit">�U������</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Stage3::Pattrn7(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static float fan_timer = 0.0f;
    static float fan_total_time = 0.0f;

    fan_timer += delta_second;
    fan_total_time += delta_second;

    if (fan_timer >= fan_interval)
    {
        fan_timer = 0.0f;

        int bullet_count = 3;
        float base_angle = 90.0f; // ���S������

        for (int i = 0; i < bullet_count; ++i)
        {
            float angle = base_angle - fan_angle_range / 2.0f +
                (fan_angle_range / (bullet_count - 1)) * i;

            float rad = angle * DX_PI / 180.0f;
            Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

            e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
            e_shot4->SetVelocity(velocity);
        }
    }

    // ��莞�Ԍo�߂�����I��
    if (fan_total_time >= fan_duration_limit)
    {
        fan_total_time = 0.0f;
        boss2->SetIsShot();
    }
}

/// <summary>
/// �U���p�^�[���W
/// </summary>
/// <param name="wave_interval">���ˊԊu</param>
/// <param name="wave_duration_limit">���Ԏ��Ԃ̏��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Stage3::Pattrn8(float wave_interval, float wave_duration_limit, const Vector2D& generate_location, float delta_second)
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

#if 1
    static float wave_timer = 0.0f;
    static float wave_total_time = 0.0f;

    wave_timer += delta_second;
    wave_total_time += delta_second;

    if (wave_timer >= wave_interval)
    {
        wave_timer = 0.0f;
        Vector2D e_lo = generate_location;

        // �E��
        e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x + 50, e_lo.y));
        e_shot5->SetWaveReflected(false);
        e_shot5->SetVelocity(Vector2D(0, 200));
        e_shot5->SetWaveParameters(400.0f, 0.7f);

        // ����
        e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x - 50, e_lo.y));
        e_shot5->SetWaveReflected(true);
        e_shot5->SetVelocity(Vector2D(0, 200));
        e_shot5->SetWaveParameters(400.0f, 0.7f);
    }

    // ��莞�Ԍo�߂�����I��
    if (wave_total_time >= wave_duration_limit)
    {
        wave_total_time = 0.0f;
        boss2->SetIsShot();
    }
#else

    int num_shots = 20;
    float spread_speed = 150.0f;
    Vector2D origin = e_location; // �e�̏������ˈʒu�i�{�X�̈ʒu�j
    Vector2D boss_center = boss2->GetLocation(); // �z�����ݒ��S�I

    for (int i = 0; i < num_shots; ++i)
    {
        float angle = 360.0f / num_shots * i;
        float rad = angle * DX_PI / 180.0f;

        e_shot5 = objm->CreateObject<EnemyShot5>(origin);
        e_shot5->SetVelocity(Vector2D(cosf(rad), sinf(rad)) * spread_speed);
        e_shot5->SetSuckCenter(boss_center); // �z�����ݐ���Z�b�g�I
    }
#endif

}

/// <summary>
/// �U���p�^�[���X
/// </summary>
/// <param name="shot_count">�e�̐�</param>
/// <param name="radius">�{�X����̋���</param>
/// <param name="angular_speed">��]���x</param>
/// <param name="bullet_speed">�e��</param>
/// <param name="rotation_timer">��]�̎��Ԍv��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Stage3::Pattrn9(int shot_count, float radius, float angular_speed, float bullet_speed, const Vector2D& generate_location, float delta_second)
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static std::vector<float> angles;    // �e�e�̊p�x�Ǘ�
    static std::vector<EnemyShot4*> rotating_shots;  // �e�̃|�C���^��ێ�

    static float rotation_timer = 0.0f;

    rotation_timer += delta_second;

    // ����������
    if (angles.empty())
    {
        rotating_shots.clear();
        // �e�𐶐����ď����p�x��ݒ�
        for (int i = 0; i < shot_count; ++i)
        {
            float angle = 360.0f / shot_count * i;  // �e�̊p�x�ݒ�
            angles.push_back(angle);

            // �{�X�̎��͂ɒe��z�u
            e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);

            if (e_shot4)
            {
                rotating_shots.push_back(e_shot4);  // �������������ꂽ��e��ۑ�
                e_shot4->SetVelocity(Vector2D(0, 0)); // ��]�����Ȃ̂Œe����0
            }
        }
    }

    // �e�e�̈ʒu���X�V���ĉ�]������
    for (int i = 0; i < shot_count; ++i)
    {
        angles[i] += angular_speed * delta_second; // �p�x�X�V
        if (angles[i] >= 360.0f) angles[i] -= 360.0f; // �p�x��0?360�x�ɒ���

        float rad = angles[i] * DX_PI / 180.0f;  // ���W�A���ɕϊ�

        // �{�X�𒆐S�ɔ��a `radius` �̉~�O����`��
        Vector2D new_pos = generate_location + Vector2D(cos(rad) * radius, sin(rad) * radius);

        // �e�̈ʒu���X�V
        if (rotating_shots[i])  // �e�����������݂��邩�m�F
        {
            rotating_shots[i]->SetLocation(new_pos);

            // �e�̑��x�̓[���i��]�̂݁j
            rotating_shots[i]->SetVelocity(Vector2D(0, 0));
        }
    }

    // �U���̏I������i��莞�Ԍo�ߌ�j
    static float duration_timer = 0.0f;
    duration_timer += delta_second;
    if (duration_timer >= 5.0f)  // 5�b��ɏI��
    {
        for (int i = 0; i < shot_count; ++i)
        {
            if (rotating_shots[i])
            {
                rotating_shots[i]->SetDestroy();  // �e���폜
            }

        }
        angles.clear();  // �p�x�����N���A
        rotating_shots.clear();  // �e�̃��X�g���N���A
        duration_timer = 0.0f;
        boss2->SetIsShot();  // �U���I��
    }
}

/// <summary>
/// �U���p�^�[���P�O
/// </summary>
/// <param name="shot_count">�e�̐�</param>
/// <param name="radius">�{�X����̋���</param>
/// <param name="angular_speed">��]���x</param>
/// <param name="bullet_speed">�e��</param>
/// <param name="rotation_timer">��]�̎��Ԍv��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Stage3::Pattrn10(int shot_count, float radius, float angular_speed, float center_speed, float duration_limit, const Vector2D& generate_location, float delta_second)
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    static float duration_timer = 0.0f;
    static Vector2D center_pos;  // �����Ȃ����S���W
    static Vector2D center_velocity;
    static std::vector<float> angles;
    static std::vector<EnemyShot4*> rotating_shots;
    static bool initialized = false;

    // ���Ԍo��
    duration_timer += delta_second;

    if (duration_timer < duration_limit)
    {
        if (!initialized) // ����������
        {
            // �{�X�̈ʒu���������S�Ƃ��āA�^���Ɉړ�������
            center_pos = generate_location;
            center_velocity = Vector2D(0.0f, center_speed); // �^������

            angles.clear();
            rotating_shots.clear();
            for (int i = 0; i < shot_count; ++i)
            {
                float angle = 360.0f / shot_count * i;
                angles.push_back(angle);

                EnemyShot4* shot = objm->CreateObject<EnemyShot4>(center_pos);
                if (shot)
                {
                    shot->SetVelocity(Vector2D(0, 0));
                    rotating_shots.push_back(shot);
                }
            }

            initialized = true;
        }

        // �����Ȃ����S�̈ړ�
        center_pos += center_velocity * delta_second;

        // ��]�e�̈ʒu�X�V
        for (int i = 0; i < shot_count; ++i)
        {
            angles[i] += angular_speed * delta_second;
            if (angles[i] >= 360.0f) angles[i] -= 360.0f;

            float rad = angles[i] * DX_PI / 180.0f;
            Vector2D offset(cos(rad) * radius, sin(rad) * radius);

            if (rotating_shots[i])
            {
                rotating_shots[i]->SetLocation(center_pos + offset);
            }
        }
    }
    else
    {
        // �U���I����̏���
        duration_timer = 0.0f;
        boss2->SetIsShot();  // �U���I��
        initialized = false;  // �������t���O�����Z�b�g
    }
}