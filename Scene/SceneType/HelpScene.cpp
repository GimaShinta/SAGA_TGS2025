#include "HelpScene.h"

HelpScene::HelpScene()
{
}

HelpScene::~HelpScene()
{
}

// ����������
void HelpScene::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
/// <returns></returns>
eSceneType HelpScene::Update(float delta_second)
{
	return GetNowSceneType();
}

/// <summary>
/// �`�揈��
/// </summary>
/// <returns></returns>
void HelpScene::Draw()
{
}

// �I���������i�g�����C���X�^���X�̍폜�Ƃ��j
void HelpScene::Finalize()
{
}

/// <summary>
/// ���݂̃V�[�����
/// </summary>
/// <returns>���݂̓��U���g�V�[���ł�</returns>
eSceneType HelpScene::GetNowSceneType() const
{
	return eSceneType::eHelp;
}
