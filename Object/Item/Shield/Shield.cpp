#include "Shield.h"
#include "../../../Utility/InputManager.h"

void Shield::Initialize()
{
    velocity.y = 60.0f;
    box_size = 10.0f;
    z_layer = 1;
    is_mobility = true;

    // �����蔻��̃I�u�W�F�N�g�ݒ�
    collision.is_blocking = true;
    // �����̃I�u�W�F�N�g�^�C�v
    collision.object_type = eObjectType::eShield;
    // �����鑊��̃I�u�W�F�N�g�^�C�v
    collision.hit_object_type.push_back(eObjectType::ePlayer);

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    images = rm->GetImages("Resource/Image/Object/Item/PowerUp/anime_item32.png", 12, 6, 2, 32, 32);
    image = images[0];

    se = rm->GetSounds("Resource/sound/se/effect/audiostock_1090663.mp3");
    ChangeVolumeSoundMem(255 * 70 / 100, se);
}

void Shield::Update(float delta)
{
    InputManager* input = Singleton<InputManager>::GetInstance();

    std::vector<int> animation_num = { 6, 7, 8, 9, 10, 11 };
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

    location += velocity * delta;
}

void Shield::Draw(const Vector2D& offset) const
{
    //DrawBox(location.x - box_size.x, location.y - box_size.y,
    //    location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);

    DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, image, TRUE);

}

void Shield::OnHitCollision(GameObjectBase* hit_object)
{
    if (hit_object->GetCollision().object_type == eObjectType::ePlayer)
    {
        this->SetDestroy();
    }
}
