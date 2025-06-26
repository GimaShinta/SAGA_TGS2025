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
	// 初期化処理
// 【記述位置は仮】ステージの長さを代入
	distance = STAGE3_DISTANCE;
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) + 100.0f));
    objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) + 50.0f));

	//フォント
// font_digital = CreateFontToHandle("DS-Digital", 28, 6, DX_FONTTYPE_ANTIALIASING);
	font_digital = CreateFontToHandle("Orbitron", 28, 6, DX_FONTTYPE_ANTIALIASING);
	font_orbitron = CreateFontToHandle("Orbitron", 22, 6, DX_FONTTYPE_ANTIALIASING);

    stage_id = StageID::Stage4;
}

void Stage4::Finalize()
{
	// 終了処理
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
	objm->Finalize();
	AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
	manager->RemoveAnimation(anim_id);

}

void Stage4::Update(float delta)
{
	UpdateBackgroundScroll(delta);

    // delta_second 分加算
    stage_timer += delta;

    //// 一定時間ごとに distance を減らす（例：0.1秒ごとに1減らす）
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

    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);

    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();
    // 毎フレーム
    manager->Update(delta);       // 更新（deltaTimeは前のフレームとの経過時間）

    // アニメーションが終了したか確認（削除が必要な場合）
    if (manager->GetAnimationFinished(anim_id) == true)
    {
        manager->RemoveAnimation(anim_id);  // 不要になったら削除
    }

    // プレイヤーの攻撃
    PlayerShot();

    // 敵の出現
    EnemyAppearance();

    // 敵の攻撃
    EnemyShot(delta);

    // 警告表示
    DisplayWarning(delta);

    // クリア判定
    UpdateGameStatus(delta);
}

void Stage4::Draw()
{
    // 背景を白で塗る（最初に描画）
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    //スクロール背景
    DrawScrollBackground();

    // 左の黒帯
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // 右の黒帯
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "Stage4", GetColor(255, 255, 255));
    DrawFormatString(0, 20, GetColor(255, 255, 0), "敵数: %d", enemy_list.size());

    // ステージ描画
    if (stage <= 1)
    {
        if (is_warning == true)
        {
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, brend);
            DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(255, 0, 0), TRUE);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        }
    }

    // --- 描画順を制御 ---
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    AnimationManager* manager = Singleton<AnimationManager>::GetInstance();


    if (draw_animation_first)  // ← このフラグで順序を切り替える
    {
        manager->Draw();       // 先にアニメーション
        objm->Draw();          // 後にオブジェクト
    }
    else
    {
        objm->Draw();          // 先にオブジェクト
        manager->Draw();       // 後にアニメーション
    }

    if (is_clear == true)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
        DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawFormatStringToHandle((D_WIN_MAX_X / 2) - 100.0f, (D_WIN_MAX_Y / 2), GetColor(255, 255, 255), font_digital, "STAGE CLEAR");

        //DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "ゲームクリア", GetColor(0, 0, 0));
    }
    else if (is_over == true)
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, transparent);
        DrawBox((D_WIN_MAX_X / 2) - 350, 0, (D_WIN_MAX_X / 2) + 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
        DrawFormatStringToHandle((D_WIN_MAX_X / 2) - 100.0f, (D_WIN_MAX_Y / 2), GetColor(255, 255, 255), font_digital, "GAME OVER");

        //DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "ゲームオーバー", GetColor(0, 0, 0));
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
    // 警告表示（赤く光る）
    if (is_warning == true)
    {
        // カウント加算
        warning_timer += delta_second;
        if (warning_timer >= 0.0005f)
        {
            // 透明度を０から２５５まで加算減算を繰り返す
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
                    // 到達カウントを加算
                    reach_count++;
                    is_brend = false;
                }
            }
            // カウントリセット
            warning_timer = 0;
        }
        // ３回警告が終わったら全てリセット
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
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // プレイヤーが弾を打つ準備ができていたら弾を生成        
    if (player->GetIsShot() == true)
    {
        Vector2D p_location = player->GetLocation();
        player->SetIsShot();
        // 上下反転していなかったら下方向に生成
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
            else
            {
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 50, p_location.y + D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 30, p_location.y));
                shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 50, p_location.y + D_OBJECT_SIZE));

            }
            shot->SetShotFlip(false);
        }
        // 反転していたら上方向に生成
        else
        {
            Vector2D p_location = player->GetLocation();
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x, p_location.y + D_OBJECT_SIZE));
            shot->SetShotFlip(true);
        }
    }
    // プレイヤーがビームをうつ準備ができていたらビームを生成
    else if (player->GetBeamOn() == true)
    {
        Vector2D p_location = player->GetLocation();
        // 上下反転していなかったら下方向に生成
        if (player->GetShotFlip() == false)
        {
            beam = objm->CreateObject<Beam>(Vector2D(p_location.x, (p_location.y - D_OBJECT_SIZE) - 848));
            beam->SetBeamFlip(false);
        }
        // 反転していたら上方向に生成
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
        objm->CreateObject<PowerUp>(Vector2D(D_WIN_MAX_X / 2 - 60, 120))->SetPlayer(player);
        objm->CreateObject<Shield>(Vector2D(D_WIN_MAX_X / 2 + 60, 120))->SetPlayer(player);

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
    //// ボスが倒れたらクリア
    //if (boss != nullptr && boss->GetIsAlive() == false && is_over == false)
    //{
    //    boss->SetDestroy();
    //    is_clear = true;
    //}

    if (boss3 != nullptr && boss3->GetIsCrashing() == true)
    {
        draw_animation_first = true;
    }


    // ボス２が倒れたらクリア
    if (boss3 != nullptr && boss3->GetIsAlive() == false && is_over == false)
    {
        boss3->SetDestroy();
        is_clear = true;
    }

    // プレイヤーが倒れたらゲームオーバー
    if (player != nullptr && player->GetIsAlive() == false && is_clear == false)
    {
        // オブジェクト管理クラスのインスタンスを取得
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

        if (scene_timer >= 4.0f)
        {
            finished = true;
        }
    }

    // 仮の条件：スペースキーを押したらステージ終了
    if (CheckHitKey(KEY_INPUT_N))
    {
        finished = true;
    }

}

void Stage4::DrawScrollBackground() const
{
    static float time = 0.0f;
    time += 0.05f;

    // 背景色：やや赤みがかったグレー
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(30, 10, 10), TRUE);

    // === 背面レイヤー（奥・細かく・暗め） ===
    const int grid_size_back = 40;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(100, 0, 0));

    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(100, 0, 0));
    }

    // === 前面レイヤー（太く・明るく・警告感） ===
    const int grid_size_front = 80;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(255, 40, 40), TRUE);

    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(255, 40, 40), TRUE);
    }

    // === ノイズ演出（崩壊感） ===
    for (int i = 0; i < 5; ++i) {
        if (rand() % 70 == 0) {
            int nx = rand() % D_WIN_MAX_X;
            int ny = rand() % D_WIN_MAX_Y;
            DrawBox(nx, ny, nx + 3, ny + 3, GetColor(255, 100, 50), TRUE); // オレンジ警告ドット
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

}
