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
	//se_handles[SE_NAME::Explosion] = rm->GetSounds("Resource/sound/se/se_effect/explosion.mp3"); // ���ɒǉ�

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
		if (pair.second != -1) // -1 �͖����ȃn���h��
		{
			DeleteSoundMem(pair.second);
			pair.second = -1; // �ė��p�E�ĉ���h�~�̂��ߖ�����
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
//	printfDx("SE�E�H�[���A�b�v�����i%d��j\n", (int)se_handles.size());
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
		printfDx("���o�^��SE���Ă΂�܂���: %d\n", static_cast<int>(name));
	}
#endif
}

void AnimationManager::ChangeSEVolume(SE_NAME se, int volume)
{
	// volume: 0?100 �͈̔͂�z��iDxLib������ 0?255�j

	if (se_handles.count(se) == 0) return; // ���݊m�F
	int handle = se_handles[se];

	if (handle != -1)
	{
		int dx_volume = Clamp(volume * 255 / 100, 0, 255); // �ϊ��{����
		ChangeVolumeSoundMem(dx_volume, handle);
	}
}

// �A�j���[�V�������Đ�����
AnimationID AnimationManager::PlayerAnimation(const std::vector<int>& image_handles, const Vector2D& position, float frame_time_sec, bool loop)
{
	AnimationID id = next_id++;  // ��ӂ�ID�𔭍s
	animations[id] = std::make_unique<Animation>(id, image_handles, position, frame_time_sec, loop);
	return id;
}

AnimationID AnimationManager::PlayerAnimation(EffectName effect_name, const Vector2D& position, float frame_time_sec, bool loop)
{
	auto it = effect_images.find(effect_name);

	// ���h��R�[�h�F���o�^ or �� vector �����o
	if (it == effect_images.end() || it->second.empty())
	{
		// �f�o�b�O���̒ʒm
#ifndef NDEBUG
		printfDx("Error: �G�t�F�N�g�����[�h����Ă��Ȃ� (%d)\n", effect_name);
#endif

		return -1;  // ������ID��Ԃ��ČĂяo�����Ŗ���������
	}

	const std::vector<int>& handles = it->second;

	AnimationID id = next_id++;
	animations[id] = std::make_unique<Animation>(id, handles, position,
		frame_time_sec, loop);
	return id;
}

// �A�j���[�V�����̍X�V����
void AnimationManager::Update(const float& delta_second)
{
	for (auto it = animations.begin(); it != animations.end(); )
	{
		Animation* anim = it->second.get();

		// ��~�� or �ꎞ��~���̃A�j���[�V�����̓X�L�b�v
		if (anim->is_paused)
		{
			++it;
			continue;
		}

		// ���Ԍo�߂Ńt���[���i�s
		anim->elapsed_time += delta_second;
		if (anim->elapsed_time >= anim->frame_time)
		{
			anim->elapsed_time -= anim->frame_time;
			anim->current_frame++;

			if (anim->current_frame >= static_cast<int>(anim->image_handles.size()))
			{
				if (anim->loop)
				{
					anim->current_frame = 0;  // ���[�v�Đ�
				}
				else
				{
					anim->current_frame = static_cast<int>(anim->image_handles.size()) - 1;
					anim->is_finished = true;  // �Đ�����
				}
			}
		}

		// �Đ������Ȃ�폜
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

// �A�j���[�V�����̕`�揈��
void AnimationManager::Draw()
{
	std::vector<Animation*> draw_list;

	// �ꎞ��~���Ă��Ȃ��A�j���[�V����������`��Ώۂɂ���
	for (const auto& pair : animations)
	{
		if (!pair.second->is_paused)
		{
			draw_list.push_back(pair.second.get());
		}
	}

	// ���C���[���Ƀ\�[�g�i�l���傫���قǑO�ʁj
	std::sort(draw_list.begin(), draw_list.end(), [](Animation* a, Animation* b)
		{
			return a->z_layer < b->z_layer;
		});

	// �`�揈��
	for (auto* anim : draw_list)
	{
		int handle = anim->image_handles[anim->current_frame];

		// �����x��K�p
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, anim->alpha);

		// �g�嗦�Ɖ�]�p�i�����ł� 0�j���w�肵�ĕ`��
		DrawRotaGraphF(anim->position.x, anim->position.y, anim->scale, 0.0f, handle, true);

		// �u�����h���[�h�����Z�b�g
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

// �A�j���[�V�����̑S�폜�i�I�������j
void AnimationManager::Finalize()
{
	animations.clear();
}

// �w��ID�̃A�j���[�V�������I�����Ă��邩�ǂ���
bool AnimationManager::GetAnimationFinished(AnimationID id) const
{
	auto it = animations.find(id);
	if (it != animations.end())
	{
		return it->second->is_finished;
	}
	return false;
}

// �w��ID�̃A�j���[�V�������폜
void AnimationManager::RemoveAnimation(AnimationID id)
{
	animations.erase(id);
}

// ���l�i�����x�j��ݒ�i0�`255�͈̔͂ɐ����j
void AnimationManager::SetAlpha(AnimationID id, int alpha)
{
	if (animations.count(id))
	{
		if (alpha < 0) alpha = 0;
		else if (alpha > 255) alpha = 255;

		animations[id]->alpha = alpha;
	}
}

// �X�P�[�����O�i�g�嗦�j��ݒ�i��F1.0f �œ��{�j
void AnimationManager::SetScale(AnimationID id, float scale)
{
	if (animations.count(id))
	{
		animations[id]->scale = scale;
	}
}

// �ꎞ��~
void AnimationManager::PauseAnimation(AnimationID id)
{
	if (animations.count(id))
	{
		animations[id]->is_paused = true;
	}
}

// �ĊJ
void AnimationManager::ResumeAnimation(AnimationID id)
{
	if (animations.count(id))
	{
		animations[id]->is_paused = false;
	}
}

// ���C���[�i�`�揇�j��ݒ�
void AnimationManager::SetZLayer(AnimationID id, int z)
{
	if (animations.count(id))
	{
		animations[id]->z_layer = z;  // �� �C���ς݁F����R����C��
	}
}

// �ʒu���̐ݒ�
void AnimationManager::SetPosition(AnimationID id, const Vector2D& posi)
{
	if (animations.count(id))
	{
		animations[id]->position = posi;
	}
}
