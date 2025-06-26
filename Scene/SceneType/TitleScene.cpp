#include "TitleScene.h"
#include "DxLib.h"
#include <corecrt_math.h>

TitleScene::TitleScene()
    : m_selectedIndex(0), m_menuFontHandle(0), m_logoHandle(0), m_logoScale(0.0f), m_logoAlpha(0),
    m_logoX(0), m_logoY(0),m_logoW(0), m_logoH(0),drawY(0)
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    m_selectedIndex = 0;
    m_menuFontHandle = CreateFontToHandle("Orbitron", 36, 6); // ���j���[��p�t�H���g
    m_cursorY = 370.0f; // ����Y���W

    m_startTransitioning = false;
    m_transitionTimer = 0.0f;
    m_menuScale = 1.0f;
    m_menuAlpha = 255.0f;

    m_logoAppearTimer = 0.0f;
    m_logoAppeared = false;
    m_logoScale = 1.3f; // ���T���߂Ȋg�嗦��
    m_logoAlpha = 0;

    ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
   // logo_se = rm->GetSounds("Resource/sound/se/battle/audiostock_1498491.mp3");
    bgm = rm->GetSounds("Resource/sound/bgm/TitleBGM.mp3");
    cursor_se = rm->GetSounds("Resource/sound/se/se_effect/cursor.mp3");
    tap_se = rm->GetSounds("Resource/sound/se/effect/start_se.mp3");

    ChangeVolumeSoundMem(255 * 60 / 100, bgm);
    PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);

    

    // ���S�摜�ǂݍ��݁i����PNG�j
    m_logoHandle = rm->GetImages("Resource/Image/BackGround/Title/DigitalNexus.png")[0];

    m_logoScale = 1.5f;   // �g�傩��n�܂�
    m_logoAlpha = 0;      // �t�F�[�h�C���J�n

    // ���S�T�C�Y�擾
    GetGraphSize(m_logoHandle, &m_logoW, &m_logoH);
    m_logoScale = 0.7f;
    m_logoAlpha = 0;

    // ���S�\���ʒu�i�����񂹁j
    m_logoX = (D_WIN_MAX_X - (int)(m_logoW * m_logoScale)) / 2;
    m_logoY = -120;

    // ���q���������i�����_���Ȉʒu�ɔz�u�j
    for (int i = 0; i < 30; ++i)
    {
        Particle p;
        p.x = (float)(rand() % D_WIN_MAX_X);
        p.y = (float)(D_WIN_MAX_Y - rand() % 100); // �� �����烉���_��
        p.vy = -0.3f - (rand() % 30) / 100.0f;
        p.alpha = 128 + rand() % 128;
        m_particles.push_back(p);
    }

}


eSceneType TitleScene::Update(float delta_second)
{
    InputManager* input = Singleton<InputManager>::GetInstance();

    // �㉺�L�[ or �R���g���[���\���őI��
    bool movedUp = input->GetKeyDown(KEY_INPUT_UP) || input->GetKeyDown(KEY_INPUT_W) ||
        input->GetButtonDown(XINPUT_BUTTON_DPAD_UP);
    bool movedDown = input->GetKeyDown(KEY_INPUT_DOWN) || input->GetKeyDown(KEY_INPUT_S) ||
        input->GetButtonDown(XINPUT_BUTTON_DPAD_DOWN);

    // �X�e�B�b�NY���̓��͂𔽉f�i�㉺�Ŕ���A�N�[���_�E������t���j
    static float stickCooldown = 0.0f;
    stickCooldown -= delta_second;

    Vector2D stick = input->GetLeftStick(); // ���X�e�B�b�NY���F�と���A������
    const float STICK_THRESHOLD = 0.5f;     // ���x����
    const float COOLDOWN_TIME = 0.2f;       // �A�����͖h�~

    if (stickCooldown <= 0.0f)
    {
        if (stick.y > STICK_THRESHOLD)
        {
            movedUp = true;
            stickCooldown = COOLDOWN_TIME;
        }
        else if (stick.y < -STICK_THRESHOLD)
        {
            movedDown = true;
            stickCooldown = COOLDOWN_TIME;
        }
    }

    if (movedUp)
    {
        PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
        m_selectedIndex = (m_selectedIndex + 3) % 4;
    }
    if (movedDown)
    {
        PlaySoundMem(cursor_se, DX_PLAYTYPE_BACK);
        m_selectedIndex = (m_selectedIndex + 1) % 4;
    }


    // ����i�X�y�[�X or A�{�^���j
    if (!m_startTransitioning && (input->GetKeyDown(KEY_INPUT_SPACE) || input->GetButtonDown(XINPUT_BUTTON_A)))
    {
        PlaySoundMem(tap_se, DX_PLAYTYPE_BACK);

        if (m_selectedIndex == 0) {
            m_startTransitioning = true;
            m_transitionTimer = 0.0f;
        }
        else if (m_selectedIndex == 1) {
            return eSceneType::eRanking;
        }
        else if (m_selectedIndex == 3) {
            return eSceneType::eXit;
        }
    }

    // �o�����o���̃^�C�}�[�i�s
    if (m_startTransitioning)
    {
        StopSoundMem(bgm);

        m_transitionTimer += delta_second;

        if (m_transitionTimer >= 2.0f) // 2�b�Ŏ��̃V�[����
            return eSceneType::eGameMain;
    }


    // ���S���o�i�������k���������x�A�b�v�j
    if (!m_logoAppeared)
    {
        m_logoAppearTimer += delta_second;
        float t = m_logoAppearTimer / 1.0f;
        if (t >= 1.0f)
        {
            t = 1.0f;
            m_logoAppeared = true;
        }

        // 1.3 �� 0.7 �ɃC�[�W���O�i�ɂ₩�ɏk���j
        m_logoScale = 0.7f + (1.3f - 0.7f) * powf(1.0f - t, 2); // EaseOutQuad
        m_logoAlpha = (int)(t * 255.0f);
    }



    // --- �J�[�\���i���j��Y�ʒu���C�[�W���O�ŒǏ] ---
    float targetY = 400.0f + m_selectedIndex * 50.0f;
    float diff = targetY - m_cursorY;
    m_cursorY += diff * 0.2f;
    if (fabsf(diff) < 1.0f) m_cursorY = targetY;

    if (m_startTransitioning)
    {
        m_transitionTimer += delta_second;

        // ���j���[���ڂ̊g��E�������o
        m_menuScale += delta_second * 1.0f;   // �g��
        m_menuAlpha -= delta_second * 300.0f; // �t�F�[�h�A�E�g
        if (m_menuAlpha < 0.0f) m_menuAlpha = 0.0f;

        if (m_transitionTimer >= 2.0f)
            return eSceneType::eGameMain;
    }

    // �p�[�e�B�N���̍X�V
    for (auto& p : m_particles)
    {
        p.y += p.vy;
        if (p.y < 0) p.y = D_WIN_MAX_Y; // ��ɏo���牺����o��悤�Ƀ��[�v
    }


    return GetNowSceneType();
}

void TitleScene::Draw()
{
    DrawBackgroundGrid();
    DrawLogo();
    DrawParticles();
    DrawMenu();
    if (m_startTransitioning)
    {
        float t = m_transitionTimer / 2.0f;
        if (t > 1.0f) t = 1.0f;

        // �C�[�W���O�iEaseInOutCubic�j
        float eased = t < 0.5f
            ? 4 * t * t * t
            : 1 - powf(-2 * t + 2, 3) / 2;

        // �X�P�[���F�ŏ��͏����g�偨�����ɋz������
        float scale = 0.7f * (1.0f - eased) + 1.2f * (1.0f - (1.0f - eased) * (1.0f - eased));
        int alpha = (int)((1.0f - eased) * 255);

        int scaledW = (int)(m_logoW * scale);
        int scaledH = (int)(m_logoH * scale);

        int drawX = (D_WIN_MAX_X - scaledW) / 2;
        int drawY = m_logoY + (int)((m_logoH * 0.7f - scaledH) / 2 - 50 * eased);  // ������ɏオ��

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
        DrawExtendGraph(drawX, drawY, drawX + scaledW, drawY + scaledH, m_logoHandle, TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // �c�� or �M�����̔��g�i�����j
        if (eased < 0.8f) {
            int glowAlpha = (int)((1.0f - eased) * 80);
            SetDrawBlendMode(DX_BLENDMODE_ADD, glowAlpha);
            DrawBox(drawX - 8, drawY - 8, drawX + scaledW + 8, drawY + scaledH + 8, GetColor(180, 240, 255), FALSE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        // �w�i�̈Ó]
        int fade = (int)(eased * 255);
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);
        DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        // CONNECTING �\��
        // �Q�[�W�� LOADING ���o�i1.4�b��ɕ\���j
if (m_transitionTimer > 1.4f)
{
    float loadingT = (m_transitionTimer - 1.4f) / 0.6f; // 0?1
    if (loadingT > 1.0f) loadingT = 1.0f;

    int cx = D_WIN_MAX_X / 2;
    int cy = D_WIN_MAX_Y / 2 + 40;

    const int barWidth = 300;
    const int barHeight = 16;
    int filledWidth = (int)(barWidth * loadingT);

    // �g
    DrawBox(cx - barWidth / 2, cy, cx + barWidth / 2, cy + barHeight, GetColor(100, 100, 100), FALSE);
    // ���g
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(cx - barWidth / 2, cy, cx - barWidth / 2 + filledWidth, cy + barHeight, GetColor(80, 200, 255), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // LOADING����
    const char* msg = "LOADING";
    int msgWidth = GetDrawStringWidthToHandle(msg, strlen(msg), m_menuFontHandle);
    DrawStringToHandle(cx - msgWidth / 2, cy - 30, msg, GetColor(200, 255, 255), m_menuFontHandle);
}

    }




}


void TitleScene::Finalize()
{
    // �t�H���g�n���h�����
    DeleteFontToHandle(m_menuFontHandle);

}

eSceneType TitleScene::GetNowSceneType() const
{
    return eSceneType::eTitle;
}

void TitleScene::DrawMenu()
{
    const char* menuItems[] = {
        "START GAME",
        "RANKING",
        "CREDITS",
        "EXIT"
    };

    for (int i = 0; i < 4; ++i)
    {
        int y = 400 + i * 50;
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

    for (int i = 0; i < 4; ++i)
    {
        int y = 400 + i * 50;
        int textWidth = GetDrawStringWidthToHandle(menuItems[i], strlen(menuItems[i]), m_menuFontHandle);
        int x = (D_WIN_MAX_X - textWidth) / 2;

        // --- �o�����̓��ʉ��o ---
        if (m_startTransitioning)
        {
            if (i == m_selectedIndex)
            {
                // �g��E�����E�t�F�[�h���o
                float glowScale = m_menuScale;
                int baseTextWidth = GetDrawStringWidthToHandle(menuItems[i], strlen(menuItems[i]), m_menuFontHandle);
                int scaledTextWidth = (int)(baseTextWidth * glowScale);

                int drawX = (D_WIN_MAX_X - scaledTextWidth) / 2;
                int drawY = (int)(400 + i * 50 - (glowScale - 1.0f) * 20); // ������ɕ␳

                SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)m_menuAlpha);
                DrawExtendStringToHandle(drawX, drawY, glowScale, glowScale,
                    menuItems[i], GetColor(255, 255, 255), m_menuFontHandle);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            }

            continue; // �����j���[�͔�\��
        }

        // �ʏ�̕`�揈���i�ȗ��������̂܂܁j
        if (i == m_selectedIndex)
        {
            int barHeight = 40;
            int barAlpha = 120 + (int)(sinf(GetNowCount() / 60.0f) * 50);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, barAlpha);
            DrawBox(0, y - 5, D_WIN_MAX_X, y + barHeight, GetColor(0, 200, 255), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

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




void TitleScene::DrawBackgroundGrid()
{
    int grid_size = 40;
    int pulseColor = 50 + (int)(sinf(GetNowCount() / 60.0f) * 50);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, pulseColor, 100));
    for (int y = 0; y < D_WIN_MAX_Y; y += grid_size)
        DrawLine(0, y, D_WIN_MAX_X, y, GetColor(0, pulseColor, 100));
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::DrawLogo()
{
    int scaledW = (int)(m_logoW * m_logoScale);
    int scaledH = (int)(m_logoH * m_logoScale);
    int drawX = (D_WIN_MAX_X - scaledW) / 2;
    int drawY = m_logoY + (int)((m_logoH * 0.7f - scaledH) / 2);

    // �T���߃t���b�V���i�������͂̂݁j
    if (!m_logoAppeared)
    {
        //int flashAlpha = (int)((1.0f - m_logoAppearTimer / 1.0f) * 100.0f);
        //SetDrawBlendMode(DX_BLENDMODE_ADD, flashAlpha);
        //DrawBox(drawX - 10, drawY - 10, drawX + scaledW + 10, drawY + scaledH + 10, GetColor(100, 200, 255), TRUE);
        //SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_logoAlpha);
    DrawExtendGraph(drawX, drawY, drawX + scaledW, drawY + scaledH, m_logoHandle, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}



void TitleScene::DrawParticles()
{
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    for (const auto& p : m_particles)
    {
        DrawCircle((int)p.x, (int)p.y, 2, GetColor(100, 200, 255), TRUE);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

