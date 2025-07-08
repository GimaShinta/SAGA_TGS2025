#include "AnimationManager.h"
#include "ResourceManager.h"
#include <DxLib.h>
#include <algorithm>

void AnimationManager::LoadAllEffects()
{
	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();

	effect_images[EffectName::eExprotion] = rm->GetImages("Resource/Image/Effect/E_Explosion.png", 54, 9, 6, 517, 517);
	effect_images[EffectName::eExprotion2] = rm->GetImages("Resource/Image/Effect/E_Explosion2.png", 54, 9, 6, 355, 365);

	std::vector<int> charge = rm->GetImages("Resource/Image/Effect/E_Charged.png", 80, 10, 8, 357, 295);
	std::vector<int> charge_2;

	for (int i = 0; i < 25; i++)
	{
		charge_2.push_back(charge[i]);
	}

	effect_images[EffectName::eChenge] = charge_2;
}

void AnimationManager::LoadSE()
{
	WarmUpSE();
	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();

	se_handles[SE_NAME::Shot] = rm->GetSounds("Resource/sound/se/shot/shot_02.mp3");
	se_handles[SE_NAME::Destroy] = rm->GetSounds("Resource/sound/se/se_effect/kill_4.mp3");
	se_handles[SE_NAME::Hit] = rm->GetSounds("Resource/sound/se/se_effect/hit_se.mp3");
	//se_handles[SE_NAME::Reaction] = rm->GetSounds("Resource/sound/se/se_effect/reaction.mp3");
	//se_handles[SE_NAME::Explosion] = rm->GetSounds("Resource/sound/se/se_effect/explosion.mp3"); // 仮に追加

	se_handles[SE_NAME::Bakuhatu] = rm->GetSounds("Resource/sound/se/battle/bakuhatu_b.mp3");
	se_handles[SE_NAME::Kill] = rm->GetSounds("Resource/sound/se/boss_se/boss_kill.mp3");
	se_handles[SE_NAME::Bakuhatu_End] = rm->GetSounds("Resource/sound/se/boss_se/bakuhatu_end.mp3");
	se_handles[SE_NAME::Hamon] = rm->GetSounds("Resource/sound/se/boss_se/hamon.mp3");
	se_handles[SE_NAME::EnemyShot] = rm->GetSounds("Resource/sound/se/shot/shot_01.mp3");
	se_handles[SE_NAME::EnemyBeam] = rm->GetSounds("Resource/sound/se/effect/audiostock_1553653.mp3");
	//ChangeVolumeSoundMem(255 * 100 / 100, se_handles[SE_NAME::Bakuhatu]);
	//ChangeVolumeSoundMem(255 * 100 / 100, se_handles[SE_NAME::Kill]);
	//ChangeVolumeSoundMem(255 * 100 / 100, se_handles[SE_NAME::Bakuhatu_End]);

}

void AnimationManager::UnloadSE()
{
	for (auto& pair : se_handles)
	{
		if (pair.second != -1) // -1 は無効なハンドル
		{
			DeleteSoundMem(pair.second);
			pair.second = -1; // 再利用・再解放防止のため無効化
		}
	}
}

void AnimationManager::WarmUpSE()
{
	for (auto it = se_handles.begin(); it != se_handles.end(); ++it)
	{
		SE_NAME name = it->first;
		int handle = it->second;

		PlaySoundMem(handle, DX_PLAYTYPE_BACK);
		StopSoundMem(handle);
	}

//#ifndef NDEBUG
//	printfDx("SEウォームアップ完了（%d種）\n", (int)se_handles.size());
//#endif

}

void AnimationManager::PlaySE(SE_NAME name)
{
	if (se_handles.count(name))
	{
		PlaySoundMem(se_handles[name], DX_PLAYTYPE_BACK);
	}
#ifndef NDEBUG
	else
	{
		printfDx("未登録のSEが呼ばれました: %d\n", static_cast<int>(name));
	}
#endif
}

void AnimationManager::ChangeSEVolume(SE_NAME se, int volume)
{
	// volume: 0?100 の範囲を想定（DxLib内部は 0?255）

	if (se_handles.count(se) == 0) return; // 存在確認
	int handle = se_handles[se];

	if (handle != -1)
	{
		int dx_volume = Clamp(volume * 255 / 100, 0, 255); // 変換＋制限
		ChangeVolumeSoundMem(dx_volume, handle);
	}
}

// アニメーションを再生する
AnimationID AnimationManager::PlayerAnimation(const std::vector<int>& image_handles, const Vector2D& position, float frame_time_sec, bool loop)
{
	AnimationID id = next_id++;  // 一意のIDを発行
	animations[id] = std::make_unique<Animation>(id, image_handles, position, frame_time_sec, loop);
	return id;
}

AnimationID AnimationManager::PlayerAnimation(EffectName effect_name, const Vector2D& position, float frame_time_sec, bool loop)
{
	auto it = effect_images.find(effect_name);

	// ★防御コード：未登録 or 空 vector を検出
	if (it == effect_images.end() || it->second.empty())
	{
		// デバッグ時の通知
#ifndef NDEBUG
		printfDx("Error: エフェクトがロードされていない (%d)\n", effect_name);
#endif

		return -1;  // 無効なIDを返して呼び出し元で無視させる
	}

	const std::vector<int>& handles = it->second;

	AnimationID id = next_id++;
	animations[id] = std::make_unique<Animation>(id, handles, position,
		frame_time_sec, loop);
	return id;
}

// アニメーションの更新処理
void AnimationManager::Update(const float& delta_second)
{
	for (auto it = animations.begin(); it != animations.end(); )
	{
		Animation* anim = it->second.get();

		// 停止中 or 一時停止中のアニメーションはスキップ
		if (anim->is_paused)
		{
			++it;
			continue;
		}

		// 時間経過でフレーム進行
		anim->elapsed_time += delta_second;
		if (anim->elapsed_time >= anim->frame_time)
		{
			anim->elapsed_time -= anim->frame_time;
			anim->current_frame++;

			if (anim->current_frame >= static_cast<int>(anim->image_handles.size()))
			{
				if (anim->loop)
				{
					anim->current_frame = 0;  // ループ再生
				}
				else
				{
					anim->current_frame = static_cast<int>(anim->image_handles.size()) - 1;
					anim->is_finished = true;  // 再生完了
				}
			}
		}

		// 再生完了なら削除
		if (anim->is_finished)
		{
			it = animations.erase(it);
		}
		else
		{
			++it;
		}
	}
}

// アニメーションの描画処理
void AnimationManager::Draw()
{
	std::vector<Animation*> draw_list;

	// 一時停止していないアニメーションだけを描画対象にする
	for (const auto& pair : animations)
	{
		if (!pair.second->is_paused)
		{
			draw_list.push_back(pair.second.get());
		}
	}

	// レイヤー順にソート（値が大きいほど前面）
	std::sort(draw_list.begin(), draw_list.end(), [](Animation* a, Animation* b)
		{
			return a->z_layer < b->z_layer;
		});

	// 描画処理
	for (auto* anim : draw_list)
	{
		int handle = anim->image_handles[anim->current_frame];

		// 透明度を適用
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, anim->alpha);

		// 拡大率と回転角（ここでは 0）を指定して描画
		DrawRotaGraphF(anim->position.x, anim->position.y, anim->scale, 0.0f, handle, true);

		// ブレンドモードをリセット
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

// アニメーションの全削除（終了処理）
void AnimationManager::Finalize()
{
	animations.clear();
}

// 指定IDのアニメーションが終了しているかどうか
bool AnimationManager::GetAnimationFinished(AnimationID id) const
{
	auto it = animations.find(id);
	if (it != animations.end())
	{
		return it->second->is_finished;
	}
	return false;
}

// 指定IDのアニメーションを削除
void AnimationManager::RemoveAnimation(AnimationID id)
{
	animations.erase(id);
}

// α値（透明度）を設定（0～255の範囲に制限）
void AnimationManager::SetAlpha(AnimationID id, int alpha)
{
	if (animations.count(id))
	{
		if (alpha < 0) alpha = 0;
		else if (alpha > 255) alpha = 255;

		animations[id]->alpha = alpha;
	}
}

// スケーリング（拡大率）を設定（例：1.0f で等倍）
void AnimationManager::SetScale(AnimationID id, float scale)
{
	if (animations.count(id))
	{
		animations[id]->scale = scale;
	}
}

// 一時停止
void AnimationManager::PauseAnimation(AnimationID id)
{
	if (animations.count(id))
	{
		animations[id]->is_paused = true;
	}
}

// 再開
void AnimationManager::ResumeAnimation(AnimationID id)
{
	if (animations.count(id))
	{
		animations[id]->is_paused = false;
	}
}

// レイヤー（描画順）を設定
void AnimationManager::SetZLayer(AnimationID id, int z)
{
	if (animations.count(id))
	{
		animations[id]->z_layer = z;  // ← 修正済み：代入漏れを修正
	}
}

// 位置情報の設定
void AnimationManager::SetPosition(AnimationID id, const Vector2D& posi)
{
	if (animations.count(id))
	{
		animations[id]->position = posi;
	}
}
