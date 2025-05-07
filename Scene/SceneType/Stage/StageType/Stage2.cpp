#include "Stage3.h"
#include "Stage2.h"
#include "Stage1.h" // ���̃X�e�[�W������ꍇ

#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"

#include "../../../../Object/GameObjectManager.h"

#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"



Stage2::Stage2(Player* player) : StageBase(player) {}
Stage2::~Stage2() {}

void Stage2::Initialize() 
{
    // ����������

}

void Stage2::Finalize() 
{
    // �I������
        // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Finalize();

}

void Stage2::Update(float delta) 
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);
    EnemyAppearance(delta);

    stage_timer += delta; 

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


    // ���̏����F�X�y�[�X�L�[����������X�e�[�W�I��
    if (CheckHitKey(KEY_INPUT_N)) 
    {
        finished = true;
    }
    // ���̏����F�X�y�[�X�L�[����������X�e�[�W�I��
    if (CheckHitKey(KEY_INPUT_U)) 
    {
        is_clear = true;
    }
    // ���̏����F�X�y�[�X�L�[����������X�e�[�W�I��
    if (CheckHitKey(KEY_INPUT_I)) 
    {
        is_over = true;
    }
}

void Stage2::Draw() 
{
    DrawString(100, 100, "Stage 2", GetColor(255, 255, 255));

    DrawString(0, 0, "�Q�[�����C��", GetColor(255, 255, 255));

    DrawString(0, 300, "������@\n\n���X�e�B�b�N\n�\���{�^��\nWASD�L�[ : �ړ�\n\nA�{�^��\n�X�y�[�X�L�[ : ����\n\nB�{�^��\nB�L�[ : ���[�U�[\n\nRB�{�^��\nL�L�[ : �ˏo���]", GetColor(255, 255, 255));

    DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);


    


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

    // ���̍���
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // �E�̍���
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
}

bool Stage2::IsFinished() 
{
    return finished;
}

bool Stage2::IsClear()
{
    return is_clear;
}

bool Stage2::IsOver()
{
    return is_over;
}

StageBase* Stage2::GetNextStage(Player* player) 
{
    return new Stage3(player); // ���̃X�e�[�W��
}
void Stage2::EnemyAppearance(float delta)
{
    enemy_spawn_timer += 1.0f / 60.0f; // 1�t���[�����Ƃɉ��Z�i60FPS�z��j

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

        // Zako2���o�������鏈���iStage1�̐i�s�ɍ��킹�āj�����̂܂܎c��
        if (stage_timer >= 5.0f && !zako4_spawned)
        {
            printf("Zako2 is about to appear!\n");

            Vector2D spawn_pos(400.0f, 100.0f);
            zako2 = objm->CreateObject<Zako2>(spawn_pos);
            if (zako2 != nullptr)
            {
                zako2->Initialize(); 
                enemy_list.push_back(zako2);
                zako4_spawned = true;
            }

        }

        // �^�C�}�[���Z�b�g
        enemy_spawn_timer = 0.0f;
    
}
void Stage2::DrawScrollBackground() const
{
    const int grid_size1 = 80;  // �w�ʃO���b�h
    const int grid_size2 = 40;  // �O�ʃO���b�h
    const int alpha1 = 60;
    const int alpha2 = 100;

    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(10, 30, 30), TRUE); // �O���[�����̂���w�i

    // �w�ʃO���b�h�i���C���[1�j
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha1);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size1)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, 100, 255));
    for (int y = -grid_size1; y < D_WIN_MAX_Y + grid_size1; y += grid_size1) {
        int sy = y - (int)bg_scroll_offset_layer1 % grid_size1;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(0, 100, 255));
    }

    // �O�ʃO���b�h�i���C���[2�j
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha2);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size2)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(180, 0, 255));
    for (int y = -grid_size2; y < D_WIN_MAX_Y + grid_size2; y += grid_size2) {
        int sy = y - (int)bg_scroll_offset_layer2 % grid_size2;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(180, 0, 255));
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
