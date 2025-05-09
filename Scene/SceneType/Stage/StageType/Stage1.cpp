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
#include "../../../../Object/Character/Enemy/EnemyType/Zako2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako3.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Scene/SceneType/Stage/StageType/Stage1.h"

#include"../../../../Object/Item/Exp/Exp.h"

#include <math.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()

Stage1::Stage1(Player* player)
    : StageBase(player),
    zako2(nullptr),
    zako4(nullptr),
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

    // �����������i1�񂾂��s���j
    srand(static_cast<unsigned int>(time(NULL)));

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

}

void Stage1::Update(float delta)
{
    UpdateBackgroundScroll(delta);

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
   // �^�C�}�[2�ɕ�����
    stage_timer += delta;          // 10�b����p
    distance_timer += delta;       // �X�N���[���p

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
    if (stage_timer >= 60.0f)
    {
        is_clear = true;
    }


    // �G�̏o��
    EnemyAppearance(delta);

  

    // �G���|���ꂽ���͌o���l�𐶐��B
    for (auto& enemy : enemy_list)
    {
        if (enemy->is_destroy == true)
        {
            Exp* exp = Singleton<GameObjectManager>::GetInstance()->CreateObject<Exp>(enemy->GetLocation());
            exp->SetPlayer(player);
        }
    }

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


    if (is_clear == true || is_over == true)
    {
        // �G�S�폜�i���ɍ폜����Ă�����͉̂����N���Ȃ��j
        for (auto& enemy : enemy_list)
        {
            enemy->SetDestroy();
        }

        enemy_list.clear(); // �Ǘ����X�g���N���A

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

    DrawString(0, 0, "�Q�[�����C��", GetColor(255, 255, 255));
    DrawString(0, 300, "������@\n\n���X�e�B�b�N\n�\���{�^��\nWASD�L�[ : �ړ�\n\nA�{�^��\n�X�y�[�X�L�[ : ����\n\nB�{�^��\nB�L�[ : ���[�U�[\n\nRB�{�^��\nL�L�[ : �ˏo���]", GetColor(255, 255, 255));
    DrawFormatString(0, 20, GetColor(255, 255, 0), "�G��: %d", enemy_list.size());
    // �X�e�[�W�^�C�}�[�̕\���i�E��j
    DrawFormatString(D_WIN_MAX_X - 200, 20, GetColor(255, 255, 255), "Time: %.1f", stage_timer);


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

void Stage1::EnemyAppearance(float delta)
{
    // �^�C�}�[���Z
    enemy_spawn_timer += delta;

    const float spawn_interval = 3.0f;

    // 5�b���Ƃ�1�̂����o��
    if (enemy_spawn_timer >= spawn_interval)
    {
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

        Vector2D spawn_pos(400.0f, 100.0f);
        zako1 = objm->CreateObject<Zako1>(spawn_pos);
        enemy_list.push_back(zako1);
        zako1->SetPlayer(player);

        // �p�^�[�������Ԃɉ����Đ؂�ւ�
        if (stage_timer < 30.0f)
        {
            zako1->SetPattern(Zako1Pattern::MoveStraight); // �O��
        }
        else
        {
            zako1->SetPattern(Zako1Pattern::MoveZigzag);   // �㔼
        }

        // �^�C�}�[���Z�b�g���Ď��̏o���ɔ�����
        enemy_spawn_timer = 0.0f;
    }
}




//�X�N���[���`��
void Stage1::DrawScrollBackground() const
{
    // �w�i�F�i����߂̍��j
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(20, 20, 40), TRUE);

    // �w�ʃO���b�h�i�����E�ׂ����E���ׂ��j
    const int grid_size_back = 40;
    const int alpha_back = 100;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_back);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, 100, 255)); // �l�I���u���[�i���j

    //�摜���Ƃ�����ύX
    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back)
    {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(0, 100, 255));
    }

    /**     �񖇖�     **/

    // ��O�O���b�h�i�߂��E�L�߁E�����߁j
    const int grid_size_front = 80;
    const int alpha_front = 120;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_front);

    // �������C���p�̎l�p�i�c���C���j
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(180, 0, 255), TRUE); // ��O�͑��߃p�[�v��

    //�摜���Ƃ�����ύX
    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(180, 0, 255), TRUE); // �����ŉ��ߊ�
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

