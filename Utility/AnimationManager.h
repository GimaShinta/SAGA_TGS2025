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
	// 必要に応じて追加
};

// AnimationID を int型と同等にする
using AnimationID = int;

// アニメーション制御クラス
class AnimationManager : public Singleton<AnimationManager>
{
private:
	// 各アニメーションに項目設定
	struct Animation
	{
		AnimationID id;                    // アニメーションを区別するID
		std::vector<int> image_handles;    // アニメーションの画像（）複数でも単体でも
		Vector2D position;                 // 描画位置
		float elapsed_time = 0.0f;         // 累積時間
		float frame_time = 0.1f;           // 各フレームを何秒表示するか
		int current_frame = 0;             // 現在表示しているフレーム番号
		bool loop = false;                 // アニメーションをループさせるか
		bool is_paused = false;            // 一時停止させるかどうか
		bool is_finished = false;          // 終了させるかどうか

		int alpha = 255;                   // 透明度
		float scale = 1.0f;  // 拡大率
		int z_layer = 0;                       // レイヤー

		// 初期化処理
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
	/// アニメーション再生
	/// </summary>
	/// <param name="imageHandles">アニメーション画像</param>
	/// <param name="position">再生位置</param>
	/// <param name="frame_time_sec">再生速度</param>
	/// <param name="loop">ループ再生するかどうか</param>
	/// <returns></returns>
	AnimationID PlayerAnimation(const std::vector<int>& imageHandles, const Vector2D& position,
		float frame_time_sec, bool loop);
	AnimationID PlayerAnimation(EffectName effect_name, const Vector2D& position,
		float frame_time_sec, bool loop);

	// 更新処理
	void Update(const float& delta_second);

	// 描画処理
	void Draw();

	// 終了時処理
	void Finalize();

public:
	// 指定IDのアニメーションが終了しているかどうかを返す
	bool GetAnimationFinished(AnimationID id) const;

	// 指定IDのアニメーションを即削除
	void RemoveAnimation(AnimationID id);

	// α値の設定（０～２５５）
	void SetAlpha(AnimationID id, int alpha);

	// 拡大率設定（1.0　＝　等倍）
	void SetScale(AnimationID id, float scale);

	// 一時停止
	void PauseAnimation(AnimationID id);

	// 再開
	void ResumeAnimation(AnimationID id);

	// レイヤーの設定
	void SetZLayer(AnimationID id, int z);

	// 位置情報の設定
	void SetPosition(AnimationID id, const Vector2D& posi);

private:
	// 次に再生するID番号
	AnimationID next_id = 0;

	// IDとアニメーションの対応マップ
	std::unordered_map<AnimationID, std::unique_ptr<Animation>> animations;

	std::map<int, std::vector<int>> effect_images;

private:
	std::map<SE_NAME, int> se_handles; // SEのハンドルマップ

	float Clamp(float value, float min, float max)
	{
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

};