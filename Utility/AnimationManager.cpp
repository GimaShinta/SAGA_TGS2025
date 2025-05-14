#include "AnimationManager.h"
#include <DxLib.h>
#include <algorithm>

/// <summary>
/// アニメーション再生
/// </summary>
/// <param name="imageHandles">アニメーション画像</param>
/// <param name="position">再生位置</param>
/// <param name="frame_time_sec">再生速度</param>
/// <param name="loop">ループ再生するかどうか</param>
/// <returns></returns>
AnimationID AnimationManager::PlayerAnimation(const std::vector<int>& image_handles, const Vector2D& position, float frame_time_sec, bool loop)
{
	// id の作成
	AnimationID id = next_id++;
	// アニメーションの生成
	animations[id] = std::make_unique<Animation>(id, image_handles, position, frame_time_sec, loop);
	return id;
}

// 更新処理
void AnimationManager::Update(const float& delta_second)
{
	// アニメーションの生成
	for (auto it = animations.begin(); it != animations.end();)
	{
		// ポインタの取得
		Animation* anim = it->second.get();

		// ストップがかかったら止める
		if (anim->is_finished == true || anim->is_paused == true)
		{
			++it;
			continue;
		}

		// アニメーションの再生
		anim->elapsed_time += delta_second;
		if (anim->elapsed_time >= anim->frame_time)
		{
			anim->elapsed_time -= anim->frame_time;
			anim->current_frame++;

			if (anim->current_frame >= static_cast<int>(anim->image_handles.size())) 
			{
				// ループするならそのまま再生
				if (anim->loop == true) 
				{
					anim->current_frame = 0;
				}
				// ループしないなら止める
				else 
				{
					anim->current_frame = static_cast<int>(anim->image_handles.size()) - 1;
					anim->is_finished = true;
				}
			}
		}

		// 終了したアニメーションは削除
		if (anim->is_finished == true) 
		{
			it = animations.erase(it);  // 削除後に次のイテレータを返す
		}
		else 
		{
			++it;  // 次のアニメーションへ
		}
	}
}

// 描画処理
void AnimationManager::Draw()
{
	// Z順で描画順をソート
	std::vector<Animation*> draw_list;

	for (const auto& pair : animations) 
	{
		if (!pair.second->is_paused) 
		{
			draw_list.push_back(pair.second.get());
		}

		const Animation* anim = pair.second.get();

		// ストップとポーズがかかったら止める
		if (anim->is_paused == true)
		{
			continue;
		}
	}

	// Z順でソート（大きい値ほど前に表示）
	std::sort(draw_list.begin(), draw_list.end(), [](Animation* a, Animation* b) 
	{
		return a->z_layer < b->z_layer;
	});

	// 各アニメーションを描画
	for (auto* anim : draw_list) 
	{
		// アニメーション画像を選定
		int handle = anim->image_handles[anim->current_frame];

		// 透明度設定（0～255）
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, anim->alpha);

		// スケーリングと座標設定、描画
		DrawRotaGraphF(anim->position.x, anim->position.y, anim->scale, 0.0f, handle, true);

		// 描画後は透明度の設定を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

// 終了時処理
void AnimationManager::Finalize()
{
	animations.clear();
}

// 指定IDのアニメーションが終了しているかどうかを返す
bool AnimationManager::GetAnimationFinished(AnimationID id) const
{
	auto it = animations.find(id);
	if (it != animations.end())
	{
		return it->second->is_finished;
	}
	return false;
}

// 指定IDのアニメーションを即削除
void AnimationManager::RemoveAnimation(AnimationID id)
{
	animations.erase(id);
}

// α値の設定（０～２５５）
void AnimationManager::SetAlpha(AnimationID id, int alpha)
{
	if (animations.count(id))
	{
		animations[id]->alpha = clamp(alpha, 0, 255);
	}
}

// 拡大率設定（1.0　＝　等倍）
void AnimationManager::SetScale(AnimationID id, float scale)
{
	if (animations.count(id))
	{
		animations[id]->scale = scale;
	}
}

// アニメーションを一時停止
void AnimationManager::PauseAnimation(AnimationID id)
{
	if (animations.count(id))
	{
		animations[id]->is_paused = true;
	}
}

// アニメーションを再開
void AnimationManager::ResumeAnimation(AnimationID id)
{
	if (animations.count(id))
	{
		animations[id]->is_paused = false;
	}
}

// レイヤーを設定
void AnimationManager::SetZLayer(AnimationID id, int z)
{
	if (animations.count(id))
	{
		animations[id]->z_layer;
	}
}
