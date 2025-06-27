#include "GameMainScene.h"
#include "../../Scene/SceneType/Stage/StageType/Stage1.h"
#include "../../Scene/SceneType/Stage/StageType/Stage2.h"
#include <algorithm>   // std::min ���g�����߂ɕK�v
#include "Stage/StageType/Stage3.h"
#include "Stage/StageType/Stage4.h"



GameMainScene::GameMainScene() : player(nullptr), current_stage(nullptr), next_scene_timer(0.0f)
{
}

GameMainScene::~GameMainScene()
{
}

// ����������
void GameMainScene::Initialize()
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    player = objm->CreateObject<Player>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) + 220.0f));

    AnimationManager* anim = Singleton<AnimationManager>::GetInstance();
    //anim->LoadAllEffects();

    current_stage = new Stage1(player);

    current_stage->Initialize();

    // BGM�ǂݍ��݁i����̂݁j
    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    stage_bgm1 = rm->GetSounds("Resource/sound/bgm/stage/BGM_2.mp3");
    stage_bgm3 = rm->GetSounds("Resource/sound/bgm/stage/Cybernetic.mp3");

    stage_bgm4 = rm->GetSounds("Resource/sound/bgm/stage/Last_Boss.mp3"); // �C�ӂ̃t�@�C��
    se_warning = rm->GetSounds("Resource/sound/se/battle/Warning.mp3");       // �x����SE

    se_charge = rm->GetSounds("Resource/sound/se/effect/audiostock_1133382.mp3");
    ChangeVolumeSoundMem(255 * 100 / 100, se_charge);

    //�T�C�h�p�l���摜
    obi_handle = rm->GetImages("Resource/Image/BackGround/Main/Obi_1.png")[0];

    //�t�H���g
   // font_digital = CreateFontToHandle("DS-Digital", 28, 6, DX_FONTTYPE_ANTIALIASING);
    font_digital = CreateFontToHandle("Orbitron", 28, 6, DX_FONTTYPE_ANTIALIASING);
    font_orbitron = CreateFontToHandle("Orbitron", 22, 6, DX_FONTTYPE_ANTIALIASING);
    //font_orbitron = CreateFontToHandle("DS-Digital", 28, 6, DX_FONTTYPE_ANTIALIASING);
    font_warning = CreateFontToHandle("Orbitron", 48, 6, DX_FONTTYPE_ANTIALIASING);

    m_menuFontHandle = CreateFontToHandle("Orbitron", 36, 6); // ���j���[��p�t�H���g


    // �X�e�[�W1�pBGM���Đ�
    current_bgm_handle = stage_bgm1;

    ChangeVolumeSoundMem(255 * 50 / 100, current_bgm_handle); 
    ChangeVolumeSoundMem(255 * 50 / 100, stage_bgm1);

    PlaySoundMem(current_bgm_handle, DX_PLAYTYPE_LOOP);

    // =====================
    // �v�����[�h�����i�x���h�~�j
    // =====================


    //AnimationManager �ɂ�鎖�O�A�j���Đ��i0.01�b�A��ʊO�ň�x�����Đ��j

    anim->LoadAllEffects(); // �������Ŗ����I�ɉ摜���ǂ݂�����I
    anim->LoadSE();


   // anim->LoadAllEffects(); // �������Ŗ����I�ɉ摜���ǂ݂�����I


    // ���ɃG�t�F�N�g2��o�^�ς݂Ȃ�
    //anim->PlayerAnimation(EffectName::eExprotion, Vector2D(-999, -999), 0.01f, false);
    //anim->PlayerAnimation(EffectName::eExprotion2, Vector2D(-999, -999), 0.01f, false);


    // �� �t�H���g�`��i���񂾂��B��ĕ`�悵�ăt�H���g�W�J���ς܂���j
    DrawStringToHandle(-999, -999, "PRELOAD", GetColor(0, 0, 0), font_digital);
    DrawStringToHandle(-999, -999, "PRELOAD", GetColor(0, 0, 0), font_orbitron);
    DrawStringToHandle(-999, -999, "PRELOAD", GetColor(0, 0, 0), m_menuFontHandle);

    // �� �V���b�g���^�q�b�g����ǂݍ��݁��_�~�[�Đ��Ńo�b�t�@�W�J
    int preload_se1 = rm->GetSounds("Resource/sound/se/shot/shot_02.mp3");
    int preload_se2 = rm->GetSounds("Resource/sound/se/se_effect/reaction.mp3");

    PlaySoundMem(preload_se1, DX_PLAYTYPE_BACK, TRUE);
    StopSoundMem(preload_se1);
    PlaySoundMem(preload_se2, DX_PLAYTYPE_BACK, TRUE);
    StopSoundMem(preload_se2);

    // �� �`���[�WSE���o�b�t�@��
    PlaySoundMem(se_charge, DX_PLAYTYPE_BACK, TRUE);
    StopSoundMem(se_charge);

    // === �G�p�摜�i�z��t�@�C�����j���v�����[�h ===
    int preload_img1 = LoadGraph("Resource/Image/Enemy/enemy1.png");
    int preload_img2 = LoadGraph("Resource/Image/Enemy/enemy2.png");

    // �K�v�ɉ����ĕ���
    DrawGraph(-999, -999, preload_img1, FALSE);
    DrawGraph(-999, -999, preload_img2, FALSE);

    // === �G�p���ʉ��̃o�b�t�@�m�� ===
    int preload_hit = LoadSoundMem("Resource/sound/se/se_effect/reaction.mp3");
    PlaySoundMem(preload_hit, DX_PLAYTYPE_BACK, TRUE);
    StopSoundMem(preload_hit);

    // === �G���g���A�j���[�V��������ʊO�Ń_�~�[���� ===
    //anim->PlayerAnimation(EffectName::eExprotion, Vector2D(-999, -999), 0.01f, false);
    //anim->PlayerAnimation(EffectName::eExprotion2, Vector2D(-999, -999), 0.01f, false);


}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
/// <returns></returns>
eSceneType GameMainScene::Update(float delta_second)
{
    InputManager* input = Singleton<InputManager>::GetInstance();
#if _DEBUG
    InputSePlay();
#endif
    if (input->GetButtonDown(XINPUT_BUTTON_START) ||
        input->GetKeyDown(KEY_INPUT_P)) {
        isPaused = !isPaused;
        m_selectedIndex = 0;
    }

    if (!isPaused) 
    {
        black_in_timer += delta_second;
        line_effect_timer += delta_second;
        transparent = 0;
        // �|�[�Y���͉����X�V���Ȃ��i�܂��̓|�[�Y��ʂ̍X�V�̂݁j
        if (current_stage)
        {
            AnimationManager* anim = Singleton<AnimationManager>::GetInstance();
            anim->Update(delta_second);

            current_stage->Update(delta_second);
            if (current_stage && current_stage->GetStageID() == StageID::Stage3)
            {
                Stage3* stage3 = dynamic_cast<Stage3*>(current_stage);
                if (stage3 != nullptr)
                {
                    if (stage3->request_stop_bgm)
                    {
                        StopSoundMem(stage_bgm3);
                        stage3->request_stop_bgm = false;
                    }
                     // BGM��~���N�G�X�g�i�t�F�[�h��Ɏ~�߂�j
                    //StopSoundMem(stage_bgm3);
                    //if (stage3->request_play_warning_se)
                    //{
                    //    PlaySoundMem(se_warning, DX_PLAYTYPE_BACK);
                    //    stage3->request_play_warning_se = false;
                    //}
                }
            }

            if (current_stage->GetStageID() == StageID::Stage4)
            {

                if (warning_state == WarningState::None && is_none == false)
                {
                    // �����ɉ����āi��F��莞�Ԍo�ߌ�Ȃǁj
                    if (black_in_timer2 >= 2.0f)
                    {
                        is_none = true;
                        warning_state = WarningState::Expanding;
                        warning_timer = 0.0f;
                        band_half_height = 0.0f; // �O�̂��ߏ�����
                        warning_text_x = 1020;   // �����ʒu�������i�E�[���痬���ꍇ�j
                    }

                }


                black_in_timer2 += delta_second;

                warning_timer += delta_second;

                switch (warning_state)
                {
                case WarningState::Expanding:
                    band_half_height += band_expand_speed * delta_second;
                    if (band_half_height >= band_max_half_height)
                    {
                        band_half_height = band_max_half_height;
                        warning_state = WarningState::Displaying;
                        warning_timer = 0.0f;

                        // �x��SE�Đ�
                        PlaySoundMem(se_warning, DX_PLAYTYPE_BACK);

                       
                    }
                    break;

                case WarningState::Displaying:
                    warning_text_x -= warning_scroll_speed * delta_second;
                    warning_timer += delta_second;

                    if (warning_text_x < -200)
                        warning_text_x = 1020;

                    if (warning_timer >= warning_duration)
                    {
                        warning_state = WarningState::Shrinking;
                    }
                    break;

                case WarningState::Shrinking:
                    band_half_height -= band_expand_speed * delta_second;
                    if (band_half_height <= 0.0f)
                    {
                        band_half_height = 0.0f;
                        warning_state = WarningState::None;  
                    }
                    // �X�e�[�W4��BGM�Đ�
                    PlaySoundMem(stage_bgm4, DX_PLAYTYPE_LOOP);
                    break;

                default:
                    break;
                }


            }



            if (current_stage->IsFinished())
            {
                if (current_stage->IsClear() == true)
                {        
                    if (current_stage->GetStageID() == StageID::Stage3)
                    {
                          black_fade_timer += delta_second;
                                if (alpha >= 255)
                                {
                                    StopSoundMem(stage_bgm3); // �� �X�e�[�W3��BGM�𖾎��I�ɒ�~

                                    StageBase* next_stage = current_stage->GetNextStage(player);

                                    current_stage->Finalize();
                                    delete current_stage;
                                    current_stage = nullptr;

                                    if (next_stage != nullptr)
                                    {
                                        current_stage = next_stage;
                                        current_stage->Initialize();

                                        // �X�e�[�W4�p��BGM���Đ��i�K�v�Ȃ� stage_bgm4 ���`�j
                                        current_bgm_handle = stage_bgm4;  // �܂��� stage_bgm4
                                        ChangeVolumeSoundMem(255 * 90 / 100, current_bgm_handle);
                                      //  PlaySoundMem(current_bgm_handle, DX_PLAYTYPE_LOOP);
                                    }
                                    else
                                    {
                                        return eSceneType::eTitle;
                                    }
                                }

                     }
                        
                    

                    else
                    {
                        StageBase* next_stage = current_stage->GetNextStage(player);

                        current_stage->Finalize();
                        delete current_stage;
                        current_stage = nullptr;

                        if (next_stage != nullptr)
                        {
                            // === �X�e�[�W�̐ؑւ�BGM���� ===
                            current_stage = next_stage;
                            current_stage->Initialize();

                            // �X�e�[�W3�ɓ��B�����ꍇ�̂�BGM�ؑ�
                            if (dynamic_cast<Stage3*>(current_stage) != nullptr)
                            {
                                StopSoundMem(current_bgm_handle); // ���݂�BGM���~
                                current_bgm_handle = stage_bgm3;  // �X�e�[�W3�pBGM�ɐ؂�ւ�
                                ChangeVolumeSoundMem(255 * 90 / 100, current_bgm_handle);
                                PlaySoundMem(current_bgm_handle, DX_PLAYTYPE_LOOP);
                            }
                        }
                        else
                        {
                            return eSceneType::eTitle;
                        }

                    }


                }
                else if (current_stage->IsOver() == true)
                {
                    current_stage->Finalize();
                    delete current_stage;
                    current_stage = nullptr;

                    return eSceneType::eTitle;
                }
            }

        }
    }
    else
    {
        pause_timer += delta_second;

        if (pause_timer >= 0.01f)
        {
            transparent++;
            pause_timer = 0.0f;
        }

        if (transparent >= 100)
        {
            transparent = 100;

        }

        InputManager* input = Singleton<InputManager>::GetInstance();

        // �㉺�L�[ or �R���g���[���\���őI��
        if (input->GetKeyDown(KEY_INPUT_UP) || input->GetKeyDown(KEY_INPUT_W) ||
            input->GetButtonDown(XINPUT_BUTTON_DPAD_UP))
        {
            m_selectedIndex = (m_selectedIndex + 1) % 2;
        }
        if (input->GetKeyDown(KEY_INPUT_DOWN) || input->GetKeyDown(KEY_INPUT_S) ||
            input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN))
        {
            m_selectedIndex = (m_selectedIndex + 1) % 2;
        }

        // ����i�X�y�[�X or A�{�^���j
        if (!m_startTransitioning && (input->GetKeyDown(KEY_INPUT_SPACE) || input->GetButtonDown(XINPUT_BUTTON_A)))
        {
            if (m_selectedIndex == 0) {
                isPaused = false;
            }
            else if (m_selectedIndex == 1) {
                // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
                GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
                objm->Finalize();

                return eSceneType::eTitle;
            }
        }



    }


    // ======= �X�R�A���O�̃X���C�h���o�X�V =======
    for (auto& log : score_logs)
    {
        if (log.y_offset < 0)
        {
            log.y_offset += delta_second * 40;  // ���ɃX���C�h�i40px/sec�j
            if (log.y_offset > 0)
                log.y_offset = 0; // �I�[�o�[�␳
        }
    }

    // ======= �V�����X�R�A�̒ǉ����o�ƃ��O���� =======
    ScoreData* score = Singleton<ScoreData>::GetInstance();
    const auto& all_scores = score->GetScoreData();

    while (previous_score_count < all_scores.size())
    {
        float new_score = all_scores[static_cast<size_t>(previous_score_count)];

        // �X�R�A���O�p���b�Z�[�W�쐬
        char buf[64];
        sprintf_s(buf, sizeof(buf), "Enemy +%.0f", new_score);

        // �� ��ɍő�s���`�F�b�N�i10�s��������Â��̂���폜�j
        if (score_logs.size() >= 10)
        {
            score_logs.erase(score_logs.begin()); // ��ԌÂ����O���폜
        }

        // �������O�����ɉ����o��
        for (auto& log : score_logs)
        {
            log.y_offset = -20.0f; // �X���C�h�J�n�ʒu
        }

        // �V�������O��ǉ��i�X���C�h�Ȃ��j
        ScoreLog new_log;
        new_log.text = buf;
        new_log.y_offset = 0.0f;
        score_logs.push_back(new_log);

        previous_score_count += 1.0f;
    }

    // �V�[���hON���o
    static bool prev_shield = false;
    bool now_shield = player->GetShieldOn();
    if (!prev_shield && now_shield) {
        effect_shield_on = true;
        effect_timer = 0.0f;
    }
    if (prev_shield && !now_shield) {
        effect_shield_off = true;
        effect_timer = 0.0f;
    }
    prev_shield = now_shield;

    // �p���[�A�b�v���o�i1�t���[���̂݁j
    static int prev_power = 1;
    int now_power = player->GetPowerd();
    if (now_power > prev_power) {
        effect_powerup = true;
        effect_timer = 0.0f;
    }
    prev_power = now_power;

    // �^�C�}�[�i�s
    effect_timer += delta_second;
    if (effect_timer > effect_duration) {
        effect_shield_on = effect_shield_off = effect_powerup = false;
    }


	return GetNowSceneType();
}

/// <summary>
/// �`�揈��
/// </summary>
/// <returns></returns>
void GameMainScene::Draw()
{
    if (current_stage)
    {
        AnimationManager* anim = Singleton<AnimationManager>::GetInstance();
        anim->Draw();
        current_stage->Draw();
        DrawUI();

        if (current_stage->GetStageID() == StageID::Stage3)
        {
            if (current_stage->IsClear())
            {
                if (black_fade_timer >= 0.01f)
                {
                    black_fade_timer = 0.0f;
                    alpha+=2;
                }
                //int alpha = static_cast<int>((black_fade_timer / black_fade_duration) * 255.0f);
                if (alpha > 255) alpha = 255;

                SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
                DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

                if (alpha >= 255)
                {
                    current_stage->SetFinished();

                    // �Ó]���� �� ���̃V�[���֐؂�ւ��Ȃ�
                    // SceneManager::ChangeScene(...);
                }
            }
        }
        else if (current_stage->GetStageID() == StageID::Stage4)
        {

            if (black_in_timer2 >= 8.0f)
            {
                if (black_in_timer >= 0.01f)
                {
                    black_in_timer = 0.0f;
                    alpha--;
                }
            }

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE); // �� �w�i���ŏ��ɕ`��
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // �� �����ǉ��I�i�����d�v�I�j ?

            if (warning_state != WarningState::None)
            {
                int y_top = static_cast<int>(band_center_y - band_half_height);
                int y_bottom = static_cast<int>(band_center_y + band_half_height);

                if (band_half_height > 1.0f)
                {
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
                    DrawBox(0, y_top, 1280, y_bottom, GetColor(255, 0, 0), TRUE);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                    DrawLine(0, y_top, 1280, y_top, GetColor(255, 255, 255));
                    DrawLine(0, y_bottom, 1280, y_bottom, GetColor(255, 255, 255));
                }

                if (warning_state == WarningState::Displaying)
                {
                    DrawStringToHandle(static_cast<int>(warning_text_x), band_center_y - 20,
                        "!! WARNING !!", GetColor(255, 255, 255), font_warning);
                }
            }



            //alpha--;

            if (alpha < 0)
            {
                alpha = 0;
                black_in_timer2 = 0.0f;
            }
            else
            {

            }


        }

        if (isPaused)
        {
            const int cx = D_WIN_MAX_X / 2;
            const int cy = D_WIN_MAX_Y / 2 - 20;

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
            DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
            //DrawBox(400, 200, 880, D_WIN_MAX_Y - 200, GetColor(255, 255, 255), FALSE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


            const char* menuItems[] = {
                "START GAME",
                "BACK TITLE",
            };

            for (int i = 0; i < 2; ++i)
            {
                int y = 300 + i * 50;
                int textWidth = GetDrawStringWidthToHandle(menuItems[i], strlen(menuItems[i]), m_menuFontHandle);
                int x = (D_WIN_MAX_X - textWidth) / 2;

                if (i == m_selectedIndex)
                {
                    // =========================
                    // �w�i�n�C���C�g�o�[�i��ʒ[�܂Łj
                    // =========================
                    int barHeight = 40;
                    int barAlpha = 120 + (int)(sinf(GetNowCount() / 60.0f) * 50); // �p���X
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, barAlpha);
                    DrawBox(
                        0, y - 5,
                        D_WIN_MAX_X, y + barHeight,
                        GetColor(0, 200, 255), TRUE
                    );

                    // =========================
                    // ���[�̃G�b�W�G�t�F�N�g�i�������j
                    // =========================
                    int edgeWidth = 80;
                    int edgeHeight = 4;
                    int scrollSpeed = 2;
                    int edgeOffset = (GetNowCount() * scrollSpeed) % (D_WIN_MAX_X + edgeWidth * 2);

                    // ������E�֗����i2�\�����ă��[�v�����o���j
                    for (int j = 0; j < 2; ++j)
                    {
                        int edgeX = edgeOffset - edgeWidth * j;
                        SetDrawBlendMode(DX_BLENDMODE_ADD, 50);
                        DrawBox(
                            edgeX, y + 10,
                            edgeX + edgeWidth, y + 10 + edgeHeight,
                            GetColor(200, 255, 255), TRUE
                        );
                        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                    }

                    // =========================
                    // �e�L�X�g�i�A�E�g���C���{�O���[�j
                    // =========================
                    int offsetX = (rand() % 3) - 1;
                    int offsetY = (rand() % 3) - 1;

                    DrawStringToHandle(x + offsetX - 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
                    DrawStringToHandle(x + offsetX + 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
                    DrawStringToHandle(x + offsetX, y + offsetY - 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
                    DrawStringToHandle(x + offsetX, y + offsetY + 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);

                    int glow = (int)((sinf(GetNowCount() / 30.0f) + 1.0f) * 127);
                    DrawStringToHandle(x + offsetX, y + offsetY, menuItems[i], GetColor(100 + glow, 255, 255), m_menuFontHandle);
                }
                else
                {
                    DrawStringToHandle(x, y, menuItems[i], GetColor(180, 180, 180), m_menuFontHandle);
                }
            }


        }


    }
    DrawFormatString(D_WIN_MAX_X / 2, 0, GetColor(255, 255, 255), "Stage%d", current_stage->GetStageID());

}


// �I���������i�g�����C���X�^���X�̍폜�Ƃ��j
void GameMainScene::Finalize()
{
    if (current_stage) 
    {
        current_stage->Finalize();
        delete current_stage;
        current_stage = nullptr;
    }
    if (current_bgm_handle != -1)
    {
        StopSoundMem(current_bgm_handle);
        current_bgm_handle = -1;
    }
    if (font_digital != -1) {
        DeleteFontToHandle(font_digital);
        font_digital = -1;
    }
    if (font_orbitron != -1) {
        DeleteFontToHandle(font_orbitron);
        font_orbitron = -1;
    }



}

/// <summary>
/// ���݂̃V�[�����
/// </summary>
/// <returns>���݂̓��U���g�V�[���ł�</returns>
eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::eGameMain;
}


void GameMainScene::AddScoreLog(const std::string& text)
{
    // �V�������O����ɒǉ��i�X���C�h�����j
    ScoreLog log;
    log.text = text;
    log.y_offset = 0.0f;
    score_logs.push_back(log);

    // �ő�10�s�ێ� �� �Â����̂���폜
    if (score_logs.size() >= 10)
    {
        score_logs.erase(score_logs.begin()); // �擪�i�Â��j���폜
    }

    // �������O�̓X���C�h�J�n�i���ɉ����o�����j
    for (auto& log : score_logs)
    {
        log.y_offset = -20.0f;  // ���t���[���ŕ␳�����
    }


}

void GameMainScene::DrawUI()
{
    //// ���̍���
//DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

//// �E�̍���
//DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

// === ���̍��� ===
    DrawGraph(0, 0, obi_handle, TRUE);

    // === �E�̍��сi���E���]�j===
    DrawTurnGraph(D_WIN_MAX_X - 290, 0, obi_handle, TRUE);




    // === ���̃T�C�h�p�l���i�T�C�o�[���j ===
    {
        int left_x1 = 0;
        int left_x2 = (D_WIN_MAX_X / 2) - 350;
        int panel_color = GetColor(10, 10, 30);
        int neon_color = GetColor(0, 255, 255);

        // �w�i
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(left_x1, 0, left_x2, D_WIN_MAX_Y, panel_color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

        // �㉺�l�I�����C��
        DrawLine(left_x1, 0, left_x2, 0, neon_color);
        DrawLine(left_x1, D_WIN_MAX_Y - 1, left_x2, D_WIN_MAX_Y - 1, neon_color);



        const float scanline_speed = 60.0f;  // ��F1�b��60px���ֈړ�
        int scan_y = static_cast<int>(line_effect_timer * scanline_speed) % D_WIN_MAX_Y;
        DrawLine(left_x1, scan_y, left_x2, scan_y, GetColor(0, 150, 255));


        //// �X�L�������C�����A�j��
        //int scan_y = (GetNowCount() / 5) % D_WIN_MAX_Y;
        //DrawLine(left_x1, scan_y, left_x2, scan_y, GetColor(0, 150, 255));
    }

    // === �E�̃T�C�h�p�l���i�T�C�o�[���j ===
    {
        int right_x1 = (D_WIN_MAX_X / 2) + 350;
        int right_x2 = D_WIN_MAX_X;
        int panel_color = GetColor(10, 10, 30);
        int neon_color = GetColor(0, 255, 255);

        // �w�i
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        DrawBox(right_x1, 0, right_x2, D_WIN_MAX_Y, panel_color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

        // �㉺�l�I�����C��
        DrawLine(right_x1, 0, right_x2, 0, neon_color);
        DrawLine(right_x1, D_WIN_MAX_Y - 1, right_x2, D_WIN_MAX_Y - 1, neon_color);


        const float scanline_speed = 60.0f;  // ��F1�b��60px���ֈړ�
        int scan_y = static_cast<int>(line_effect_timer * scanline_speed) % D_WIN_MAX_Y;
        DrawLine(right_x1, scan_y, right_x2, scan_y, GetColor(0, 150, 255));


        //// �X�L�������C�����A�j��
        //int scan_y = (GetNowCount() / 5) % D_WIN_MAX_Y;
        //DrawLine(right_x1, scan_y, right_x2, scan_y, GetColor(0, 150, 255));
    }

    // Draw() �̃T�C�h�p�l���`���ɒǉ�
    for (int y = 0; y < 720; y += 4)
    {
        int alpha = (y % 8 == 0) ? 40 : 20;
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawBox(0, y, 290, y + 2, GetColor(100, 255, 255), TRUE); // ���p�l��
        DrawBox(990, y, 1280, y + 2, GetColor(100, 255, 255), TRUE); // �E�p�l���i��j
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // DrawFormatString(D_WIN_MAX_X - 140, 0, GetColor(255, 255, 255), "Life:%d", player->life);

     // ==== ���v�X�R�A�i�L�����O�̏�j ====
    {
        ScoreData* score = Singleton<ScoreData>::GetInstance();
        const auto& all_scores = score->GetScoreData();

        float total_score = 0.0f;
        for (float s : all_scores)
        {
            total_score += s;
        }

        // �\���ʒu�ƃT�C�Y
        int score_box_x = 30;
        int score_box_y = D_WIN_MAX_Y - 640;
        int score_box_w = 220;
        int score_box_h = 32;

        // �_�Ńt���[���J���[
        int pulse = static_cast<int>(GetNowCount() % 100) > 50 ? 255 : 100;
        int frame_color = GetColor(pulse, 255, pulse);

        // �w�i�Ƙg
        //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
        //DrawBox(score_box_x, score_box_y, score_box_x + score_box_w, score_box_y + score_box_h, GetColor(10, 10, 30), TRUE);
        //SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        //DrawBox(score_box_x, score_box_y, score_box_x + score_box_w, score_box_y + score_box_h, frame_color, FALSE);

        // �X�R�A�e�L�X�g
        DrawFormatStringToHandle(score_box_x - 20, score_box_y + 8, frame_color, font_digital, "TOTAL SCORE");
        DrawFormatStringToHandle(score_box_x, score_box_y + 40, frame_color, font_digital, "%.0f", total_score);

        // �ʏ�`��ɖ߂��i�O�̂��߁j
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#if _DEBUG
        DrawFormatString(0, 0, GetColor(255, 255, 255), "se_volume : %d", vo);

#endif // _DEBUG

    }


    // ==== �X�R�A���O�i�����j ====
    int log_base_x = 30;
    int log_base_y = D_WIN_MAX_Y - 400;
    int line_height = 40;

    int count = static_cast<int>(score_logs.size());
    for (int i = 0; i < count; ++i)
    {
        const auto& log = score_logs[count - 1 - i];
        int draw_y = log_base_y + static_cast<int>(i * line_height + log.y_offset);

        // DrawBox(log_base_x - 10, draw_y - 2, log_base_x + 200, draw_y + 18, GetColor(10, 10, 30), TRUE);
        DrawLine(log_base_x - 10, draw_y - 2, log_base_x + 200, draw_y - 2, GetColor(0, 255, 255)); // �ド�C��
        DrawFormatStringToHandle(log_base_x, draw_y, GetColor(0, 255, 255), font_digital, "%s", log.text.c_str());

    }

    // ==== LIFE - STOCK �\���i�E��ADigital���{�c�@�A�C�R���j ====
    if (player)
    {
        int life_box_x = D_WIN_MAX_X - 230;
        int life_box_y = 80;
        int life_box_w = 200;
        int life_box_h = 60;

        // ���x��
        DrawStringToHandle(life_box_x + 10, life_box_y + 4, "LIFE - STOCK", GetColor(0, 255, 255), font_orbitron);

        // �c�@�\���i�������ɕ��ׂ�j
        int icon_base_x = life_box_x + 20;
        int icon_y = life_box_y + 32;
        int icon_size = 14;
        int icon_gap = 20;

        for (int i = 0; i < player->life; ++i)
        {
            int x = icon_base_x + i * icon_gap;

            // �� �O�p�`�i���^�h���[�����j
            DrawTriangle(
                x, icon_y + icon_size,           // ��
                x + icon_size / 2, icon_y,       // ��
                x + icon_size, icon_y + icon_size, // ��
                GetColor(255, 100, 100), TRUE     // �h��Ԃ�
            );
        }
    }

    // PlaySoundMem(se_charge, DX_PLAYTYPE_BACK);

    if (player)
    {
        int power_box_x = D_WIN_MAX_X - 230;
        int power_box_y = 150 + 60 + 10; // LIFE-STOCK�̉�
        int power_box_w = 200;
        int power_box_h = 50;

        DrawStringToHandle(power_box_x + 10, power_box_y + 2, "POWER UP", GetColor(0, 255, 255), font_orbitron);

        int level = player->GetPowerd();
        for (int i = 0; i < level; ++i)
        {
            int x = power_box_x + 20 + i * 20;
            int y = power_box_y + 25;
            DrawBox(x, y, x + 14, y + 14, GetColor(255, 255, 100), TRUE); // ���F�̏��u���b�N
        }
    }

    if (player)
    {
        int shield_box_x = D_WIN_MAX_X - 230;
        int shield_box_y = 150 + 60 + 10 + 60; // PowerUP�̉�
        int shield_box_w = 200;
        int shield_box_h = 50;

        DrawStringToHandle(shield_box_x + 10, shield_box_y + 2, "SHIELD", GetColor(0, 255, 255), font_orbitron);

        const char* shield_text = player->GetShieldOn() ? "ON" : "OFF";
        int color = player->GetShieldOn() ? GetColor(0, 255, 100) : GetColor(255, 100, 100);
        DrawStringToHandle(shield_box_x + 20, shield_box_y + 25, shield_text, color, font_digital);
        if (player->GetShieldOn())
        {
            float t = sinf(GetNowCount() / 30.0f) * 5 + 15; // ���a�ϓ�
            int cx = shield_box_x + 150;
            int cy = shield_box_y + 35;
            DrawCircle(cx, cy, static_cast<int>(t), GetColor(0, 255, 180), FALSE);
        }

    }


    if (player)
    {
        int exp_box_x = 30;
        int exp_box_y = D_WIN_MAX_Y - 580; // TOTAL SCORE �̏�����
        int exp_box_w = 220;
        int exp_box_h = 40;

        ScoreData* score = Singleton<ScoreData>::GetInstance();
        const auto& all_scores = score->GetScoreData();
        float total_score = 0.0f;
        for (float s : all_scores)
            total_score += s;

        DrawStringToHandle(exp_box_x + 10, exp_box_y + 4, "EXP", GetColor(0, 255, 255), font_orbitron);
        DrawFormatStringToHandle(exp_box_x + 80, exp_box_y + 8, GetColor(0, 255, 180), font_digital, "%.0f", total_score * 0.1f);
    }


    // ==== �K�E�Z�Q�[�W�iLIFE�̉��A�_�Ł����C�A�E�g�����ρj ====
    if (player)
    {
        float rate = player->GetChargeRate(); // 0.0 �` 1.0
        int gauge_x = D_WIN_MAX_X - 230;
        int gauge_y = 150;
        int gauge_w = 200;
        int gauge_h = 50;

        DrawStringToHandle(gauge_x + 10, gauge_y + 2, "CHARGE", GetColor(0, 255, 255), font_orbitron);

        int bar_x = gauge_x + 10;
        int bar_y = gauge_y + 25;
        int bar_w = gauge_w - 20;
        int bar_h = 12;

        int fill_color = player->CanUseSpecial()
            ? GetColor(0, (GetNowCount() % 100 > 50) ? 255 : 100, 255)  // �_��
            : GetColor(0, 255, 255); // �ʏ�F

        DrawBox(bar_x, bar_y, bar_x + bar_w, bar_y + bar_h, GetColor(30, 30, 30), TRUE); // �w�i
        DrawBox(bar_x, bar_y, bar_x + static_cast<int>(bar_w * rate), bar_y + bar_h, fill_color, TRUE); // �{��

        if (player->CanUseSpecial())
        {
            for (int i = 0; i < 5; ++i)
            {
                int x1 = bar_x + GetRand(bar_w);
                int x2 = x1 + GetRand(10);
                DrawLine(x1, bar_y, x2, bar_y + bar_h, GetColor(100, 255, 255));
            }
        }

    }

    // ==== SPECIAL READY UI�i�v���C���[���ɐ^�񒆕\���j ====
    if (player && player->CanUseSpecial())
    {

        Vector2D pos = player->GetLocation();
        int ui_x = static_cast<int>(pos.x) - 55;
        int ui_y = static_cast<int>(pos.y) + 40;
        int pulse = static_cast<int>(GetNowCount() % 100) > 50 ? 255 : 100;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawFormatStringToHandle(ui_x, ui_y, GetColor(255, pulse, pulse), font_orbitron, "Press B!!");
    }
    if (player && player->CanUseSpecial())
    {
        int gauge_x = D_WIN_MAX_X - 260;
        int gauge_y = 150;

        int text_x = gauge_x + 140;
        int text_y = gauge_y + 30;
        int pulse = (GetNowCount() % 100 > 50) ? 255 : 100;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawStringToHandle(text_x, text_y, "Press B!!", GetColor(255, pulse, pulse), font_orbitron);
    }




    // ==== ����UI�i�E���j ====
    int panel_x = D_WIN_MAX_X - 260;
    int panel_y = D_WIN_MAX_Y - 150;
    int panel_w = 230;
    int panel_h = 130;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(10, 10, 30), TRUE); // �w�i
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(0, 255, 255), FALSE); // �g��

    DrawStringToHandle(panel_x + 10, panel_y + 10, "Manual", GetColor(0, 255, 255), font_orbitron);

    DrawStringToHandle(panel_x + 10, panel_y + 35, "Move : L-Stick", GetColor(255, 255, 255), font_orbitron);
    DrawStringToHandle(panel_x + 10, panel_y + 55, "Shot : Space / A", GetColor(255, 255, 255), font_orbitron);
    DrawStringToHandle(panel_x + 10, panel_y + 75, "Beam : B", GetColor(255, 255, 255), font_orbitron);
    DrawStringToHandle(panel_x + 10, panel_y + 95, "Flip : RB / L", GetColor(255, 255, 255), font_orbitron);


    // ==== �T�C�h�p�l���Ƃ̋��E�Ɂu����郉�C���v�G�t�F�N�g�iStage3�͐F�ύX�j ====
    {
        const int line_width = 4;
        const int flow_speed = 1000;  // �� 1�b�Ԃ�60px�����i���� flow_speed=1 �ɋ߂Â���j
        const int gap = 70;

        int offset = static_cast<int>(line_effect_timer * flow_speed) % gap;

        int pulse = (static_cast<int>(line_effect_timer * 100) % 100 > 50) ? 255 : 100;

        int line_color = GetColor(0, pulse, 255); // �ʏ�F�l�I���u���[

        if (dynamic_cast<Stage3*>(current_stage) != nullptr)
        {
            // line_color = GetColor(255, 100, 150); // ���K�v�ɉ����ĕ���
        }

        int left_x = (D_WIN_MAX_X / 2) - 350;
        for (int y = -gap; y < D_WIN_MAX_Y + gap; y += gap)
        {
            int y0 = y + offset;
            DrawBox(left_x, y0, left_x + line_width, y0 + 20, line_color, TRUE);
        }

        int right_x = (D_WIN_MAX_X / 2) + 350 - line_width;
        for (int y = -gap; y < D_WIN_MAX_Y + gap; y += gap)
        {
            int y0 = y + offset;
            DrawBox(right_x, y0, right_x + line_width, y0 + 20, line_color, TRUE);
        }
    }


    if (isPaused)
    {
        const int cx = D_WIN_MAX_X / 2;
        const int cy = D_WIN_MAX_Y / 2 - 20;

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
        DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
        //DrawBox(400, 200, 880, D_WIN_MAX_Y - 200, GetColor(255, 255, 255), FALSE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


        const char* menuItems[] = {
            "START GAME",
            "BACK TITLE",
                };

        for (int i = 0; i < 2; ++i)
        {
            int y = 300 + i * 50;
            int textWidth = GetDrawStringWidthToHandle(menuItems[i], strlen(menuItems[i]), m_menuFontHandle);
            int x = (D_WIN_MAX_X - textWidth) / 2;

            if (i == m_selectedIndex)
            {
                // =========================
                // �w�i�n�C���C�g�o�[�i��ʒ[�܂Łj
                // =========================
                int barHeight = 40;
                int barAlpha = 120 + (int)(sinf(GetNowCount() / 60.0f) * 50); // �p���X
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, barAlpha);
                DrawBox(
                    0, y - 5,
                    D_WIN_MAX_X, y + barHeight,
                    GetColor(0, 200, 255), TRUE
                );

                // =========================
                // ���[�̃G�b�W�G�t�F�N�g�i�������j
                // =========================
                int edgeWidth = 80;
                int edgeHeight = 4;
                int scrollSpeed = 2;
                int edgeOffset = (GetNowCount() * scrollSpeed) % (D_WIN_MAX_X + edgeWidth * 2);

                // ������E�֗����i2�\�����ă��[�v�����o���j
                for (int j = 0; j < 2; ++j)
                {
                    int edgeX = edgeOffset - edgeWidth * j;
                    SetDrawBlendMode(DX_BLENDMODE_ADD, 50);
                    DrawBox(
                        edgeX, y + 10,
                        edgeX + edgeWidth, y + 10 + edgeHeight,
                        GetColor(200, 255, 255), TRUE
                    );
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }

                // =========================
                // �e�L�X�g�i�A�E�g���C���{�O���[�j
                // =========================
                int offsetX = (rand() % 3) - 1;
                int offsetY = (rand() % 3) - 1;

                DrawStringToHandle(x + offsetX - 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
                DrawStringToHandle(x + offsetX + 1, y + offsetY, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
                DrawStringToHandle(x + offsetX, y + offsetY - 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);
                DrawStringToHandle(x + offsetX, y + offsetY + 1, menuItems[i], GetColor(0, 0, 0), m_menuFontHandle);

                int glow = (int)((sinf(GetNowCount() / 30.0f) + 1.0f) * 127);
                DrawStringToHandle(x + offsetX, y + offsetY, menuItems[i], GetColor(100 + glow, 255, 255), m_menuFontHandle);
            }
            else
            {
                DrawStringToHandle(x, y, menuItems[i], GetColor(180, 180, 180), m_menuFontHandle);
            }
        }


    }

    if (effect_shield_on || effect_shield_off || effect_powerup)
    {
        int alpha = static_cast<int>(100 * (1.0f - (effect_timer / effect_duration))); // �ő�100�ɗ}����

        int color = GetColor(0, 0, 0); // �f�t�H���g�i�������j

        if (effect_shield_on)
            color = GetColor(0, 255, 180); // �Όn�i�V�[���hON�j
        else if (effect_shield_off)
            color = GetColor(255, 50, 50); // �Ԍn�i�V�[���h�j��j
        else if (effect_powerup)
            color = GetColor(255, 255, 100); // ���n�i�p���[�A�b�v�j

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, color, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

}



void GameMainScene::InputSePlay()
{
    InputManager* input = Singleton<InputManager>::GetInstance();
    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
    AnimationManager* am = Singleton<AnimationManager>::GetInstance();
    int se = NULL;

    if (input->GetKeyDown(KEY_INPUT_1))
    {
        am->PlaySE(SE_NAME::Shot);
    }
    if (input->GetKeyDown(KEY_INPUT_2))
    {
        se = rm->GetSounds("Resource/sound/se/effect/audiostock_1133382.mp3");
        PlaySoundMem(se, DX_PLAYTYPE_LOOP);
    }
    if (input->GetKeyDown(KEY_INPUT_3))
    {
        se = rm->GetSounds("Resource/sound/se/effect/audiostock_1133382.mp3");
        PlaySoundMem(se, DX_PLAYTYPE_LOOP);
    }
    if (input->GetKeyDown(KEY_INPUT_4))
    {
        se = rm->GetSounds("Resource/sound/se/effect/audiostock_1133382.mp3");
        PlaySoundMem(se, DX_PLAYTYPE_LOOP);
    }
    if (input->GetKeyDown(KEY_INPUT_5))
    {
        se = rm->GetSounds("Resource/sound/se/effect/audiostock_1133382.mp3");
        PlaySoundMem(se, DX_PLAYTYPE_LOOP);
    }
    if (input->GetKeyDown(KEY_INPUT_6))
    {
        se = rm->GetSounds("Resource/sound/se/effect/audiostock_1133382.mp3");
        PlaySoundMem(se, DX_PLAYTYPE_LOOP);
    }

    if (input->GetKeyDown(KEY_INPUT_UP))
    {
        vo++;

        if (vo > 100)
        {
            vo = 100;
        }
    }
    else if (input->GetKeyDown(KEY_INPUT_DOWN))
    {
        vo--;

        if (vo < 0)
        {
            vo = 0;
        }
    }

    int volume = 255 * vo / 100;
    ChangeVolumeSoundMem(volume, se);
}

