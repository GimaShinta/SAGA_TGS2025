#pragma once
#include "../GameObjectBase.h"
#include"../../Object/Character/Player/Player.h"

class ItemBase : public GameObjectBase
{
protected:
	Player* player;	//���L���ꂽ�v���C���[���

	float attract_range;   //�Ǐ]��ON�ɂ��鋗��
	bool is_attracting;    //�������Ă��邩

	float speed;           // �����X�s�[�h
	float max_speed;       // �ő�X�s�[�h
	float acceleration;    // �����x�i1�b������̑����ʁj

public:
	// ����������
	virtual void Initialize();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta_second">�P�t���[��������̎���</param>
	/// <returns></returns>
	virtual void Update(float delata_second);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="delta_second"></param>
	/// <returns></returns>
	virtual void Draw(const Vector2D& screen_offset)const;

	// �I��������
	virtual void Finalize();

public:
	/// <summary>
	/// �q�b�g�����Ƃ��̏���
	/// </summary>
	/// <param name="hit_object">�v���C���[���Ƃ�����A�����͓G�Ƃ��u���b�N�Ɠ����������ƂɂȂ�</param>
	virtual void OnHitCollision(GameObjectBase* hit_object);

	/// <summary>
	/// �v���C���[�������L
	/// </summary>
	/// <param name="p">�v���C���[���</param>
	void SetPlayer(Player* p);
};

