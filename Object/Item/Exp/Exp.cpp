#include "Exp.h"
#include "DxLib.h"
#include "../../Character/Player/Player.h"
#include "../../GameObjectManager.h"


Exp::Exp() : 
    attract_range(250.0f),  //�Ǐ]��ON�ɂ��鋗��
    speed(1.0f),            //�����X�s�[�h
    max_speed(200.0f),      //�ő�X�s�[�h
    acceleration(100.0f),   //�����x(��b������̑�����)
    lifetime(5.0f),         //�����폜����b��
    is_attracting(false)    //�������Ă��邩
{
}

Exp::~Exp()
{
}

// ����������
void Exp::Initialize()
{
    // �����蔻��ݒ�
    collision.is_blocking = true;                         // �ڐG����L��
    collision.object_type = eObjectType::eExp;            // �����̎�ނ�Exp
    collision.hit_object_type.push_back(eObjectType::ePlayer); // �v���C���[�ƏՓ˔���

    box_size = 6.0f;             // �o���l�A�C�e���̃T�C�Y�i���a�j
    z_layer = 1;                 // �`�惌�C���[�i�O�ʂɕ\���j

    is_mobility = true;          // �����蔻��ňړ�����L��
    is_attracting = false;       // �z�����[�hOFF�ŊJ�n
    velocity = Vector2D(0, 50.0f); // ���������i�ӂ���Ɖ��ցj

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();

    se = rm->GetSounds("Resource/sound/se/effect/audiostock_890909.mp3");
    ChangeVolumeSoundMem(255 * 30 / 100, se);

    images = rm->GetImages("Resource/Image/Effect/pipo-gate01c.png", 15, 5, 3, 480, 480);
    image = images[0];


}

// �X�V����
void Exp::Update(float delta)
{
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

    // ���������i�������ŏ����j
    lifetime -= delta;
    if (lifetime <= 0.0f)
    {
        this->SetDestroy(); // 5�b��ɍ폜
        return;
    }

    // ��ʊO�ɗ�������폜
    if (location.y > D_WIN_MAX_Y + 200)
    {
        this->SetDestroy();
        return;
    }

    std::vector<int> animation_num = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
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

}

// �`�揈��
void Exp::Draw(const Vector2D& offset) const
{
    DrawRotaGraph(location.x, location.y, 0.07f, 0.0f, image, TRUE);


    //// �O���F���F�i�O���[���j
    //DrawCircle(location.x, location.y, (int)box_size.x, GetColor(0, 255, 255), TRUE);

    //// �����F���F
    //DrawCircle(location.x, location.y, (int)box_size.x - 2.0f, GetColor(255, 255, 0), TRUE);
}

// �Փˏ���
void Exp::OnHitCollision(GameObjectBase* hit_object)
{
    // �v���C���[�Ɠ���������
    if (hit_object->GetCollision().object_type == eObjectType::ePlayer)
    {

        PlaySoundMem(se, DX_PLAYTYPE_BACK);
        if (player && !player->GetBeamOn())  // �r�[�����łȂ���Ή��Z
        {
            player->AddCharge(1.0f);  // �����\
        }
        this->SetDestroy(); // �z�� �� �폜
    }
}
