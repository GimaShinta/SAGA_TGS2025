#include "Stage3.h"
#include "Stage2.h"
#include "Stage1.h"

#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"
#include "../../../../Object/GameObjectManager.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako5.h"

#include <cstdlib>
#include <ctime>

Stage2::Stage2(Player* player) : StageBase(player)
{}
Stage2::~Stage2()
{}

void Stage2::Initialize()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // ランダムシード設定
    stage_timer = 20.0f;
    enemy_spawn_timer = 0.0f;
    zako5_spawned = false;
}

void Stage2::Finalize()
{
    for (auto& enemy : enemy_list)
    {
        enemy->SetDestroy();
    }
    enemy_list.clear();
}

void Stage2::Update(float delta)
{
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    stage_timer += delta;
    EnemyAppearance(delta);
    UpdateBackgroundScroll(delta);

    // プレイヤーの弾発射処理
    if (player->GetIsShot())
    {
        Vector2D p_location = player->GetLocation();
        player->SetIsShot();
        if (!player->GetShotFlip())
        {
            objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE))->SetShotFlip(false);
            objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE))->SetShotFlip(false);
        }
        else
        {
            objm->CreateObject<Shot>(Vector2D(p_location.x, p_location.y + D_OBJECT_SIZE))->SetShotFlip(true);
        }
    }
    // ビーム発射処理
    else if (player->GetBeamOn())
    {
        Vector2D p_location = player->GetLocation();
        const float beam_offset = 848.0f;
        Beam* beam = nullptr;
        if (!player->GetShotFlip())
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y - D_OBJECT_SIZE) - beam_offset));
            beam->SetBeamFlip(false);
        }
        else
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y + D_OBJECT_SIZE) + beam_offset));
            beam->SetBeamFlip(true);
        }
        beam->SetPlayer(player);
        player->SetBeamOn();
    }

    // デバッグ用キー入力
    if (CheckHitKey(KEY_INPUT_N)) finished = true;
    if (CheckHitKey(KEY_INPUT_U)) is_clear = true;
    if (CheckHitKey(KEY_INPUT_I)) is_over = true;

    // タイムで自動クリア
    if (stage_timer >= 60.0f)

    {
        is_clear = true;
    }

    // ステージ終了処理
    if (is_clear || is_over)
    {
        for (auto& enemy : enemy_list)
        {
            enemy->SetDestroy();
        }
        enemy_list.clear();

        scene_timer += delta;
        if (fade_alpha >= 255.0f && scene_timer >= 1.5f)
        {
            finished = true;
        }
    }

    UpdateFade(delta);
}

void Stage2::Draw()
{
    // 背景塗りつぶし
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);
    DrawScrollBackground();

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Draw();

    // 黒帯
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "STAGE2", GetColor(255, 255, 255));
    DrawFormatString(0, 50, GetColor(255, 255, 0), "Enemy: %d", enemy_list.size());
    // 右側操作説明パネル
    int panel_x = D_WIN_MAX_X - 270;
    int panel_y = 60;
    int panel_w = 250;
    int panel_h = 180;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
    // ステージタイマーの表示（右上）

    DrawFormatString(0, 20, GetColor(255, 255, 255), "Time: %.1f", stage_timer);

    if (is_clear)
    {
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "NEXT STAGE...", GetColor(255, 255, 255));
    }
    else if (is_over)
    {
        DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "ゲームオーバー", GetColor(255, 255, 255));
    }

    // 操作説明パネル
    int panel_x = D_WIN_MAX_X - 270;
    int panel_y = 60;
    int panel_w = 250;
    int panel_h = 180;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(panel_x, panel_y, panel_x + panel_w, panel_y + panel_h, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

    DrawFadeOverlay();
}

bool Stage2::IsFinished()
{
    return finished;
}
bool Stage2::IsClear()
{
    return is_clear;
}
bool Stage2::IsOver()
{
    return is_over;
}

StageBase* Stage2::GetNextStage(Player* player)
{
    return new Stage3(player);
}

void Stage2::EnemyAppearance(float delta)
{
    enemy_spawn_timer += delta;
    const float spawn_interval = 2.5f;
    const int MAX_ENEMIES_ON_SCREEN = 20;

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    if (enemy_list.size() >= MAX_ENEMIES_ON_SCREEN)
        return;

    if (stage_timer < 15.0f)
    {
        if (enemy_spawn_timer >= spawn_interval)
        {
            enemy_spawn_timer = 0.0f;

            float base_x = (std::rand() % 2 == 0) ? 400.0f : 900.0f;
            float base_y = 0.0f;
            float offset_x = 50.0f;
            float offset_y = 30.0f;

            Zako* zako_top = objm->CreateObject<Zako>(Vector2D(base_x, base_y - offset_y));
            zako_top->SetPattern(ZakoPattern::MoveStraight);
            enemy_list.push_back(zako_top);

            Zako* zako_left = objm->CreateObject<Zako>(Vector2D(base_x - offset_x, base_y + offset_y));
            zako_left->SetPattern(ZakoPattern::MoveStraight);
            enemy_list.push_back(zako_left);

            Zako* zako_right = objm->CreateObject<Zako>(Vector2D(base_x + offset_x, base_y + offset_y));
            zako_right->SetPattern(ZakoPattern::MoveStraight);
            enemy_list.push_back(zako_right);
        }
    }
    else if (stage_timer < 30.0f)
    {
        if (enemy_spawn_timer >= spawn_interval)
        {
            enemy_spawn_timer = 0.0f;
            int padding = 100;
            float random_x = static_cast<float>(padding + (std::rand() % (D_WIN_MAX_X - padding * 2)));
            float random_y = static_cast<float>((std::rand() % (D_WIN_MAX_Y / 2)) * 0.8f);  // 少し抑えめ

            Zako* zako = objm->CreateObject<Zako>(Vector2D(random_x, random_y));
            zako->SetPattern(ZakoPattern::DepthAppear);
            enemy_list.push_back(zako);
        }
    }
    else if (stage_timer < 60.0f && !zako5_spawned)
    {
        Zako5* left = objm->CreateObject<Zako5>(Vector2D(420, 100));
        Zako5* right = objm->CreateObject<Zako5>(Vector2D(850, 100));
        left->SetPlayer(player);
        right->SetPlayer(player);
        enemy_list.push_back(left);
        enemy_list.push_back(right);
        zako5_spawned = true;
    }
}

void Stage2::DrawScrollBackground() const
{
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(20, 20, 40), TRUE);

    const int grid_size_back = 40;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, 100, 255));
    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back)
    {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(0, 100, 255));
    }

    const int grid_size_front = 80;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(180, 0, 255), TRUE);
    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front)
    {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(180, 0, 255), TRUE);
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
