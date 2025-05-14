#include "Stage3.h"
#include "Stage2.h"
#include "Stage1.h" // ���̃X�e�[�W������ꍇ

#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"

#include "../../../../Object/GameObjectManager.h"

#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"

#include <cstdlib> 

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

    UpdateBackgroundScroll(delta);

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
    // �w�i�𔒂œh��i�ŏ��ɕ`��j
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    DrawScrollBackground();  

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

    if (is_clear)
    {
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "�Q�[���N���A", GetColor(0, 0, 0));
    }
    else if (is_over)
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
    return new Stage3(player); // ���̃X�e�[�W��
}
void Stage2::EnemyAppearance(float delta)
{
    // 1�t���[�����Ƃɉ��Z�i60FPS�z��j
    enemy_spawn_timer += delta;

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // ���Ԋu���Ɓi5�b���Ɓj��Zako2�o��
    const float spawn_interval = 1.0f; // �o���Ԋu

    if (enemy_spawn_timer >= spawn_interval)
    {
        Vector2D spawn_pos(400.0f, 100.0f);
        zako2 = objm->CreateObject<Zako2>(spawn_pos);
        zako2->SetPlayer(player);
        enemy_spawn_timer = 0.0f; // �^�C�}�[���Z�b�g
        enemy_list.push_back(zako2);
    }

}

//�X�N���[���`��
void Stage2::DrawScrollBackground() const
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
