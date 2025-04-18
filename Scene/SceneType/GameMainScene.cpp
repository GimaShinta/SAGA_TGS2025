#include "GameMainScene.h"
#include "../../Scene/SceneType/Stage/StageType/Stage1.h"

GameMainScene::GameMainScene() : player(nullptr), current_stage(nullptr), next_scene_timer(0.0f)
{
}

GameMainScene::~GameMainScene()
{
}

// ����������
void GameMainScene::Initialize()
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    player = objm->CreateObject<Player>(Vector2D(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2));

    current_stage = new Stage1(player);
    current_stage->Initialize();
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
/// <returns></returns>
eSceneType GameMainScene::Update(float delta_second)
{
    if (current_stage)
    {
        current_stage->Update(delta_second);

        if (current_stage->IsFinished())
        {
            if (current_stage->IsClear() == true)
            {
                StageBase* next_stage = current_stage->GetNextStage(player);

                current_stage->Finalize();
                delete current_stage;
                current_stage = nullptr;

                if (next_stage != nullptr)
                {
                    current_stage = next_stage;
                    current_stage->Initialize();
                }
                else
                {
                    // �ŏI�X�e�[�W���I������烊�U���g��
                    return eSceneType::eTitle;
                }
            }
            else if (current_stage->IsOver() == true)
            {
                current_stage->Finalize();
                delete current_stage;
                current_stage = nullptr;

                // �ŏI�X�e�[�W���I������烊�U���g��
                return eSceneType::eTitle;
            }
        }
    }

	return GetNowSceneType();
}

/// <summary>
/// �`�揈��
/// </summary>
/// <returns></returns>
void GameMainScene::Draw()
{
    if (current_stage) 
    {
        current_stage->Draw();
    }
}

// �I���������i�g�����C���X�^���X�̍폜�Ƃ��j
void GameMainScene::Finalize()
{
    if (current_stage) 
    {
        current_stage->Finalize();
        delete current_stage;
        current_stage = nullptr;
    }
}

/// <summary>
/// ���݂̃V�[�����
/// </summary>
/// <returns>���݂̓��U���g�V�[���ł�</returns>
eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::eGameMain;
}
