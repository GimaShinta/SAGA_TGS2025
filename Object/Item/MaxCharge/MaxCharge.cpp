#include "MaxCharge.h"
#include "../../../Utility/ScoreData.h"

void MaxCharge::Initialize()
{
    velocity.y = 60.0f;
    box_size = 20.0f;
    z_layer = 1;
    is_mobility = true;

    // �����蔻��̃I�u�W�F�N�g�ݒ�
    collision.is_blocking = true;
    // �����̃I�u�W�F�N�g�^�C�v
    collision.object_type = eObjectType::eMaxCharge;
    // �����鑊��̃I�u�W�F�N�g�^�C�v
    collision.hit_object_type.push_back(eObjectType::ePlayer);

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images = rm->GetImages("Resource/Image/Object/Item/PowerUp/max.png", 48, 8, 6, 48, 64);
    image = images[0];

    se = rm->GetSounds("Resource/sound/se/effect/audiostock_1133382.mp3");
    ChangeVolumeSoundMem(255 * 100 / 100, se);
}

void MaxCharge::Update(float delta)
{
    std::vector<int> animation_num = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };
    //�t���[�����[�g�Ŏ��Ԃ��v��
    animation_time += delta;
    //8�b�o������摜��؂�ւ���
    if (animation_time >= 0.03f)
    {
        //�v�����Ԃ̏�����
        animation_time = 0.0f;
        //���Ԍo�߃J�E���g�̑���
        animation_count++;
        //�J�E���g���A�j���[�V�����摜�̗v�f���ȏ�ɂȂ�����
        if (animation_count >= animation_num.size())
        {
            //�J�E���g�̏�����
            animation_count = 0;
        }
        // �A�j���[�V���������Ԃɑ�������
        image = images[animation_num[animation_count]];
    }


    if (!player)
    {
        return; // �v���C���[�����ݒ�Ȃ珈�����Ȃ�
    }

    //�v���C���[���F�ʒu���
    Vector2D to_player = player->GetLocation() - location;

    // ������2��ŋz���J�n�𔻒�
    float dist_sq = to_player.x * to_player.x + to_player.y * to_player.y;
    float attract_range_sq = attract_range * attract_range;

    if (!is_attracting && dist_sq < attract_range_sq)
    {
        // �z���J�n
        is_attracting = true;
    }

    if (is_attracting)
    {
        // �z�����[�h���F�������ăv���C���[�Ɍ�����
        to_player.Normalize();

        speed += acceleration * delta;   // ���X�ɉ���

        if (speed > max_speed)
        {
            speed = max_speed; // �ő�X�s�[�h����
        }
        velocity = to_player * speed;
    }

    // ���݂̑��x�ňʒu�X�V
    location += velocity * delta;

}

void MaxCharge::Draw(const Vector2D& offset) const
{
    DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, image, TRUE);

}

void MaxCharge::OnHitCollision(GameObjectBase* hit_object)
{
    if (hit_object->GetCollision().object_type == eObjectType::ePlayer)
    {
        PlaySoundMem(se, DX_PLAYTYPE_BACK);
        Singleton<ScoreData>::GetInstance()->AddScore(GetRand(100) + 50);
        this->SetDestroy();
    }

}
