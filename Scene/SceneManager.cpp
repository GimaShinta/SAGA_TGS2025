#include "SceneManager.h"
#include "DxLib.h"

// �e�V�[���̃C���N���[�h�i�K�v�ȃV�[���͉��ɒǉ��j
#include "SceneType/StartScene.h"
#include "SceneType/TitleScene.h"
#include "SceneType/HelpScene.h"
#include "SceneType/GameMainScene.h"
#include "SceneType/ResultScene.h"

// Singleton�p���N���X�̃C���N���[�h
#include "../Utility/InputManager.h"

// �R���X�g���N�^
SceneManager::SceneManager() : current_scene(nullptr)
{
}

// �f�X�g���N�^
SceneManager::~SceneManager()
{
}

// �C���X�^���X�i�I�u�W�F�N�g�ւ̎Q�Ɓj�̍폜
void SceneManager::DeleteInstance()
{
	// ���N���X�̃|�C���^�i���̂��A�h���X�̐�ŕۗL�j
	static SceneManager* instance = nullptr;

	// �C���X�^���X�����݂��Ă���ꍇ�A�폜����
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

// ����������
void SceneManager::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
/// <returns>�I�����邩���Ȃ����itrue�Ȃ�I���Afalse�Ȃ�I�����Ȃ��j</returns>
bool SceneManager::Update(float delta_second)
{
	// ���͏����擾����
	InputManager* input = Singleton<InputManager>::GetInstance();

	// �V�[���̍X�V
	eSceneType next_scene_type = current_scene->Update(delta_second);

	// ���݂̃V�[���^�C�v�����̃V�[���^�C�v�ƈ���Ă��邩�H
	if (current_scene->GetNowSceneType() != next_scene_type)
	{
		// ����Ă�����V�[���̐؂�ւ�����
		ChangeScene(next_scene_type);
	}

	// �Q�[�����I�����邩�m�F����
	if (next_scene_type == eSceneType::eXit ||
		(input->GetButtonUp(XINPUT_BUTTON_BACK)) ||
		input->GetKeyUp(KEY_INPUT_ESCAPE))
	{
		// �U���J�n
		StartJoypadVibration(DX_INPUT_PAD1, 1000, 1000, -1);

		// �P�b�Ԍo�߂���܂ő҂�
		WaitTimer(1000);

		//�G���h�`�F�b�N�i�I������j
		return true;
	}

	// �G���h�`�F�b�N�i�I�����Ȃ��j
	return false;
}

// �`�揈��
void SceneManager::Draw()
{
	// current_scene�ɓ����Ă���V�[���^�C�v�̕`�揈���ֈړ�
	current_scene->Draw();
}

// �I���������i�g�����C���X�^���X�̍폜�Ƃ��j
void SceneManager::Finalize()
{
	// �V�[����񂪐�������Ă���΁A�폜����
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
		current_scene = nullptr;
	}
	// �C���X�^���X�i�N���X�ւ̎Q�Ɓj�̍폜
	InputManager::DeleteInstance();
}

/// <summary>
/// �V�[���̐؂�ւ�
/// </summary>
/// <param name="scene_type">�؂�ւ������V�[���̃^�C�v</param>
void SceneManager::ChangeScene(eSceneType type)
{
	// �����œn���ꂽ�؂�ւ������V�[�����쐬���āA�ϐ��ɑ��
	SceneBase* new_scene = CreateScene(type);

	// �G���[�`�F�b�N
	if (new_scene == nullptr)
	{
		throw("�V�[���������ł��܂���ł���\n");
	}

	// ���݃V�[���̏I������
	if (current_scene != nullptr)
	{
		current_scene->Finalize();
		delete current_scene;
	}

	// �V�����V�[���̏�����
	new_scene->Initialize();

	// ���݂̃V�[����V�����V�[���ɂ���
	current_scene = new_scene;
}

/// <summary>
/// �V�[�����̐���
/// </summary>
/// <param name="scene_type">�����������V�[���^�C�v</param>
/// <returns>�^�C�g���V�[���N���X�̏��</returns>
/// <returns>�I���V�[���N���X�̏��</returns>
/// <returns>�o�g���V�[���N���X�̏��</returns>
/// <returns>���U���g�V�[���N���X�̏��</returns>
/// <returns>�����Ԃ��Ȃ�</returns>
SceneBase* SceneManager::CreateScene(eSceneType type)
{
	// �����̃V�[���^�C�v�ɂ���āA��������V�[����؂�ւ���
	switch (type)
	{
	case eSceneType::eStart:
		return dynamic_cast<SceneBase*>(new StartScene());
	case eSceneType::eTitle:
		return dynamic_cast<SceneBase*>(new TitleScene());
	case eSceneType::eHelp:
		return dynamic_cast<SceneBase*>(new HelpScene());
	case eSceneType::eGameMain:
		return dynamic_cast<SceneBase*>(new GameMainScene());
	case eSceneType::eResult:
		return dynamic_cast<SceneBase*>(new GameMainScene());
	case eSceneType::eXit:
		return nullptr;  // ����I���Ȃ̂�throw�����Ȃ�
	default:
		return nullptr;
	}
}