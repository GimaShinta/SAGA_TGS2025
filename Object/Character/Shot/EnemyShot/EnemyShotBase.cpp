#include "EnemyShotBase.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"

EnemyShotBase::EnemyShotBase()
{
}

EnemyShotBase::~EnemyShotBase()
{
}

// ����������
void EnemyShotBase::Initialize()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void EnemyShotBase::Update(float delta_second)
{
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void EnemyShotBase::Draw(const Vector2D& screen_offset) const
{
	// �e�̉�]�p�i�i�s�����ɂ���ĉ�]���x�ς��Ă�OK�j
	float angle = move_time * 10.0f; // ���߂ɉ�]�����Ēe���ۂ�
	float hw = box_size.x;
	float hh = box_size.y;

	float cosA = cos(angle);
	float sinA = sin(angle);

	Vector2D p1(-hw, -hh);
	Vector2D p2(hw, -hh);
	Vector2D p3(hw, hh);
	Vector2D p4(-hw, hh);

	auto rotate = [&](Vector2D p) {
		return Vector2D(
			location.x + p.x * cosA - p.y * sinA,
			location.y + p.x * sinA + p.y * cosA
		);
		};

	p1 = rotate(p1);
	p2 = rotate(p2);
	p3 = rotate(p3);
	p4 = rotate(p4);

	int color = GetColor(0, 255, 255);
	DrawTriangle(p1.x, p2.y, p2.x, p2.y, p3.x, p3.y, color, TRUE);
	DrawTriangle(p1.x, p1.y, p3.x, p3.y, p4.x, p4.y, color, TRUE);
}

// �I��������
void EnemyShotBase::Finalize()
{
}

/// <summary>
/// �q�b�g������
/// </summary>
/// <param name="hit_object">������������</param>
void EnemyShotBase::OnHitCollision(GameObjectBase* hit_object)
{
	// �����������肪�e��������
	if (hit_object->GetCollision().object_type == eObjectType::ePlayer)
	{
		is_destroy = true;
	}
}
