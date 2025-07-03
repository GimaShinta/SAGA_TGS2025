#pragma once
#include "../SceneBase.h"
#include <vector>

class CreditScene : public SceneBase
{
public:
    CreditScene();
    virtual ~CreditScene();

    virtual void Initialize() override;
    virtual eSceneType Update(float delta) override;
    virtual void Draw() override;
    virtual void Finalize() override;

private:
    float scrollY;
    float scrollSpeed;
    std::vector<const char*> creditLines;
    int font_credits;

    // ���݂̃V�[���^�C�v�i�I�[�o�[���C�h�K�{�j
    virtual eSceneType GetNowSceneType() const override;
};
