#include "ResultScene.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

// ����������
void ResultScene::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
/// <returns></returns>
eSceneType ResultScene::Update(float delta_second)
{
	return GetNowSceneType();
}

/// <summary>
/// �`�揈��
/// </summary>
/// <returns></returns>
void ResultScene::Draw()
{
	DrawString(0, 0, "Result", GetColor(255, 255, 255));
}

// �I���������i�g�����C���X�^���X�̍폜�Ƃ��j
void ResultScene::Finalize()
{
}

/// <summary>
/// ���݂̃V�[�����
/// </summary>
/// <returns>���݂̓��U���g�V�[���ł�</returns>
eSceneType ResultScene::GetNowSceneType() const
{
	return eSceneType::eResult;
}
