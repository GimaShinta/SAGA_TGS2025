#pragma once
#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../../Object/Character/Shot/EnemyShot/EnemyShot2.h"

/// <summary>
/// Zako1�̍s���p�^�[�����`����񋓌^
/// </summary>
enum class Zako1Pattern
{
	MoveStraight, // �^�������i��
	RightMove,    // �E�Ɉړ����Ȃ���i��
	LeftMove,      // ���Ɉړ����Ȃ���i��
	ZIgzag,
	MoveAndStopShoot,   // ���ǉ�
	MoveThenDiagonal    // ���ǉ�
};

/// <summary>
/// �G�L�����N�^�[�uZako1�v�̃N���X�B
/// EnemyBase ���p�����A�Ǝ��̈ړ��E�U���E�����蔻�菈������������B
/// </summary>
class Zako1 : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Zako1();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Zako1();

public:
	/// <summary>
	/// ����������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// ���t���[���̍X�V����
	/// </summary>
	/// <param name="delta_second">�O�t���[���Ƃ̌o�ߎ���</param>
	void Update(float delta_second) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="screen_offset">��ʃX�N���[���̃I�t�Z�b�g</param>
	void Draw(const Vector2D& screen_offset) const override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

protected:
	/// <summary>
	/// �e��������
	/// </summary>
	/// <param name="delta_second">�O�t���[���Ƃ̌o�ߎ���</param>
	void Shot(float delta_second) override;

private:
	Zako1Pattern pattern = Zako1Pattern::MoveStraight;  // ���݂̍s���p�^�[��
	float pattern_timer = 0.0f;                         // �p�^�[���؂�ւ��̂��߂̃^�C�}�[
	Vector2D start_location;                            // �o�����̏����ʒu
	bool is_returning = false;                          // �߂蓮�쒆���ǂ����̃t���O
	bool on_hit;                                        // ��e�����ǂ���
	bool has_shot = false;                              // �e�����������ǂ���
	float after_shot_timer = 0.0f; // �e���ˌ�̑ҋ@����
	bool just_shot = false;                             // �e������������̈ꎞ�t���O


	//std::vector<int> image_num;

	Vector2D follow_velocity;                           // �v���C���[��Ǐ]����ۂ̑��x
	float spawn_delay_timer;                            // �o����̈�莞�ԍs�����~����^�C�}�[

	/// <summary>
	/// �s���p�^�[���������_���ɕύX����
	/// </summary>
	void ChangePatternRandomly();

public:
	/// <summary>
	/// �s���p�^�[�����O������ݒ肷��
	/// </summary>
	/// <param name="new_pattern">�V�����p�^�[��</param>
	void SetPattern(Zako1Pattern new_pattern);

};
