#include "AnimationManager.h"
#include <DxLib.h>
#include <algorithm>

/// <summary>
/// �A�j���[�V�����Đ�
/// </summary>
/// <param name="imageHandles">�A�j���[�V�����摜</param>
/// <param name="position">�Đ��ʒu</param>
/// <param name="frame_time_sec">�Đ����x</param>
/// <param name="loop">���[�v�Đ����邩�ǂ���</param>
/// <returns></returns>
AnimationID AnimationManager::PlayerAnimation(const std::vector<int>& image_handles, const Vector2D& position, float frame_time_sec, bool loop)
{
	// id �̍쐬
	AnimationID id = next_id++;
	// �A�j���[�V�����̐���
	animations[id] = std::make_unique<Animation>(id, image_handles, position, frame_time_sec, loop);
	return id;
}

// �X�V����
void AnimationManager::Update(const float& delta_second)
{
	// �A�j���[�V�����̐���
	for (auto it = animations.begin(); it != animations.end();)
	{
		// �|�C���^�̎擾
		Animation* anim = it->second.get();

		// �X�g�b�v������������~�߂�
		if (anim->is_finished == true || anim->is_paused == true)
		{
			++it;
			continue;
		}

		// �A�j���[�V�����̍Đ�
		anim->elapsed_time += delta_second;
		if (anim->elapsed_time >= anim->frame_time)
		{
			anim->elapsed_time -= anim->frame_time;
			anim->current_frame++;

			if (anim->current_frame >= static_cast<int>(anim->image_handles.size())) 
			{
				// ���[�v����Ȃ炻�̂܂܍Đ�
				if (anim->loop == true) 
				{
					anim->current_frame = 0;
				}
				// ���[�v���Ȃ��Ȃ�~�߂�
				else 
				{
					anim->current_frame = static_cast<int>(anim->image_handles.size()) - 1;
					anim->is_finished = true;
				}
			}
		}

		// �I�������A�j���[�V�����͍폜
		if (anim->is_finished == true) 
		{
			it = animations.erase(it);  // �폜��Ɏ��̃C�e���[�^��Ԃ�
		}
		else 
		{
			++it;  // ���̃A�j���[�V������
		}
	}
}

// �`�揈��
void AnimationManager::Draw()
{
	// Z���ŕ`�揇���\�[�g
	std::vector<Animation*> draw_list;

	for (const auto& pair : animations) 
	{
		if (!pair.second->is_paused) 
		{
			draw_list.push_back(pair.second.get());
		}

		const Animation* anim = pair.second.get();

		// �X�g�b�v�ƃ|�[�Y������������~�߂�
		if (anim->is_paused == true)
		{
			continue;
		}
	}

	// Z���Ń\�[�g�i�傫���l�قǑO�ɕ\���j
	std::sort(draw_list.begin(), draw_list.end(), [](Animation* a, Animation* b) 
	{
		return a->z_layer < b->z_layer;
	});

	// �e�A�j���[�V������`��
	for (auto* anim : draw_list) 
	{
		// �A�j���[�V�����摜��I��
		int handle = anim->image_handles[anim->current_frame];

		// �����x�ݒ�i0�`255�j
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, anim->alpha);

		// �X�P�[�����O�ƍ��W�ݒ�A�`��
		DrawRotaGraphF(anim->position.x, anim->position.y, anim->scale, 0.0f, handle, true);

		// �`���͓����x�̐ݒ�����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

// �I��������
void AnimationManager::Finalize()
{
	animations.clear();
}

// �w��ID�̃A�j���[�V�������I�����Ă��邩�ǂ�����Ԃ�
bool AnimationManager::GetAnimationFinished(AnimationID id) const
{
	auto it = animations.find(id);
	if (it != animations.end())
	{
		return it->second->is_finished;
	}
	return false;
}

// �w��ID�̃A�j���[�V�����𑦍폜
void AnimationManager::RemoveAnimation(AnimationID id)
{
	animations.erase(id);
}

// ���l�̐ݒ�i�O�`�Q�T�T�j
void AnimationManager::SetAlpha(AnimationID id, int alpha)
{
	if (animations.count(id))
	{
		animations[id]->alpha = clamp(alpha, 0, 255);
	}
}

// �g�嗦�ݒ�i1.0�@���@���{�j
void AnimationManager::SetScale(AnimationID id, float scale)
{
	if (animations.count(id))
	{
		animations[id]->scale = scale;
	}
}

// �A�j���[�V�������ꎞ��~
void AnimationManager::PauseAnimation(AnimationID id)
{
	if (animations.count(id))
	{
		animations[id]->is_paused = true;
	}
}

// �A�j���[�V�������ĊJ
void AnimationManager::ResumeAnimation(AnimationID id)
{
	if (animations.count(id))
	{
		animations[id]->is_paused = false;
	}
}

// ���C���[��ݒ�
void AnimationManager::SetZLayer(AnimationID id, int z)
{
	if (animations.count(id))
	{
		animations[id]->z_layer;
	}
}
