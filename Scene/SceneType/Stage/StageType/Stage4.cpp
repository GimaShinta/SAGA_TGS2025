#include "Stage4.h"
#include "../../../../Utility/AnimationManager.h"
#include "../../../../Utility/ResourceManager.h"
#include "../../../../Utility/InputManager.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Boss3.h"
#include "Stage3.h"
#include "../../../../Object/Item/PowerUp/PowerUp.h"
#include "../../../../Object/Item/Shield/Shield.h"
#include <memory>



Stage4::Stage4(Player* player) : StageBase(player)
{
}

Stage4::~Stage4()
{
}

void Stage4::Initialize()
{
	// ����������
// �y�L�q�ʒu�͉��z�X�e�[�W�̒�������
	distance = STAGE3_DISTANCE;
    //GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    //objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) + 100.0f));
    //objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) + 50.0f));

	//�t�H���g
// font_digital = CreateFontToHandle("DS-Digital", 28, 6, DX_FONTTYPE_ANTIALIASING);
	font_digital = CreateFontToHandle("Orbitron", 28, 6, DX_FONTTYPE_ANTIALIASING);
	font_orbitron = CreateFontToHandle("Orbitron", 22, 6, DX_FONTTYPE_ANTIALIASING);

    stage_id = StageID::Stage4;
}

void Stage4::Finalize()
{
	// �I������
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
	objm->Finalize();
	//AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
	//manager->RemoveAnimation(anim_id);

}

void Stage4::Update(float delta)
{
	UpdateBackgroundScroll(delta);

    // delta_second �����Z
    stage_timer += delta;

    //// ��莞�Ԃ��Ƃ� distance �����炷�i��F0.1�b���Ƃ�1���炷�j
    //if (stage_timer >= 0.01f)
    //{
    //    if (distance > 0)
    //    {
    //        distance--;
    //    }
    //    else
    //    {
    //        distance = 0;
    //    }
    //    stage_timer = 0;
    //}

    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    // ���t���[��
    manager->Update(delta);       // �X�V�ideltaTime�͑O�̃t���[���Ƃ̌o�ߎ��ԁj

    // �A�j���[�V�������I���������m�F�i�폜���K�v�ȏꍇ�j
    if (manager->GetAnimationFinished(anim_id) == true)
    {
        manager->RemoveAnimation(anim_id);  // �s�v�ɂȂ�����폜
    }

    // �v���C���[�̍U��
    PlayerShot();

    // �G�̏o��
    EnemyAppearance();

    // �G�̍U��
    EnemyShot(delta);

    // �x���\��
    DisplayWarning(delta);

    // �N���A����
    UpdateGameStatus(delta);

    if (is_clear)
    {
        if (result_started)
        {
            ResultDraw(delta);
        }
    }

    if (glitch_done)
    {
        finished = true;
    }

}

void Stage4::Draw()
{
    // �w�i�𔒂œh��i�ŏ��ɕ`��j
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    //�X�N���[���w�i
    DrawScrollBackground();

    // ���̍���
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // �E�̍���
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "Stage4", GetColor(255, 255, 255));
    DrawFormatString(0, 20, GetColor(255, 255, 0), "�G��: %d", enemy_list.size());

    // �X�e�[�W�`��
    if (stage <= 1)
    {
        if (is_warning == true)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, brend);
            DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(255, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        }
    }

    // --- �`�揇�𐧌� ---
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();


    if (draw_animation_first)  // �� ���̃t���O�ŏ�����؂�ւ���
    {
        manager->Draw();       // ��ɃA�j���[�V����
        objm->Draw();          // ��ɃI�u�W�F�N�g
    }
    else
    {
        objm->Draw();          // ��ɃI�u�W�F�N�g
        manager->Draw();       // ��ɃA�j���[�V����
    }

    if (is_clear == true)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
        DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawFormatStringToHandle((D_WIN_MAX_X / 2) - 100.0f, 200, GetColor(255, 255, 255), font_digital, "GAME CLEAR");

        if (result_started)
        {
            int fade_alpha = (result_timer * 12.0f < 60.0f) ? static_cast<int>(result_timer * 12.0f) : 60;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
            DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
            ResultDraw(delta_draw);
        }



        //DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "�Q�[���N���A", GetColor(0, 0, 0));
    }
    else if (is_over == true)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
        DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawFormatStringToHandle((D_WIN_MAX_X / 2) - 100.0f, (D_WIN_MAX_Y / 2), GetColor(255, 255, 255), font_digital, "GAME OVER");

        //DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "�Q�[���I�[�o�[", GetColor(0, 0, 0));
    }

}

bool Stage4::IsFinished()
{
	return finished;
}

void Stage4::SetFinished()
{
    finished = true;
}


bool Stage4::IsClear()
{
	return is_clear;
}

bool Stage4::IsOver()
{
	return is_over;
}

StageBase* Stage4::GetNextStage(Player* player)
{
	return nullptr;
}

StageBase* Stage4::GetNowStage()
{
    return this;
}

void Stage4::DisplayWarning(float delta_second)
{
    // �x���\���i�Ԃ�����j
    if (is_warning == true)
    {
        // �J�E���g���Z
        warning_timer += delta_second;
        if (warning_timer >= 0.0005f)
        {
            // �����x���O����Q�T�T�܂ŉ��Z���Z���J��Ԃ�
            if (is_brend == false)
            {
                if (brend < 255)
                {
                    brend++;
                }
                else
                {
                    is_brend = true;
                }
            }
            else
            {
                if (brend > 0)
                {
                    brend--;
                }
                else
                {
                    // ���B�J�E���g�����Z
                    reach_count++;
                    is_brend = false;
                }
            }
            // �J�E���g���Z�b�g
            warning_timer = 0;
        }
        // �R��x�����I�������S�ă��Z�b�g
        if (reach_count >= 3)
        {
            is_warning = false;
            brend = 255;
            warning_timer = 0;
            reach_count = 0;
        }
    }

}

void Stage4::PlayerShot()
{
    // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // �v���C���[���e��ł������ł��Ă�����e�𐶐�        
    if (player->GetIsShot() == true)
    {
        Vector2D p_location = player->GetLocation();
        player->SetIsShot();
        // �㉺���]���Ă��Ȃ������牺�����ɐ���
        if (player->GetShotFlip() == false)
        {
            if (player->GetPowerd() <= 1)
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));

            }
            else if (player->GetPowerd() == 2)
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 30, p_location.y));
            }
            else if (player->GetPowerd() == 3)
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 50, p_location.y + D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 50, p_location.y + D_OBJECT_SIZE));

            }
            //else
            //{
            //    shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 70, p_location.y + (D_OBJECT_SIZE * 2)));
            //    shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 50, p_location.y + D_OBJECT_SIZE));
            //    shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 30, p_location.y));
            //    shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
            //    shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
            //    shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 30, p_location.y));
            //    shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 50, p_location.y + D_OBJECT_SIZE));
            //    shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 70, p_location.y + (D_OBJECT_SIZE * 2)));

            //}
            shot->SetShotFlip(false);
        }
        // ���]���Ă����������ɐ���
        else
        {
            Vector2D p_location = player->GetLocation();
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x, p_location.y + D_OBJECT_SIZE));
            shot->SetShotFlip(true);
        }
    }
    // �v���C���[���r�[�������������ł��Ă�����r�[���𐶐�
    else if (player->GetBeamOn() == true)
    {
        Vector2D p_location = player->GetLocation();
        // �㉺���]���Ă��Ȃ������牺�����ɐ���
        if (player->GetShotFlip() == false)
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y - D_OBJECT_SIZE) - 848));
            beam->SetBeamFlip(false);
        }
        // ���]���Ă����������ɐ���
        else
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y + D_OBJECT_SIZE) + 848));
            beam->SetBeamFlip(true);
        }
        beam->SetPlayer(player);
        player->SetBeamOn();
    }
}

void Stage4::EnemyAppearance()
{
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    if (boss3_spawned) return;


    if (stage_timer < 10.0f)
    {
    }
    else 
    {
        //objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 - 60, 120))->SetPlayer(player);
        objm->CreateObject<Shield>(Vector2D(D_WIN_MAX_X / 2, 120))->SetPlayer(player);

        boss3 = objm->CreateObject<Boss3>(Vector2D(D_WIN_MAX_X / 2, -100));
        boss3->SetPlayer(player);
        boss3_spawned = true;
    }
}

void Stage4::EnemyShot(float delta_second)
{
}

void Stage4::UpdateGameStatus(float delta)
{
    //// �{�X���|�ꂽ��N���A
    //if (boss != nullptr && boss->GetIsAlive() == false && is_over == false)
    //{
    //    boss->SetDestroy();
    //    is_clear = true;
    //}

    if (boss3 != nullptr && boss3->GetIsCrashing() == true)
    {
        draw_animation_first = true;
    }


    // �{�X�Q���|�ꂽ��N���A
    if (boss3 != nullptr && boss3->GetIsAlive() == false && is_over == false)
    {
        boss3->SetDestroy();
        is_clear = true;
    }

    // �v���C���[���|�ꂽ��Q�[���I�[�o�[
    if (player != nullptr && player->GetIsAlive() == false && is_clear == false)
    {
        // �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
        GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
        objm->Finalize();
        is_over = true;
    }

    if (is_clear == true || is_over == true)
    {
        scene_timer += delta;
        gameover_timer += delta;

        if (gameover_timer >= 0.005f)
        {
            transparent++;
            gameover_timer = 0.0f;
        }

        if (scene_timer >= 10.0f)
        {
            finished = true;
        }
    }

    // ���̏����F�X�y�[�X�L�[����������X�e�[�W�I��
    if (CheckHitKey(KEY_INPUT_N))
    {
        finished = true;
    }
    if (is_clear == true && result_started == false)
    {
        clear_wait_timer += delta;
        if (clear_wait_timer >= 5.0f)
        {
            result_started = true;
            result_timer = 0.0f; // �X�R�A���o�^�C�}�[���Z�b�g
        }
    }

}

void Stage4::ResultDraw(float delta)
{
    if (!result_started) return;

    // �^�C�}�[�i�s
    if (!result_fadeout_started)
        result_timer += delta * 60.0f;
    else
        result_fadeout_timer += delta * 60.0f;

    const int cx = D_WIN_MAX_X / 2;
    const int cy = D_WIN_MAX_Y / 2 - 20;

    // �w�i�t�F�[�h�i�Œ�j
    int fade_alpha = (result_timer * 12.0f < 60.0f) ? static_cast<int>(result_timer * 12.0f) : 60;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
    DrawBox(cx - 350, 0, cx + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // �t�F�[�h�^�X���C�h�萔
    const int fade_duration = 60;
    const int slide_distance = 60;

    // �\���⏕�֐�
    auto GetAlpha = [&](int delay, bool is_fadeout = false) -> int {
        int t = static_cast<int>(result_fadeout_started ? result_fadeout_timer : result_timer) - delay;
        if (t < 0) return is_fadeout ? 255 : 0;
        if (t >= fade_duration) return is_fadeout ? 0 : 255;
        return is_fadeout ? 255 - (255 * t) / fade_duration : (255 * t) / fade_duration;
        };

    auto GetSlideY = [&](int base_y, int delay, bool is_fadeout = false) -> int {
        int t = static_cast<int>(result_fadeout_started ? result_fadeout_timer : result_timer) - delay;
        if (t < 0) return base_y + (is_fadeout ? 0 : slide_distance);
        if (t >= fade_duration) return base_y + (is_fadeout ? slide_distance : 0);
        int offset = (slide_distance * t) / fade_duration;
        return is_fadeout ? base_y + offset : base_y + slide_distance - offset;
        };

    // �X�R�A�W�v
    ScoreData* score = Singleton<ScoreData>::GetInstance();
    const auto& scores = score->GetScoreData();
    float base_score = 0.0f;
    for (float s : scores) base_score += s;
    int life_bonus = player->life * 1000;
    total_score = base_score;
    
    if (scored == false)
    {
        score->SetScoreData(life_bonus);
        scored = true;
    }

    // �\�����C���ݒ�
    struct ResultLine {
        int delay;
        int y_offset;
        std::string label;
        std::string format;
    };

    std::vector<ResultLine> lines = {
        {  30, -80, "RESULT", "" },
        {  70, -20, "BASE SCORE", "BASE SCORE : %.0f" },
        { 110,  20, "LIFE BONUS", "LIFE BONUS : %d" },
        { 160,  60, "TOTAL SCORE", "TOTAL SCORE : %.0f" },
    };

    bool fading_out = result_fadeout_started;

    for (size_t i = 0; i < lines.size(); ++i)
    {
        const auto& line = lines[i];

        // �t�F�[�h�A�E�g���� delay ���t���ɂ���
        int delay = line.delay;
        if (result_fadeout_started)
        {
            delay = lines.back().delay - line.delay;
        }

        int alpha = GetAlpha(delay, result_fadeout_started);
        int y = GetSlideY(cy + line.y_offset, delay, result_fadeout_started);
        int color = GetColor(255, 255, 255);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

        char buf[128];
        if (line.format.empty()) {
            sprintf_s(buf, "%s", line.label.c_str());
        }
        else if (line.label == "BASE SCORE") {
            sprintf_s(buf, line.format.c_str(), base_score - life_bonus);
        }
        else if (line.label == "LIFE BONUS") {
            sprintf_s(buf, line.format.c_str(), life_bonus);
        }
        else if (line.label == "TOTAL SCORE") {
            sprintf_s(buf, line.format.c_str(), total_score);
        }

        int width = GetDrawStringWidthToHandle(buf, strlen(buf), font_digital);
        DrawStringToHandle(cx - width / 2, y, buf, color, font_digital);

        if (line.label == "TOTAL SCORE" && alpha == 255 && !result_fadeout_started) {
            result_displayed = true;
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    // �\����̃O���b�`�ҋ@
    if (result_displayed && !glitch_started && !glitch_done) {
        post_result_wait_timer += delta;
        if (post_result_wait_timer >= 8.0f) {
            glitch_started = true;
            glitch_timer = 0.0f;
        }
    }

    // �O���b�`���o
    if (glitch_started && !glitch_done) {
        //for (int i = 0; i < 50; ++i) {
        //    int x = (rand() % 700) + (D_WIN_MAX_X / 2 - 350);
        //    int y = rand() % D_WIN_MAX_Y;
        //    int w = 80 + rand() % 150;
        //    int h = 8 + rand() % 40;
        //    int r = 150 + rand() % 106;
        //    int g = 150 + rand() % 106;
        //    int b = 255;
        //    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
        //    DrawBox(x, y, x + w, y + h, GetColor(r, g, b), TRUE);
        //}

        glitch_timer += delta;
        if (glitch_timer > 2.0f) {
            glitch_done = true;
        }
    }

    // �t�F�[�h�A�E�g�J�n
    if (glitch_done && !result_fadeout_started) {
        result_fadeout_started = true;
        result_fadeout_timer = 0.0f;
    }

    // �t�F�[�h�A�E�g�����ŏI��
    if (result_fadeout_started && !result_ended) {
        int last_delay = lines.back().delay;
        if (result_fadeout_timer >= fade_duration + last_delay) {
            result_ended = true;
            finished = true;
            // �K�v�ł���Ύ��̃V�[���֐؂�ւ��Ȃ�
        }
    }

    //// �t�F�[�h�A�E�g�����ňÓ]�t�F�[�h�C���J�n
    //if (result_ended && !black_fade_started) {
    //    black_fade_started = true;
    //    black_fade_timer = 0.0f;
    //}

    //// �����Ó]�t�F�[�h
    //if (black_fade_started)
    //{
    //    black_fade_timer += delta;

    //    static int alpha = 0;
    //    if (black_fade_timer >= 0.01f)
    //    {
    //        black_fade_timer = 0.0f;
    //        alpha++;
    //    }
    //    //int alpha = static_cast<int>((black_fade_timer / black_fade_duration) * 255.0f);
    //    if (alpha > 255) alpha = 255;

    //    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    //    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
    //    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //    if (alpha >= 255)
    //    {
    //        finished = true;

    //        // �Ó]���� �� ���̃V�[���֐؂�ւ��Ȃ�
    //        // SceneManager::ChangeScene(...);
    //    }
    //}


    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


}

void Stage4::DrawScrollBackground() const
{
    static float time = 0.0f;
    time += 0.05f;

    // �w�i�F�F���Ԃ݂��������O���[
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(30, 10, 10), TRUE);

    // === �w�ʃ��C���[�i���E�ׂ����E�Â߁j ===
    const int grid_size_back = 40;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(100, 0, 0));

    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(100, 0, 0));
    }

    // === �O�ʃ��C���[�i�����E���邭�E�x�����j ===
    const int grid_size_front = 80;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(255, 40, 40), TRUE);

    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(255, 40, 40), TRUE);
    }

    // === �m�C�Y���o�i���󊴁j ===
    for (int i = 0; i < 5; ++i) {
        if (rand() % 70 == 0) {
            int nx = rand() % D_WIN_MAX_X;
            int ny = rand() % D_WIN_MAX_Y;
            DrawBox(nx, ny, nx + 3, ny + 3, GetColor(255, 100, 50), TRUE); // �I�����W�x���h�b�g
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

}
