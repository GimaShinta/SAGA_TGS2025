#include "AnimationManager.h"
#include "ResourceManager.h"
#include <DxLib.h>
#include <algorithm>

void AnimationManager::LoadAllEffects()
{
	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();

	effect_images[EffectName::eExprotion] = rm->GetImages("Resource/Image/Effect/E_Explosion.png", 54, 9, 6, 517, 517);
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
