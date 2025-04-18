#pragma once

// �V���O���g���e���v���[�g�N���X
template <typename T>
class Singleton
{
protected:

	// �N���X�̎��̂������o�֐����ł��������ł��Ȃ��悤�ɂ���
	Singleton() = default;

	// �R�s�[�K�[�h
	Singleton(const Singleton&) = delete;
	Singleton& operator = (const Singleton&) = delete;
	// ~�R�s�[�K�[�h

	~Singleton() = default;

public:
	// �C���X�^���X�̎擾
	static T* GetInstance();
};

// �C���X�^���X�̎擾
template<typename T>
inline T* Singleton<T>::GetInstance()
{
	// static �ϐ��Ƃ��ăC���X�^���X��ێ�
	// T* �ł͂Ȃ� T �^�̃C���X�^���X�𒼐ڕێ�����
	static T instance;
	// �A�h���X��Ԃ�
	return &instance;
}