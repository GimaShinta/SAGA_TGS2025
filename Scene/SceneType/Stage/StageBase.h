#pragma once

#include "../../../Object/Character/Player/Player.h"
#include "../../../Utility/ProjectConfig.h"

// StageBase.h
enum class StageID {
    Stage1,
    Stage2,
    Stage3,
    Stage4,
    Unknown
};

class StageBase {
protected:
    bool is_clear = false;
    bool is_over = false;
    int se;


public:
    StageBase(Player* player) : player(player) {}
    virtual ~StageBase() {}
    virtual void Initialize() = 0;
    virtual void Update(float delta) = 0;
    virtual void Draw() = 0;
    virtual void Finalize() = 0;
    virtual bool IsFinished() = 0;
    virtual void SetFinished() = 0;
    virtual bool IsClear() = 0;
    virtual bool IsOver() = 0;
    virtual StageBase* GetNextStage(Player* player) = 0;
    virtual StageBase* GetNowStage() = 0;

protected:
    StageID stage_id;
public:
    StageID GetStageID() const { return stage_id; }

protected:
    class Player* player; // ���L����v���C���[���

protected:
    float bg_scroll_speed_layer1 = 100.0f;  // �w�ʂ̃X�N���[�����x
    float bg_scroll_speed_layer2 = 200.0f;  // �O�ʂ̃X�N���[�����x
    float bg_scroll_offset_layer1 = 0.0f;
    float bg_scroll_offset_layer2 = 0.0f;
    bool is_scroll_enabled = true;

public:
    //�X�N���[����
    virtual void UpdateBackgroundScroll(float delta)
    {
        if (is_scroll_enabled) 
        {
            bg_scroll_offset_layer1 -= bg_scroll_speed_layer1 * delta;
            bg_scroll_offset_layer2 -= bg_scroll_speed_layer2 * delta;

            if (bg_scroll_offset_layer1 >= D_WIN_MAX_Y) bg_scroll_offset_layer1 -= D_WIN_MAX_Y;
            if (bg_scroll_offset_layer2 >= D_WIN_MAX_Y) bg_scroll_offset_layer2 -= D_WIN_MAX_Y;
        }
    }

    virtual void DrawScrollBackground() const = 0; // �e�X�e�[�W�ŋ�̎���

    // StageBase.h �� protected �����ɒǉ�
protected:
    // �t�F�[�h����
    bool is_fading_in = true;     // ������ԂŃt�F�[�h�C��
    bool is_fading_out = false;   // �N���A�^�I�[�o�[���Ƀt�F�[�h�A�E�g�J�n
    float fade_alpha = 255.0f;    // 0: ���S�\��, 255: ���S��

public:
    // ���t���[���Ăяo���t�F�[�h�X�V
    virtual void UpdateFade(float delta)
    {
        // �X�e�[�W�N���A�^�I�[�o�[�Ńt�F�[�h�A�E�g��
        if (!is_fading_out && (is_clear || is_over))
        {
            is_fading_out = true;
        }

        // �t�F�[�h�C�����Falpha��0
        if (is_fading_in)
        {
            fade_alpha -= 300.0f * delta;
            if (fade_alpha <= 0.0f)
            {
                fade_alpha = 0.0f;
                is_fading_in = false;
            }
        }
        // �t�F�[�h�A�E�g���Falpha��255
        else if (is_fading_out)
        {
            fade_alpha += 300.0f * delta;
            if (fade_alpha >= 255.0f)
            {
                fade_alpha = 255.0f;
            }
        }
    }

    // �`��̍Ō�ɌĂԃt�F�[�h�`��
    virtual void DrawFadeOverlay() const
    {
        if (is_fading_in || is_fading_out)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(fade_alpha));
            DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }


};