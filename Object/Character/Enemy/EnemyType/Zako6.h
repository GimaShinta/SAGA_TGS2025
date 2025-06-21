#pragma once
#include "../EnemyBase.h"

enum class ZakoMode {
    Zako2,
    Zako3
};

enum class ZakoState {
    AppearingFromBottom,  // Zako2スタイル
    Descending,           // Zako2スタイル
    Appearing,            // Zako3スタイル
    Floating,              // 共通
    Leaving
};

class Zako6 : public EnemyBase {
public:
    Zako6();
    ~Zako6();

    void Initialize() override;
    void Update(float delta_second) override;
    void Draw(const Vector2D& screen_offset) const override;
    void Finalize() override;

    void Shot(float delta_second) override;
    void SetFlip(bool flip);

    /// 出現アニメーション用パラメータの設定
    void SetAppearParams(const Vector2D& start_pos_param, const Vector2D& end_pos_param, float appear_time, bool from_left);

    /// Zako2 or Zako3 の動作切り替え
    void SetMode(ZakoMode mode);

private:
    ZakoMode mode;
    ZakoState state;

    float float_timer;
    float appear_timer;
    float appear_duration;
    float descend_duration;

    float scale_min;
    float scale_max;

    float alpha;
    float scale;
    float rotation;

    bool is_from_left;
    bool zako3_flip;

    Vector2D start_pos;
    Vector2D target_pos;
    Vector2D descend_pos;
    Vector2D base_location;

    int zako2_image;
    int zako3_image;

    template <typename T>
    T my_min(T a, T b) {
        return (a < b) ? a : b;
    }

};