#include "Stage2.h"
#include "Stage1.h" // ���̃X�e�[�W������ꍇ

#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"

#include "../../../../Object/GameObjectManager.h"


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
    return nullptr; // ���̃X�e�[�W��
}