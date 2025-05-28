#include "../EnemyBeam1.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"
#include "../../Enemy/EnemyType/Boss2.h"

EnemyBeam1::EnemyBeam1()
{
}

EnemyBeam1::~EnemyBeam1()
{
}

// ����������
void EnemyBeam1::Initialize()
{
	z_layer = 4;
	box_size = Vector2D(48.0f, 800.0f);

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eEnemyBeam;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::ePlayer);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void EnemyBeam1::Update(float delta_second)
{
#if 0
    beam_time += delta_second;
    alpha_timer += delta_second;

    // �����̕ω��i�ׂ� �� �����j
    float max_thickness = 48.0f;
    float min_thickness = 8.0f;
    float thickness = min_thickness;

    if (beam_time < growth_duration)
    {
        float t = beam_time / growth_duration;
        thickness = min_thickness + (max_thickness - min_thickness) * t;
    }
    else
    {
        thickness = max_thickness;
    }

    box_size = Vector2D(thickness, box_size.y);

    // �r�[���̏I������
    if (beam_time >= 5.0f)
    {
        SetDestroy();
        beam_time = 0.0f;
    }

    location += velocity * delta_second;


#else

    beam_time += delta_second;

    // --- �����ω����� ----------------------------
    float max_thickness = 48.0f;         // �ő�̑���
    float min_thickness = 8.0f;          // �����ׂ̍�
    float growth_duration = 0.2f;        // ���b�ōő呾���ɂȂ邩

    float thickness = min_thickness;

    if (beam_time < growth_duration) {
        // ���`��Ԃŏ��X�ɑ�������
        float t = beam_time / growth_duration;
        thickness = min_thickness + (max_thickness - min_thickness) * t;
    }
    else {
        thickness = max_thickness;  // ��莞�Ԍ�ɍő呾���ɌŒ�
    }

    box_size = Vector2D(thickness, box_size.y);  // �����i�����j�����ς���
    // --------------------------------------------

    if (beam_time >= 5.0f)
    {
        SetDestroy(); // �����I�ɍ폜
        beam_time = 0.0f;
    }

    location += velocity * delta_second;
#endif
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void EnemyBeam1::Draw(const Vector2D& screen_offset) const
{
#if 0
    // �A���t�@�i�����x�j���T�C���g�ŕω� �� �_��
    float alpha = 255.0f;
    if (beam_time < growth_duration)
    {
        alpha = 128 + 127 * sinf(alpha_timer * 10.0f);  // �_�Ŏ�����10Hz
    }

    int r = 255;
    int g = 0;
    int b = 255;

    int color = GetColor(r, g, b);
    int alpha_i = static_cast<int>(alpha);

    // ���ߕt���`��iDxLib�j
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_i);

    DrawBox(
        static_cast<int>(location.x - box_size.x),
        static_cast<int>(location.y - box_size.y),
        static_cast<int>(location.x + box_size.x),
        static_cast<int>(location.y + box_size.y),
        color,
        TRUE
    );

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);  // ���ɖ߂�
#else
    //�����蔻��̉���
    DrawBox(location.x - box_size.x, location.y - box_size.y,
        location.x + box_size.x, location.y + box_size.y, GetColor(255, 0, 255), TRUE);
#endif

}

// �I��������
void EnemyBeam1::Finalize()
{
}

void EnemyBeam1::OnHitCollision(GameObjectBase* hit_object)
{
}

void EnemyBeam1::SetBoss2(Boss2* p_boss)
{
	boss2 = p_boss;
}
