#pragma once
#include "../ItemBase.h"
#include "../../../Utility/ProjectConfig.h"

class Exp : public ItemBase
{
private:
    std::vector<int> images;

    float attract_range;   //�Ǐ]��ON�ɂ��鋗��
    float speed;           // �����X�s�[�h
    float max_speed;       // �ő�X�s�[�h
    float acceleration;    // �����x�i1�b������̑����ʁj
    float lifetime;        //5�b�Ŏ����폜

    bool is_attracting;    //�������Ă��邩
    int se;

public:
    Exp();
    ~Exp();

    /// <summary>
    /// ����������
    /// </summary>
    void Initialize() override; 

    /// <summary>
    /// �X�V����
    /// </summary>
    /// <param name="delta"></param>
    void Update(float delta) override;

    /// <summary>
    /// �`�揈��
    /// </summary>
    /// <param name="offset"></param>
    void Draw(const Vector2D& offset) const override;

    /// <summary>
    /// �����蔻�菈��
    /// </summary>
    /// <param name="�q�b�g�I�u�W�F�N�g"></param>
    void OnHitCollision(GameObjectBase* hit_object) override;

};
