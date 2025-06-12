#include "../EnemyBeam1.h"
#include "../../../GameObjectManager.h"
#include "../../../../Utility/ProjectConfig.h"
#include "../../Enemy/EnemyType/Boss2.h"
#include "../../Enemy/EnemyType/Boss3.h"
#include <algorithm>

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
    box_size = Vector2D(8.0f, 800.0f);  // �ŏ��ׂ͍��i�\�����p�j

    collision.is_blocking = false;  // �ŏ��͓����蔻��Ȃ�
    collision.object_type = eObjectType::eEnemyBeam;
    collision.hit_object_type.push_back(eObjectType::ePlayer);

    is_mobility = true;

    beam_time = 0.0f;
    state_timer = 0.0f;
    state = BeamState::Warning;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    beam_ts = rm->GetImages("Resource/Image/Object/Player/Beam/anime_sp_weapon03_1.png", 12, 2, 6, 88, 128);
    beam_t = beam_ts[10];
    beam_bs = rm->GetImages("Resource/Image/Object/Player/Beam/anime_sp_weapon03_2.png", 12, 2, 6, 88, 80);
    beam_b = beam_bs[10];
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

    state_timer += delta_second;

    switch (state)
    {
    case BeamState::Warning:
        if (state_timer >= warning_time)
        {
            state = BeamState::Firing;
            state_timer = 0.0f;
            beam_time = 0.0f;
            box_size.x = 8.0f;

            // �����蔻��L�����F�����Œǉ�
            collision.hit_object_type.clear();
            collision.hit_object_type.push_back(eObjectType::ePlayer);
        }
        else
        {
            // �����蔻�薳�����F�v���C���[�ɓ�����Ȃ�����
            collision.hit_object_type.clear();
        }
        break;

    case BeamState::Firing:
        beam_time += delta_second;

        // �����̕ω������i�����R�[�h�j
        float max_thickness = 48.0f;
        float min_thickness = 8.0f;
        float growth_duration = 0.2f;

        float thickness = (beam_time < growth_duration)
            ? min_thickness + (max_thickness - min_thickness) * (beam_time / growth_duration)
            : max_thickness;

        box_size.x = thickness;

        if (beam_time >= 5.0f)
        {
            SetDestroy();
        }

        location += velocity * delta_second;
        break;
    }
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
    if (state == BeamState::Firing)
    {
        float max_half_width = 48.0f;
        float min_half_width = 4.0f;
        float growth_duration = 0.2f;

        float t = (beam_time < growth_duration) ? beam_time / growth_duration : 1.0f;
        float half_width = min_half_width + (max_half_width - min_half_width) * t;

        float wave_amplitude = 2.0f;
        float wave_speed = 100.0f;
        float wave_offset = sinf(beam_time * wave_speed) * wave_amplitude;

        int y_offset = 80; // �� ���ꂪ���炷��

        int cx = static_cast<int>(location.x);
        int top_y = static_cast<int>(location.y - box_size.y + wave_offset + y_offset);
        int bottom_y = static_cast<int>(location.y + box_size.y + wave_offset + y_offset);

        // --- �{�̕`��ibeam_t�j ---
        DrawModiGraph(
            cx - half_width, top_y,
            cx + half_width, top_y,
            cx + half_width, bottom_y,
            cx - half_width, bottom_y,
            beam_t,
            TRUE
        );

        int beam_b_height = 80;
        int base_bottom_y = static_cast<int>(location.y - box_size.y + wave_offset + 80);
        int base_top_y = base_bottom_y - beam_b_height;

        DrawModiGraph(
            cx - half_width, base_bottom_y,  // �� ������ base_bottom_y ��
            cx + half_width, base_bottom_y,  // �� ������ base_bottom_y ��
            cx + half_width, base_top_y,     // �� ������ base_top_y ��
            cx - half_width, base_top_y,     // �� ������ base_top_y ��
            beam_b,
            TRUE
        );
    }
    else if (state == BeamState::Warning)
    {
        // �Ԃ��\�����i�������j
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
        DrawBox(
            static_cast<int>(location.x - box_size.x),
            static_cast<int>(location.y - box_size.y),
            static_cast<int>(location.x + box_size.x),
            static_cast<int>(location.y + box_size.y),
            GetColor(255, 0, 0),
            TRUE
        );
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
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
void EnemyBeam1::SetBoss3(Boss3* p_boss)
{
	boss3 = p_boss;
}
