#pragma once
#include "../Singleton.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>
#include "Vector2D.h"
#include <algorithm>

enum EffectName
{
	eExprotion = 0,
	eExprotion2,
	eChenge,
	eExprotion3,
};

enum class SE_NAME
{
	Shot,
	Reaction,
	Explosion,
	Bakuhatu,
	Bakuhatu_End,
	Kill,
	Destroy,
	Hit,
	EnemyShot,
	Hamon,
	EnemyBeam
	// �K�v�ɉ����Ēǉ�
};

// AnimationID �� int�^�Ɠ����ɂ���
using AnimationID = int;

// �A�j���[�V��������N���X
class AnimationManager : public Singleton<AnimationManager>
{
private:
	// �e�A�j���[�V�����ɍ��ڐݒ�
	struct Animation
	{
		AnimationID id;                    // �A�j���[�V��������ʂ���ID
		std::vector<int> image_handles;    // �A�j���[�V�����̉摜�i�j�����ł��P�̂ł�
		Vector2D position;                 // �`��ʒu
		float elapsed_time = 0.0f;         // �ݐώ���
		float frame_time = 0.1f;           // �e�t���[�������b�\�����邩
		int current_frame = 0;             // ���ݕ\�����Ă���t���[���ԍ�
		bool loop = false;                 // �A�j���[�V���������[�v�����邩
		bool is_paused = false;            // �ꎞ��~�����邩�ǂ���
		bool is_finished = false;          // �I�������邩�ǂ���

		int alpha = 255;                   // �����x
		float scale = 1.0f;  // �g�嗦
		int z_layer = 0;                       // ���C���[

		// ����������
		Animation(AnimationID id_, const std::vector<int>& handles, const Vector2D& pos,
			float frame_time_sec, bool loop_flag)
			: id(id_), image_handles(handles), position(pos), frame_time(frame_time_sec), loop(loop_flag) {}
	};



public:
	AnimationManager() = default;

	void LoadAllEffects();
	void LoadSE();
	void WarmUpSE();
	void PlaySE(SE_NAME name);
	void UnloadSE();
	void ChangeSEVolume(SE_NAME se, int volume);
	/// <summary>
	/// �A�j���[�V�����Đ�
	/// </summary>
	/// <param name="imageHandles">�A�j���[�V�����摜</param>
	/// <param name="position">�Đ��ʒu</param>
	/// <param name="frame_time_sec">�Đ����x</param>
	/// <param name="loop">���[�v�Đ����邩�ǂ���</param>
	/// <returns></returns>
	AnimationID PlayerAnimation(const std::vector<int>& imageHandles, const Vector2D& position,
		float frame_time_sec, bool loop);
	AnimationID PlayerAnimation(EffectName effect_name, const Vector2D& position,
		float frame_time_sec, bool loop);

	// �X�V����
	void Update(const float& delta_second);

	// �`�揈��
	void Draw();

	// �I��������
	void Finalize();

public:
	// �w��ID�̃A�j���[�V�������I�����Ă��邩�ǂ�����Ԃ�
	bool GetAnimationFinished(AnimationID id) const;

	// �w��ID�̃A�j���[�V�����𑦍폜
	void RemoveAnimation(AnimationID id);

	// ���l�̐ݒ�i�O�`�Q�T�T�j
	void SetAlpha(AnimationID id, int alpha);

	// �g�嗦�ݒ�i1.0�@���@���{�j
	void SetScale(AnimationID id, float scale);

	// �ꎞ��~
	void PauseAnimation(AnimationID id);

	// �ĊJ
	void ResumeAnimation(AnimationID id);

	// ���C���[�̐ݒ�
	void SetZLayer(AnimationID id, int z);

	// �ʒu���̐ݒ�
	void SetPosition(AnimationID id, const Vector2D& posi);

private:
	// ���ɍĐ�����ID�ԍ�
	AnimationID next_id = 0;

	// ID�ƃA�j���[�V�����̑Ή��}�b�v
	std::unordered_map<AnimationID, std::unique_ptr<Animation>> animations;

	std::map<int, std::vector<int>> effect_images;

private:
	std::map<SE_NAME, int> se_handles; // SE�̃n���h���}�b�v

	float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

};