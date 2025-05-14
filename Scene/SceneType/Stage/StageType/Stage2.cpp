#include "Stage3.h"
#include "Stage2.h"
#include "Stage1.h" // 次のステージがある場合

#include "../../../../Object/Character/Shot/Shot.h"
#include "../../../../Object/Character/Shot/Beam.h"

#include "../../../../Object/GameObjectManager.h"

#include "../../../../Object/Character/Enemy/EnemyType/Zako1.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako2.h"
#include "../../../../Object/Character/Enemy/EnemyType/Zako4.h"

#include <cstdlib> 

Stage2::Stage2(Player* player) : StageBase(player) {}
Stage2::~Stage2() {}

void Stage2::Initialize() 
{
    // 初期化処理

}

void Stage2::Finalize() 
{
    // 終了処理
        // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Finalize();

}

void Stage2::Update(float delta) 
{
    // オブジェクト管理クラスのインスタンスを取得
    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Update(delta);
    EnemyAppearance(delta);

    UpdateBackgroundScroll(delta);

    stage_timer += delta; 

    // プレイヤーが弾を打つ準備ができていたら弾を生成        
    if (player->GetIsShot() == true)
    {
        Vector2D p_location = player->GetLocation();
        player->SetIsShot();
        // 上下反転していなかったら下方向に生成
        if (player->GetShotFlip() == false)
        {
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x - 10, p_location.y - D_OBJECT_SIZE));
            shot = objm->CreateObject<Shot>(Vector2D(p_location.x + 10, p_location.y - D_OBJECT_SIZE));
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


    // 仮の条件：スペースキーを押したらステージ終了
    if (CheckHitKey(KEY_INPUT_N)) 
    {
        finished = true;
    }
    // 仮の条件：スペースキーを押したらステージ終了
    if (CheckHitKey(KEY_INPUT_U)) 
    {
        is_clear = true;
    }
    // 仮の条件：スペースキーを押したらステージ終了
    if (CheckHitKey(KEY_INPUT_I)) 
    {
        is_over = true;
    }
}

void Stage2::Draw() 
{
    // 背景を白で塗る（最初に描画）
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(255, 255, 255), TRUE);

    DrawScrollBackground();  

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
    objm->Draw();

    // 左の黒帯
    DrawBox(0, 0, (D_WIN_MAX_X / 2) - 350, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    // 右の黒帯
    DrawBox((D_WIN_MAX_X / 2) + 350, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(0, 0, 0), TRUE);

    DrawString(0, 0, "ゲームメイン", GetColor(255, 255, 255));
    DrawString(0, 300, "操作方法\n\n左スティック\n十字ボタン\nWASDキー : 移動\n\nAボタン\nスペースキー : 発射\n\nBボタン\nBキー : レーザー\n\nRBボタン\nLキー : 射出反転", GetColor(255, 255, 255));
    DrawFormatString(0, 20, GetColor(255, 255, 0), "敵数: %d", enemy_list.size());
    // ステージタイマーの表示（右上）
    DrawFormatString(D_WIN_MAX_X - 200, 20, GetColor(255, 255, 255), "Time: %.1f", stage_timer);

    if (is_clear)
    {
        DrawString((D_WIN_MAX_X / 2) - 40, (D_WIN_MAX_Y / 2) - 100, "ゲームクリア", GetColor(0, 0, 0));
    }
    else if (is_over)
    {
        DrawString((D_WIN_MAX_X / 2) - 60, (D_WIN_MAX_Y / 2) - 100, "ゲームオーバー", GetColor(0, 0, 0));
    }
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
    return new Stage3(player); // 次のステージへ
}
void Stage2::EnemyAppearance(float delta)
{
    // 1フレームごとに加算（60FPS想定）
    enemy_spawn_timer += delta;

    GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

    // 一定間隔ごと（5秒ごと）にZako2出現
    const float spawn_interval = 1.0f; // 出現間隔

    if (enemy_spawn_timer >= spawn_interval)
    {
        Vector2D spawn_pos(400.0f, 100.0f);
        zako2 = objm->CreateObject<Zako2>(spawn_pos);
        zako2->SetPlayer(player);
        enemy_spawn_timer = 0.0f; // タイマーリセット
        enemy_list.push_back(zako2);
    }

}

//スクロール描画
void Stage2::DrawScrollBackground() const
{
    // 背景色（明るめの紺）
    DrawBox(0, 0, D_WIN_MAX_X, D_WIN_MAX_Y, GetColor(20, 20, 40), TRUE);

    // 背面グリッド（遠く・細かく・線細く）
    const int grid_size_back = 40;
    const int alpha_back = 100;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_back);

    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_back)
        DrawLine(x, 0, x, D_WIN_MAX_Y, GetColor(0, 100, 255)); // ネオンブルー（薄）

    //画像だとここを変更
    for (int y = -grid_size_back; y < D_WIN_MAX_Y + grid_size_back; y += grid_size_back)
    {
        int sy = y - static_cast<int>(bg_scroll_offset_layer1) % grid_size_back;
        DrawLine(0, sy, D_WIN_MAX_X, sy, GetColor(0, 100, 255));
    }

    /**     二枚目     **/

    // 手前グリッド（近く・広め・線太め）
    const int grid_size_front = 80;
    const int alpha_front = 120;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_front);

    // 太いライン用の四角（縦ライン）
    for (int x = 0; x < D_WIN_MAX_X; x += grid_size_front)
        DrawBox(x - 1, 0, x + 1, D_WIN_MAX_Y, GetColor(180, 0, 255), TRUE); // 手前は太めパープル

    //画像だとここを変更
    for (int y = -grid_size_front; y < D_WIN_MAX_Y + grid_size_front; y += grid_size_front) {
        int sy = y - static_cast<int>(bg_scroll_offset_layer2) % grid_size_front;
        DrawBox(0, sy - 1, D_WIN_MAX_X, sy + 1, GetColor(180, 0, 255), TRUE); // 太線で遠近感
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}
