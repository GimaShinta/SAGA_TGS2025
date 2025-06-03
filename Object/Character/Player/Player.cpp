#include "Player.h"
#include "../../../Utility/InputManager.h"
#include "../../../Utility/ProjectConfig.h"

Player::Player() : is_shot(false), count_space(1), life(3), on_hit(false), is_damage(false)
{
}

Player::~Player()
{
}

// ����������
void Player::Initialize()
{
	z_layer = 2;
	velocity = 0;
	box_size = 10;

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::ePlayer;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::eEnemy);
	collision.hit_object_type.push_back(eObjectType::eBoss2);
	collision.hit_object_type.push_back(eObjectType::eExp);
	collision.hit_object_type.push_back(eObjectType::ePowerUp);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	image = rm->GetImages("Resource/Image/Object/Player/Player_03/player03.png")[0];
	player_image_left = rm->GetImages("Resource/Image/Object/Player/Player_03/anime_player03_L01.png", 2, 2, 1, 56, 64);
	player_image_right = rm->GetImages("Resource/Image/Object/Player/Player_03/anime_player03_R01.png", 2, 2, 1, 56, 64);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void Player::Update(float delta_second)
{
	// �ړ�����
	Movement(delta_second);
	// �e��ł�
	Shot(delta_second);
	// �_���[�W���󂯂���
	Damage(delta_second);

	// ���C�t���O�ɂȂ�����Q�[���I�[�o�[
	if (life < 0)
	{
		is_alive = false;
	}

	if (powerd_time > 0.0f)
	{
		powerd_time -= delta_second;
	}
	else
	{
		powerd_on = false;
	}

	if (powerd >= 3)
	{
		powerd = 3;
	}


	// �A�j���[�V�����X�V����
	anim_timer += delta_second;

	if (anim_timer >= anim_interval)
	{
		anim_timer = 0.0f;
		anim_index++;
		if (anim_index >= 2) anim_index = 1;
	}

	// �e�N���X�̍X�V�������Ăяo��
	__super::Update(delta_second);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Player::Draw(const Vector2D& screen_offset) const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, brend);

	switch (anim_state)
	{
	case PlayerAnimState::Neutral:
		DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, image, TRUE);
		break;
	case PlayerAnimState::TiltLeft:
		DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, player_image_left[anim_index], TRUE);
		break;
	case PlayerAnimState::TiltRight:
		DrawRotaGraph(location.x, location.y, 1.0f, 0.0f, player_image_right[anim_index], TRUE);
		break;
	}

	// ���C�t�\���Ȃǂ͂��̂܂�
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawFormatString(D_WIN_MAX_X - 150, 0, GetColor(255, 255, 255), "�c�胉�C�t : %d", life);
}

// �I��������
void Player::Finalize()
{
}

/// <summary>
/// �q�b�g������
/// </summary>
/// <param name="hit_object">������������</param>
void Player::OnHitCollision(GameObjectBase* hit_object)
{
	if (hit_object->GetCollision().object_type == eObjectType::eEnemy ||
		hit_object->GetCollision().object_type == eObjectType::eEnemyShot ||
		hit_object->GetCollision().object_type == eObjectType::eEnemyBeam)
	{
		if (on_hit == false)
		{
			life--;
			is_damage = true;
			on_hit = true;
		}
	}
	if (hit_object->GetCollision().object_type == eObjectType::ePowerUp)
	{
		if (powerd_on == false)
		{
			powerd++;
			powerd_on = true;
			powerd_time = 4.0f;
		}
	}
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Player::Movement(float delta_second)
{
	InputManager* input = Singleton<InputManager>::GetInstance();
	Vector2D stick_value = input->GetLeftStick();

	const float accel = 1000.0f;
	const float friction = 800.0f;
	const float dead_zone = 0.1f;
	const float max_speed = PLAYER_SPEED_MAX;

	// ���̓x�N�g���i�������j
	Vector2D input_dir = { 0.0f, 0.0f };

	// �L�[���͂ɉ����ĕ�����ݒ�
	if (input->GetKey(KEY_INPUT_D) || input->GetButton(XINPUT_BUTTON_DPAD_RIGHT)) input_dir.x += 1.0f;
	if (input->GetKey(KEY_INPUT_A) || input->GetButton(XINPUT_BUTTON_DPAD_LEFT)) input_dir.x -= 1.0f;
	if (input->GetKey(KEY_INPUT_S) || input->GetButton(XINPUT_BUTTON_DPAD_DOWN)) input_dir.y += 1.0f;
	if (input->GetKey(KEY_INPUT_W) || input->GetButton(XINPUT_BUTTON_DPAD_UP)) input_dir.y -= 1.0f;

	// �X�e�B�b�N���͂�����
	if (std::abs(stick_value.x) > dead_zone) input_dir.x += stick_value.x;
	if (std::abs(stick_value.y) > dead_zone) input_dir.y -= stick_value.y;

	// ���͕���������ΐ��K��
	if (input_dir.Length() > 1.0f) input_dir.Normalize();

	// ���͕����Ɋ�Â��đ��x��ݒ�i�����������폜�j
	velocity = input_dir * max_speed;  // ���̍ő呬�x�ňړ�

	// ��ʊO�ړ�����
	const float limit_x = (D_WIN_MAX_X / 2);
	const float half_width = 350.0f; // �v���C���[�̕�

	// X���̐���
	if (location.x < limit_x - half_width + 10)
	{
		location.x = limit_x - half_width + 10;
		velocity.x = 0.0f; // �ʒu������ɑ��x���[���ɂ���
	}
	if (location.x > limit_x + half_width - 10)
	{
		location.x = limit_x + half_width - 10;
		velocity.x = 0.0f; // �ʒu������ɑ��x���[���ɂ���
	}

	// Y���̐���
	if (location.y < 10)
	{
		location.y = 10;
		velocity.y = 0.0f; // �ʒu������ɑ��x���[���ɂ���
	}
	if (location.y > D_WIN_MAX_Y - 10)
	{
		location.y = D_WIN_MAX_Y - 10;
		velocity.y = 0.0f; // �ʒu������ɑ��x���[���ɂ���
	}

	// �A�j���[�V������Ԍ���
	if (velocity.x > 10.0f)
	{
		anim_state = PlayerAnimState::TiltRight;
	}
	else if (velocity.x < -10.0f)
	{
		anim_state = PlayerAnimState::TiltLeft;
	}
	else
	{
		anim_state = PlayerAnimState::Neutral;
	}

	// �ʒu�X�V
	location += velocity * delta_second;
}

// �e��ł�
void Player::Shot(float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^�����擾
	InputManager* input = Singleton<InputManager>::GetInstance();

	// �ł܂ł̎��Ԃ��v��
	shot_timer += delta_second;
	beam_timer += delta_second;

	// �㉺�ɑłĂ�悤�ɂ���
	if (input->GetKeyDown(KEY_INPUT_L) ||
		input->GetButtonDown(XINPUT_BUTTON_RIGHT_SHOULDER))
	{
		if (shot_flip == false)
		{
			shot_flip = true;
		}
		else
		{
			shot_flip = false;
		}
	}

	// �X�y�[�X����������P���Ŕ���
	if (input->GetKeyDown(KEY_INPUT_SPACE) ||
		input->GetButtonDown(XINPUT_BUTTON_A))
	{
		// �����ł��ĂȂ�������łĂ�悤�ɂ���
		if (stop == false)
		{
			is_shot = true;
			shot_timer = 0.0f;
		}
	}
	// �X�y�[�X�𒷉�����������Ԋu�Ŕ���
	else if (input->GetKey(KEY_INPUT_SPACE) ||
		     input->GetButton(XINPUT_BUTTON_A))
	{
		// �����ł��ĂȂ�������łĂ�悤�ɂ���
		if (stop == false)
		{
			// ���˃C���^�[�o���𒴂����甭��
			if (shot_timer >= SHOT_INTERVAL)
			{
				is_shot = true;
				shot_timer = 0.0f;
			}
		}
	}
	else
	{
		// �����ĂȂ��Ԃ́A�����Ă�悤�ɂ���
		shot_timer = SHOT_INTERVAL; 
	}

	// B����������r�[������
	if ((input->GetKeyDown(KEY_INPUT_B) || input->GetButtonDown(XINPUT_BUTTON_B)) &&
		CanUseSpecial())  // �`���[�W�������̂�
	{
		if (stop == false)
		{
			beam_on = true;
			stop = true;
			beam_timer = 0.0f;
			UseSpecial();  // �Q�[�W����
		}
	}


	// �T�b�o������r�[���̍ċN
	if (beam_timer >= 5.0f)
	{
		stop = false;
	}
}

void Player::Damage(float delta_second)
{
	// �_���[�W���󂯂��Ƃ��`�J�`�J������
	if (is_damage == true)
	{
		// �J�E���g���Z
		damage_timer += delta_second;
		if (damage_timer >= 0.05f)
		{
			// ���B�J�E���g�����Z
			reach_count++;
			// ���B�J�E���g�������ł���Γ�����
			if (reach_count % 2 == 0)
			{
				brend = 0;
			}
			// ��ł���Εs������
			else
			{
				brend = 255;
			}
			//�J�E���g���Z�b�g
			damage_timer = 0;
		}
		// �`�J�`�J���I�������S�ă��Z�b�g
		if (reach_count >= 30)
		{
			is_damage = false;
			on_hit = false;
			brend = 255;
			damage_timer = 0;
			reach_count = 0;
		}
	}
}

bool Player::GetIsShot() const
{
	return is_shot;
}

bool Player::GetIsAlive() const
{
	return is_alive;
}

bool Player::GetBeamOn() const
{
	return beam_on;
}

bool Player::GetShotFlip() const
{
	return shot_flip;
}

void Player::SetIsShot()
{
	is_shot = false;
}

void Player::SetBeamOn()
{
	beam_on = false;
}

void Player::AddCharge(float value)
{
	// �r�[�����͉��Z���Ȃ�
	if (beam_on) return;

	charge += value;
	if (charge >= charge_max)
	{
		charge = charge_max;
		charge_ready = true;
	}
}

bool Player::CanUseSpecial() const
{
	return charge_ready;
}

void Player::UseSpecial()
{
	charge = 0.0f;
	charge_ready = false;
}

float Player::GetChargeRate() const
{
	// �������͏��MAX�\��
	if (beam_on) return 1.0f;
	return charge / charge_max;
}
