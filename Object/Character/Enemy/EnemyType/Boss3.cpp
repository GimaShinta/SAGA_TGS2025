#include "Boss3.h"
#include "../../../../Utility/ProjectConfig.h"
#include "../../Shot/EnemyBeam1.h"

Boss3::Boss3()
{
}

Boss3::~Boss3()
{
}

// ����������
void Boss3::Initialize()
{
	enemy_type = ENE_BOSS3;
	z_layer = 1;
	box_size = 30;
	hp = 10000;

	// �U���p�^�[���̐ݒ�
	attack_pattrn_num = { 11 };

	// �����蔻��̃I�u�W�F�N�g�ݒ�
	collision.is_blocking = true;
	// �����̃I�u�W�F�N�g�^�C�v
	collision.object_type = eObjectType::eBoss3;
	// �����鑊��̃I�u�W�F�N�g�^�C�v
	collision.hit_object_type.push_back(eObjectType::eShot);
	collision.hit_object_type.push_back(eObjectType::eBeam);

	// �������ǂ����itrue�Ȃ瓮���Afalse�Ȃ�~�܂�j
	is_mobility = true;

	// �퓬���̒��S���W
	base_position = Vector2D(D_WIN_MAX_X / 2, (D_WIN_MAX_Y / 2) - 225);

	// �o�ꎞ�̒��S���W
	generate_base_position = Vector2D(D_WIN_MAX_X / 2 + 150, 0 - 400);

	// �������W�͓o�ꎞ�̂��̂��g�p
	location = generate_base_position;

	ResourceManager* rm = Singleton<ResourceManager>::GetInstance();
	boss3_image[0] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_a2.png")[0];
	boss3_image[1] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_a3.png")[0];
	boss3_image[2] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_b1.png")[0];
	boss3_image[3] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_b2.png")[0];
	boss3_image[4] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_c1.png")[0];
	boss3_image[5] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_c2.png")[0];
	boss3_image[6] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_d1.png")[0];
	boss3_image[7] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_d2.png")[0];
	boss3_image[8] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_e1.png")[0];
	boss3_image[9] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_e2.png")[0];
	boss3_image[10] = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/transparent/boss01_f.png")[0];

	//boss3_anim1 = rm->GetImages("Resource/Image/Object/Enemy/Boss/Boss_03/anime01.png", 8, 8, 1, 32, 24);
	//boss3_image[10] = boss3_anim1[0];

	boss3_jet = rm->GetImages("Resource/Image/Effect/exhaust_03_spritesheet.png", 24, 8, 3, 128, 200);
	jet = boss3_jet[0];

	// �ŏ��͖{�̂̈ʒu�ɌŒ�
	for (int i = 0; i < 11; ++i)
	{
		part_positions[i] = location;
	}
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="delata_second">1�t���[��������̎���</param>
void Boss3::Update(float delta_second)
{
	// �A�j���[�V����
	GameObjectBase::AnimationControl(image);
	// �U���p�^�[����ݒ肵�Ēe��ł�
	Shot(delta_second);

	// �ړ�����
	Movement(delta_second);

	// �̗͂��Ȃ��Ȃ�����폜
	if (hp <= 0)
	{
		is_alive = false;
	}

	//// �U���p�^�[���ύX���Ɏ��ԃ��Z�b�g
	//if (attack_pattrn != prev_attack_pattrn)
	//{
	//	if (prev_attack_pattrn == 3)
	//	{
	//		move_time = 0.0f; // �O���̐擪�ɖ߂��I
	//	}
	//	prev_attack_pattrn = attack_pattrn;
	//}

	// ���i��x��ĒǏ]�����鏈��
	const float follow_speed = 5.0f; // �Ǐ]���x�i�傫���Ƒ������Ă���j

	// ���i�̑��΃I�t�Z�b�g�i���E��2���j
	Vector2D offsets[11] = {
		Vector2D(110, 0), // ����
		Vector2D(0,  -100.0f), // ��
		Vector2D(-60,  0), // �E��O
		Vector2D(-110, 0),  // �E��

		Vector2D(60, 30),  // �C
		Vector2D(-60, 30),  // �C
		Vector2D(-60, 30),  // �C
		Vector2D(-60, 30),  // �C
		Vector2D(-60, 30),  // �C
		Vector2D(-60, 30)  // �C
	};

	// ���i�̑��΃I�t�Z�b�g�i���E��2���j
	Vector2D offsets_2[11] = {
		Vector2D(0, -100), // ����
		Vector2D(0, 200), // ����O
		Vector2D(-60,  0), // �E��O
		Vector2D(-120, 250),  // �E��
		Vector2D(120, 250),  // �C
		Vector2D(-300, 100),  // �C
		Vector2D(300, 100),  // �C
		Vector2D(-350, -70),  // �C
		Vector2D(350, -70),  // �C
		Vector2D(-60, 30)  // �C
	};

	// �Ǐ]�̑���
	float individual_follow_speeds[11] = {
	100.0f,  // �����i�������j
	100.0f,  // ����O
	100.0f,  // �E��O
	15.0f,   // �E���i�������j

	15.0f,   // �C�i�������j
	10.0f,   // �C�i�������j
	10.0f ,  // �C�i�������j
	10.0f  , // �C�i�������j
	10.0f   ,// �C�i�������j
	100.0f   ,// �C�i�������j
	};

	// �Ǐ]�̑���
	float individual_follow_speeds_2[11] = {
	100.0f,  // �����i�������j
	100.0f,  // ����O
	100.0f,  // �E��O
	100.0f,   // �E���i�������j

	100.0f,   // �C�i�������j
	100.0f,   // �C�i�������j
	100.0f ,  // �C�i�������j
	100.0f  , // �C�i�������j
	100.0f   ,// �C�i�������j
	100.0f   ,// �C�i�������j
	};

	// �Ǐ]����
	for (int i = 0; i < 11; ++i)
	{
		Vector2D target;
		if (generate == false)
		{
			target = location + offsets[i];
			part_positions[i] += (target - part_positions[i]) * individual_follow_speeds_2[i] * delta_second;
			angle = 3.14 / 1.0f;

		}
		else
		{
			target = location + offsets_2[i];
			part_positions[i] += (target - part_positions[i]) * individual_follow_speeds[i] * delta_second;
			angle = 0.0f;
		}
	}


	if (attack_pattrn == 12)
	{
		anim_speed = 0.01;
	}
	else
	{
		anim_speed = 0.1f;
	}


	// �e�N���X�̍X�V�������Ăяo��
	__super::Update(delta_second);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="screen_offset"></param>
void Boss3::Draw(const Vector2D& screen_offset) const
{
	//// �G���P��`�悷��
	//DrawBox(location.x - box_size.x, location.y - box_size.y,
	//	location.x + box_size.x, location.y + box_size.y, GetColor(0, 255, 0), TRUE);

	// �̗͂̕\��
	DrawFormatString(location.x - 8, location.y - 8, GetColor(0, 0, 0), "%.0f", hp);

	// �{�X�̕`��
	DrawBoss3(Vector2D(location.x, location.y));
}

// �I��������
void Boss3::Finalize()
{
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss3::Movement(float delta_second)
{
	move_time += delta_second;

	const float float_amplitude_y = 30.0f;
	const float float_speed_y = 1.5f;
	const float float_amplitude_x = 20.0f;
	const float float_speed_x = 1.5f;
	Vector2D float_offset;
	float_offset.y = sinf(move_time * float_speed_y) * float_amplitude_y;
	float_offset.x = cosf(move_time * float_speed_x) * float_amplitude_x;

	if (!generate)
	{
		generate = true;
		velocity.y = -900.0f;
		base_position.x = generate_base_position.x;

		if (location.y < -400)
		{
			generate = true;
		}
	}
	else
	{
		float distance_y = base_position.y - location.y;
		float base_x = D_WIN_MAX_X / 2;
		base_position.x = generate_base_position.x - 150.0f;

		if (fabs(distance_y) > 1.0f)
		{
			const float max_speed = 150.0f;
			velocity.y = distance_y;
			if (velocity.y > max_speed) velocity.y = max_speed;
			if (velocity.y < -max_speed) velocity.y = -max_speed;
			static bool check = false;


			static int chenge = 1;

			switch (chenge)
			{
			case 1:
				box_size = Vector2D(350, 150);
				chenge++;
				break;
			case 2:
				box_size = Vector2D(230, 250);
				chenge++;
				break;
			case 3:
				box_size = Vector2D(160, 350);
				chenge++;
				break;
			default:
				chenge = 1;
				break;
			}

			//if (check == false)
			//{
			//	box_size = Vector2D(350, 150);
			//	check = true;
			//}
			//else
			//{
			//	box_size = Vector2D(160, 350);
			//	check = false;
			//}
			image_size =3.5f;
		}
		else
		{
			generate2 = true;

		}
	}

	generate_base_position += velocity * delta_second;
	base_position.x += velocity.x * delta_second;

	location.y = generate_base_position.y + float_offset.y;
	location.x = base_position.x + float_offset.x;
}

// �U��
void Boss3::Shot(float delta_second)
{
	// ���̍U���p�^�[���ύX�܂ł̎���
	const int shot_interval = 0.1f;

	// ���Ԍo�߂�����U���p�^�[����ύX���Ēe�𔭎�
	if (generate2 == true && is_shot == false)
	{
		shot_timer += delta_second;
		if (shot_timer >= shot_interval)
		{
#if 1

#if 0
			// ����̍U���̂݌J��Ԃ�
			attack_pattrn = 5;
#else
			// HP����������U���p�^�[����ύX�i�I�[�o�[�t���[�h�~�ɍ��킹�ă��Z�b�g�j
			if (hp <= 50 && attack_pattrn_num != std::vector<int>{7, 8})
			{
				attack_pattrn_num = { 7, 8 };
				attack_count = 0; // ���S�Ƀ��Z�b�g
			}

			// ���߂�ꂽ�U���p�^�[���̂݌J��Ԃ�
			if (!attack_pattrn_num.empty())
			{
				// �O�̂��ߔz��T�C�Y�`�F�b�N�����ށi�O�����j
				if (attack_count >= attack_pattrn_num.size())
				{
					attack_count = 0;
				}
				attack_pattrn = attack_pattrn_num[attack_count];
				attack_count++;
			}
			else
			{
				// �\�������F�U���p�^�[�����X�g����̎��͈��S�ȃf�t�H���g��
				attack_pattrn = 0;
			}
#endif

#else
			// ���S�����_��
			attack_pattrn = 4 + rand() % MAX_ATTACK_PATTRN;
#endif
			is_shot = true;
			shot_timer = 0;
		}
	}

	// �U��
	Attack(delta_second);
}

void Boss3::DrawBoss3(const Vector2D position) const
{
	DrawRotaGraph(part_positions[7].x, part_positions[7].y, image_size, angle, boss3_image[8], TRUE);
	DrawRotaGraph(part_positions[8].x, part_positions[8].y, image_size, angle, boss3_image[9], TRUE);
	// �{��
	DrawRotaGraph(position.x, position.y, image_size, angle, boss3_image[0], TRUE);
	// ���i
	DrawRotaGraph(part_positions[0].x, part_positions[0].y, image_size, angle, boss3_image[1], TRUE); // ����O
	DrawRotaGraph(part_positions[1].x, part_positions[1].y, image_size, angle, boss3_image[2], TRUE); // ����
	//DrawRotaGraph(part_positions[1].x, part_positions[1].y, image_size, angle, boss3_image[3], TRUE); // �E��O
	DrawRotaGraph(part_positions[5].x, part_positions[5].y, image_size, angle, boss3_image[6], TRUE);
	DrawRotaGraph(part_positions[3].x, part_positions[3].y, image_size, angle, boss3_image[4], TRUE); // �E��O
	DrawRotaGraph(part_positions[4].x, part_positions[4].y, image_size, angle, boss3_image[5], TRUE); // �E��
	DrawRotaGraph(part_positions[6].x, part_positions[6].y, image_size, angle, boss3_image[7], TRUE);
	//DrawRotaGraph(part_positions[1].x, part_positions[1].y, image_size, angle, boss3_image[10], TRUE);

	//if (generate == false)
	//{
	//	DrawRotaGraph(part_positions[4].x, part_positions[4].y - 50.0f, image_size, angle, boss3_image[7], TRUE);
	//	DrawRotaGraph(part_positions[5].x, part_positions[5].y - 50.0f, image_size, angle, boss3_image[7], TRUE);
	//}
	//else
	//{
	//	DrawRotaGraph(part_positions[4].x, part_positions[4].y + 110.0f, image_size, angle, boss3_image[7], TRUE);
	//	DrawRotaGraph(part_positions[5].x, part_positions[5].y + 110.0f, image_size, angle, boss3_image[7], TRUE);
	//}
	 
	//DrawBox(location.x - box_size.x, location.y - box_size.y,
	//	location.x + box_size.x, location.y + box_size.y,
	//	GetColor(255, 100, 0), TRUE);

	//DrawRotaGraph(D_WIN_MAX_X / 2, D_WIN_MAX_Y / 2, 2.0f, angle, jet, TRUE);

}

int Boss3::GetAttackPattrn() const
{
	return attack_pattrn;
}

int Boss3::GetIsAlive() const
{
	return is_alive;
}

// �{�X�Q�̍U��
void Boss3::Attack(float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	if (is_shot == true)
	{
		Vector2D e_location = location;

		switch (attack_pattrn)
		{
		case 1:
			for (int i = 0; i < 3; i++)
			{
				e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 30) + (30 * i), e_location.y + D_OBJECT_SIZE));
				e_shot4->SetVelocity(Vector2D(0, 120));
			}
			is_shot = false;
			break;
		case 2:
			// �c�Q����
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D((e_location.x - 60) + (30 * j), e_location.y));
					if (i == 0)
					{
						e_shot4->SetVelocity(Vector2D(0, 120));
					}
					else if (i == 1)
					{
						e_shot4->SetVelocity(Vector2D(0, -120));
					}
				}
			}
			// ���Q����
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					e_shot4 = objm->CreateObject<EnemyShot4>(Vector2D(e_location.x, (e_location.y - 60) + (30 * j)));
					if (i == 0)
					{
						e_shot4->SetVelocity(Vector2D(-120, 0));
					}
					else if (i == 1)
					{
						e_shot4->SetVelocity(Vector2D(120, 0));
					}
				}
			}
			is_shot = false;
			break;
		case 3:
			// �c�ɂR�悱�ɂT�����i�Е��j
			for (int j = 1; j < 4; j++)
			{
				for (int i = 0; i < 10; i++)
				{
					e_shot3 = objm->CreateObject<EnemyShot3>(location);
					e_shot3->SetPlayer(player);
					e_shot3->SetPlayerLocation(player->GetLocation());
					// +�P���Ȃ�������O�b�Ŕ��˂����
					e_shot3->SetStepShot(j);
					if (i < 5)
					{
						e_shot3->SetVelocity(Vector2D(240, 0));
						e_shot3->SetPurposeLocation(Vector2D((e_location.x + 150) + (30 * i), (e_location.y + 40) - (50 * j)));
					}
					else
					{
						e_shot3->SetVelocity(Vector2D(-240, 0));
						e_shot3->SetPurposeLocation(Vector2D(e_location.x - (30 * i), (e_location.y + 40) - (50 * j)));
					}
				}
			}
			is_shot = false;
			break;
		case 4:
			/// <summary>
			/// �U���p�^�[���S�i�ԉ΁j
			/// </summary>
			/// <param name="bullet_num">�e�̐�</param>
			/// <param name="speed">�e�̑��x</param>
			/// <param name="spiral_interval">���ˊԊu</param>
			/// <param name="spiral_duration_limit">�U�����鎞��</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn4(20, 300.0f, 1.0f, 5.0f, location, delta_second);
			break;
		case 5:
#if 0
			static float spiral_timer = 0.0f;            // �e���˂̃C���^�[�o���Ǘ�
			static float spiral_angle = 0.0f;            // ��]�p�x
			static float spiral_total_time = 0.0f;       // �p�^�[��5�ɓ����Ă���̗݌v����
			const float spiral_interval = 0.1f;
			const float spiral_duration_limit = 3.0f;    // �ő唭�ˎ���

			// 3�b�o�߂����牽�����Ȃ�
			if (spiral_total_time >= spiral_duration_limit) return;

			spiral_timer += delta_second;
			spiral_total_time += delta_second;

			if (spiral_timer >= spiral_interval)
			{
				spiral_timer = 0.0f;

				float rad = spiral_angle * DX_PI / 180.0f;
				float speed = 160.0f;
				Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

				GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();
				EnemyShot4* shot = objm->CreateObject<EnemyShot4>(Boss3->GetLocation());
				shot->SetVelocity(velocity);

				spiral_angle += 15.0f;
				if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
			}
#else

			/// <summary>
			/// �U���p�^�[���T�i�Q�����j
			/// </summary>
			/// <param name="spiral_interval">���ˊԊu</param>
			/// <param name="spiral_duration_limit">�U�����鎞��</param>
			/// <param name="spiral_speed">�e�̑��x</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn5(0.1f, 5.0f, 300.0f, location, delta_second);
#endif
			break;
		case 6:
			/// <summary>
			/// �U���p�^�[���U�i�o���o����^�j
			/// </summary>
			/// <param name="fan_angle_range">��̊p�x�i60�Ȃ�}30���j</param>
			/// <param name="bullet_speed">�e�̑��x</param>
			/// <param name="fan_interval">���ˊԊu</param>
			/// <param name="fan_duration_limit">�U�����鎞��</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn6(100.0f, 300.0f, 0.15f, 10.0f, location, delta_second);

			break;
		case 7:
			/// <summary>
			/// �U���p�^�[���V�i�i�K��`�j
			/// </summary>
			/// <param name="fan_angle_range">��̊p�x�͈́i60�Ȃ�}30���j</param>
			/// <param name="bullet_speed">�e�̑��x</param>
			/// <param name="fan_interval">���˂̊Ԋu</param>
			/// <param name="fan_duration_limit">�U������</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn7(150.0f, 300.0f, 0.5f, 10.0f, Vector2D(location.x, location.y + 100.0f), delta_second);

			break;
		case 8:
			/// <summary>
			/// �U���p�^�[���W�i�Ђ�Q�j
			/// </summary>
			/// <param name="wave_interval">���ˊԊu</param>
			/// <param name="wave_duration_limit">���Ԏ��Ԃ̏��</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn8(0.1f, 1.0f, Vector2D(location.x, location.y + 150.0f), delta_second);

			break;
		case 9:
			/// <summary>
			/// �U���p�^�[���X�i�{�X���S��]�e�j
			/// </summary>
			/// <param name="shot_count">�e�̐�</param>
			/// <param name="radius">�{�X����̋���</param>
			/// <param name="angular_speed">��]���x</param>
			/// <param name="bullet_speed">�e��</param>
			/// <param name="rotation_timer">��]�̎��Ԍv��</param>
			/// <param name="generate_location">��������ʒu</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn9(27, 250.0f, 90.0f, 0.0f, location, delta_second);

			/// <summary>
			/// �U���p�^�[��11�i��{�r�[���j
			/// </summary>
			/// <param name="offsets_x">�{�X����̋���(��)</param>
			Pattrn11(0.0f);

			break;
		case 10:
			/// <summary>
			/// �U���p�^�[���P�O�i�x�N�g�����S��]�e�j
			/// </summary>
			/// <param name="shot_count">�e�̐�</param>
			/// <param name="radius">�{�X����̋���</param>
			/// <param name="angular_speed">��]���x</param>
			/// <param name="bullet_speed">�e��</param>
			/// <param name="rotation_timer">��]�̎��Ԍv��</param>
			/// <param name="center_location">��������ꏊ</param>
			/// <param name="generate_location">��������ꏊ</param>
			/// <param name="delta_second">�P�t���[��������̎��ԁi��{�I�ɕύX�Ȃ��j</param>
			Pattrn10(8, 120.0f, 350.0f, 300.0f, 5.0f, location, delta_second);

			break;
		case 11:
			/// <summary>
			/// �U���p�^�[��11�i��{�r�[���j
			/// </summary>
			/// <param name="offsets_x">�{�X����̋���(��)</param>
			Pattrn11(0.0f);

			break;
		case 12:
			/// <summary>
			/// �U���p�^�[��12�i��{�r�[���j
			/// </summary>
			Pattrn12();

			break;
		case 13:
			Pattrn13(delta_second);

			break;
		default:
			break;
		}
	}
}

/// <summary>
/// �U���p�^�[���S
/// </summary>
/// <param name="bullet_num">�e�̐�</param>
/// <param name="speed">�e�̑��x</param>
/// <param name="spiral_interval">���ˊԊu</param>
/// <param name="spiral_duration_limit">�U�����鎞��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss3::Pattrn4(int bullet_num, float speed, float spiral_interval, float spiral_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float spiral_timer = 0.0f;
	static float spiral_total_time = 0.0f;

	spiral_timer += delta_second;
	spiral_total_time += delta_second;

	if (spiral_timer >= spiral_interval)
	{
		spiral_timer = 0.0f;

		for (int i = 0; i < bullet_num; i++)
		{
			float angle = (360.0f / bullet_num) * i;
			float rad = angle * DX_PI / 180.0f;

			Vector2D velocity(cos(rad) * speed, sin(rad) * speed);

			EnemyShot4* e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
			e_shot4->SetVelocity(velocity);
			e_shot4->SetAttackPattrn(1);
		}
	}

	// ���Ԑ����𒴂�����I���i���˂��Ȃ��j
	if (spiral_total_time >= spiral_duration_limit)
	{
		spiral_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���T
/// </summary>
/// <param name="spiral_interval">���ˊԊu</param>
/// <param name="spiral_duration_limit">�U�����鎞��</param>
/// <param name="spiral_speed">�e�̑��x</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss3::Pattrn5(float spiral_interval, float spiral_duration_limit, float spiral_speed, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float spiral_timer = 0.0f;
	static float spiral_angle = 0.0f;
	static float spiral_total_time = 0.0f;

	spiral_timer += delta_second;
	spiral_total_time += delta_second;

	if (spiral_timer >= spiral_interval)
	{
		spiral_timer = 0.0f;

		for (int dir = 0; dir < 2; dir++)
		{
			float base_angle = (dir == 0) ? 90.0f : 270.0f;
			float current_angle = base_angle + spiral_angle;

			float rad = current_angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * spiral_speed, sin(rad) * spiral_speed);

			EnemyShot4* shot = objm->CreateObject<EnemyShot4>(generate_location);
			shot->SetVelocity(velocity);
			shot->SetAttackPattrn(2);
		}

		spiral_angle += 40.0f;
		if (spiral_angle >= 360.0f) spiral_angle -= 360.0f;
	}

	// ���Ԑ����𒴂�����I���i���˂��Ȃ��j
	if (spiral_total_time >= spiral_duration_limit)
	{
		spiral_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���U
/// </summary>
/// <param name="fan_angle_range">��̊p�x�i60�Ȃ�}30���j</param>
/// <param name="bullet_speed">�e�̑��x</param>
/// <param name="fan_interval">���ˊԊu</param>
/// <param name="fan_duration_limit">�U�����鎞��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss3::Pattrn6(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	// �U�������p static �ϐ�
	static float fan_timer = 0.0f;
	static float fan_total_time = 0.0f;

	// �U���p�^�[�����ύX���ꂽ�烊�Z�b�g������ �� Boss3���ŏ�Ԏ�������̂�����

	fan_timer += delta_second;
	fan_total_time += delta_second;

	// ���Ԋu���Ƃɔ���
	if (fan_timer >= fan_interval)
	{
		fan_timer = 0.0f;

		// �����_���Ȋp�x�i���S�}60���j
		float base_angle = 90.0f; // ���������S
		float random_angle = base_angle - fan_angle_range / 2.0f + (rand() % (int)fan_angle_range);

		float rad = random_angle * DX_PI / 180.0f;
		Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

		e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
		e_shot4->SetVelocity(velocity);
		e_shot4->SetAttackPattrn(1);

	}

	// ���Ԑ����𒴂�����I���i���˂��Ȃ��j
	if (fan_total_time >= fan_duration_limit)
	{
		fan_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���V
/// </summary>
/// <param name="fan_angle_range">��̊p�x�͈́i60�Ȃ�}30���j</param>
/// <param name="bullet_speed">�e�̑��x</param>
/// <param name="fan_interval">���˂̊Ԋu</param>
/// <param name="fan_duration_limit">�U������</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss3::Pattrn7(float fan_angle_range, float bullet_speed, float fan_interval, float fan_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float fan_timer = 0.0f;
	static float fan_total_time = 0.0f;

	fan_timer += delta_second;
	fan_total_time += delta_second;

	if (fan_timer >= fan_interval)
	{
		fan_timer = 0.0f;

		int bullet_count = 6;
		float base_angle = 90.0f; // ���S������

		for (int i = 0; i < bullet_count; ++i)
		{
			float angle = base_angle - fan_angle_range / 2.0f +
				(fan_angle_range / (bullet_count - 1)) * i;

			float rad = angle * DX_PI / 180.0f;
			Vector2D velocity(cos(rad) * bullet_speed, sin(rad) * bullet_speed);

			e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);
			e_shot4->SetVelocity(velocity);
			e_shot4->SetAttackPattrn(2);
		}
	}

	// ��莞�Ԍo�߂�����I��
	if (fan_total_time >= fan_duration_limit)
	{
		fan_total_time = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���W
/// </summary>
/// <param name="wave_interval">���ˊԊu</param>
/// <param name="wave_duration_limit">���Ԏ��Ԃ̏��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss3::Pattrn8(float wave_interval, float wave_duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

#if 1
	static float wave_timer = 0.0f;
	static float wave_total_time = 0.0f;

	wave_timer += delta_second;
	wave_total_time += delta_second;

	if (wave_timer >= wave_interval)
	{
		wave_timer = 0.0f;
		Vector2D e_lo = generate_location;

		// �E��
		e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x + 50, e_lo.y));
		e_shot5->SetWaveReflected(false);
		e_shot5->SetVelocity(Vector2D(0, 200));
		e_shot5->SetWaveParameters(600.0f, 0.7f);

		// ����
		e_shot5 = objm->CreateObject<EnemyShot5>(Vector2D(e_lo.x - 50, e_lo.y));
		e_shot5->SetWaveReflected(true);
		e_shot5->SetVelocity(Vector2D(0, 200));
		e_shot5->SetWaveParameters(600.0f, 0.7f);
	}

	// ��莞�Ԍo�߂�����I��
	if (wave_total_time >= wave_duration_limit)
	{
		wave_total_time = 0.0f;
		is_shot = false;
	}
#else

	int num_shots = 20;
	float spread_speed = 150.0f;
	Vector2D origin = e_location; // �e�̏������ˈʒu�i�{�X�̈ʒu�j
	Vector2D boss_center = Boss3->GetLocation(); // �z�����ݒ��S�I

	for (int i = 0; i < num_shots; ++i)
	{
		float angle = 360.0f / num_shots * i;
		float rad = angle * DX_PI / 180.0f;

		e_shot5 = objm->CreateObject<EnemyShot5>(origin);
		e_shot5->SetVelocity(Vector2D(cosf(rad), sinf(rad)) * spread_speed);
		e_shot5->SetSuckCenter(boss_center); // �z�����ݐ���Z�b�g�I
	}
#endif

}

/// <summary>
/// �U���p�^�[���X
/// </summary>
/// <param name="shot_count">�e�̐�</param>
/// <param name="radius">�{�X����̋���</param>
/// <param name="angular_speed">��]���x</param>
/// <param name="bullet_speed">�e��</param>
/// <param name="rotation_timer">��]�̎��Ԍv��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss3::Pattrn9(int shot_count, float radius, float angular_speed, float bullet_speed, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static std::vector<float> angles;    // �e�e�̊p�x�Ǘ�
	static std::vector<EnemyShot4*> rotating_shots;  // �e�̃|�C���^��ێ�

	static float rotation_timer = 0.0f;

	rotation_timer += delta_second;

	// ����������
	if (angles.empty())
	{
		rotating_shots.clear();
		// �e�𐶐����ď����p�x��ݒ�
		for (int i = 0; i < shot_count; ++i)
		{
			float angle = 360.0f / shot_count * i;  // �e�̊p�x�ݒ�
			angles.push_back(angle);

			// �{�X�̎��͂ɒe��z�u
			e_shot4 = objm->CreateObject<EnemyShot4>(generate_location);

			if (e_shot4)
			{
				rotating_shots.push_back(e_shot4);  // �������������ꂽ��e��ۑ�
				e_shot4->SetVelocity(Vector2D(0, 0)); // ��]�����Ȃ̂Œe����0
				e_shot4->SetAttackPattrn(1);
			}
		}
	}

	// �e�e�̈ʒu���X�V���ĉ�]������
	for (int i = 0; i < shot_count; ++i)
	{
		angles[i] += angular_speed * delta_second; // �p�x�X�V
		if (angles[i] >= 360.0f) angles[i] -= 360.0f; // �p�x��0?360�x�ɒ���

		float rad = angles[i] * DX_PI / 180.0f;  // ���W�A���ɕϊ�

		// �{�X�𒆐S�ɔ��a `radius` �̉~�O����`��
		Vector2D new_pos = generate_location + Vector2D(cos(rad) * radius, sin(rad) * radius);

		// �e�̈ʒu���X�V
		if (rotating_shots[i])  // �e�����������݂��邩�m�F
		{
			rotating_shots[i]->SetLocation(new_pos);

			// �e�̑��x�̓[���i��]�̂݁j
			rotating_shots[i]->SetVelocity(Vector2D(0, 0));
		}
	}

	// �U���̏I������i��莞�Ԍo�ߌ�j
	static float duration_timer = 0.0f;
	duration_timer += delta_second;
	if (duration_timer >= 5.0f)  // 5�b��ɏI��
	{
		for (int i = 0; i < shot_count; ++i)
		{
			if (rotating_shots[i])
			{
				rotating_shots[i]->SetDestroy();  // �e���폜
			}

		}
		angles.clear();  // �p�x�����N���A
		rotating_shots.clear();  // �e�̃��X�g���N���A
		duration_timer = 0.0f;
		is_shot = false;
	}
}

/// <summary>
/// �U���p�^�[���P�O
/// </summary>
/// <param name="shot_count">�e�̐�</param>
/// <param name="radius">�{�X����̋���</param>
/// <param name="angular_speed">��]���x</param>
/// <param name="bullet_speed">�e��</param>
/// <param name="rotation_timer">��]�̎��Ԍv��</param>
/// <param name="generate_location">��������ꏊ</param>
/// <param name="delta_second">�P�t���[��������̎���</param>
void Boss3::Pattrn10(int shot_count, float radius, float angular_speed, float center_speed, float duration_limit, const Vector2D& generate_location, float delta_second)
{
	// �I�u�W�F�N�g�Ǘ��N���X�̃C���X�^���X���擾
	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	static float duration_timer = 0.0f;
	static Vector2D center_pos;  // �����Ȃ����S���W
	static Vector2D center_velocity;
	static std::vector<float> angles;
	static std::vector<EnemyShot4*> rotating_shots;
	static bool initialized = false;

	// ���Ԍo��
	duration_timer += delta_second;

	if (duration_timer < duration_limit)
	{
		if (!initialized) // ����������
		{
			// �{�X�̈ʒu���������S�Ƃ��āA�^���Ɉړ�������
			center_pos = generate_location;
			center_velocity = Vector2D(0.0f, center_speed); // �^������

			angles.clear();
			rotating_shots.clear();
			for (int i = 0; i < shot_count; ++i)
			{
				float angle = 360.0f / shot_count * i;
				angles.push_back(angle);

				EnemyShot4* shot = objm->CreateObject<EnemyShot4>(center_pos);
				if (shot)
				{
					shot->SetVelocity(Vector2D(0, 0));
					rotating_shots.push_back(shot);
					e_shot4->SetAttackPattrn(1);
				}
			}

			initialized = true;
		}

		// �����Ȃ����S�̈ړ�
		center_pos += center_velocity * delta_second;

		// ��]�e�̈ʒu�X�V
		for (int i = 0; i < shot_count; ++i)
		{
			angles[i] += angular_speed * delta_second;
			if (angles[i] >= 360.0f) angles[i] -= 360.0f;

			float rad = angles[i] * DX_PI / 180.0f;
			Vector2D offset(cos(rad) * radius, sin(rad) * radius);

			if (rotating_shots[i])
			{
				rotating_shots[i]->SetLocation(center_pos + offset);
			}
		}
	}
	else
	{
		// �U���I����̏���
		duration_timer = 0.0f;
		is_shot = false;
		initialized = false;  // �������t���O�����Z�b�g
	}
}

/// <summary>
/// �U���p�^�[��11�i��{�r�[���j
/// </summary>
/// <param name="offsets_x">�{�X����̋���</param>
void Boss3::Pattrn11(float offsets_x)
{
	static bool                         beam_on = false;          // ���ܔ��˒����H
	static EnemyBeam1* b;                     // ���˒��r�[��

	if (!beam_on)
	{
		GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

		b = objm->CreateObject<EnemyBeam1>(
			Vector2D(
				location.x + offsets_x,
				(location.y + 350.0f) - box_size.y
			));
		b->SetBoss3(this);
		beam_on = true;
	}

	if (b != nullptr)
	{
		// ���ꂼ�� +100 / -100 �̃I�t�Z�b�g�ŒǏ]
		b->SetLocation(
			Vector2D(
				location.x + offsets_x,
				(location.y + 350.0f) + b->GetBoxSize().y
			));
	}

	if (b != nullptr && b->is_destroy)   // 5 �b��Ȃǂ� true �ɂȂ�z��
	{
		b->SetDestroy();                 // �K�v�Ȃ疾���폜
		is_shot = false;
		beam_on = false;
	}
}

/// <summary>
/// �U���p�^�[��12�i�r�[����{�j
/// </summary>
void Boss3::Pattrn12()
{
	static bool                         beam_on = false;          // ���ܔ��˒����H
	static std::vector<EnemyBeam1*>     beams;                     // ���˒��r�[��
	static const float                  OFFSETS_X[2] = { +100.f, -100.f }; // +100 / -100 �̕���

	if (!beam_on)
	{
		GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

		for (int i = 0; i < 2; ++i)
		{
			EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
				Vector2D(
					location.x + OFFSETS_X[i],
					(location.y + 50.0f) - box_size.y
				));
			b->SetBoss3(this);
			beams.push_back(b);
		}
		beam_on = true;
	}

	for (size_t i = 0; i < beams.size(); ++i)
	{
		if (beams[i] == nullptr) continue;   // �O�̂���

		// ���ꂼ�� +100 / -100 �̃I�t�Z�b�g�ŒǏ]
		beams[i]->SetLocation(
			Vector2D(
				location.x + OFFSETS_X[i],
				(location.y + 50.0f) + beams[i]->GetBoxSize().y
			));
	}

	for (auto it = beams.begin(); it != beams.end(); )
	{
		EnemyBeam1* b = *it;

		if (b != nullptr && b->is_destroy)   // 5 �b��Ȃǂ� true �ɂȂ�z��
		{
			b->SetDestroy();                 // �K�v�Ȃ疾���폜
			it = beams.erase(it);            // vector ����O��
		}
		else
		{
			++it;
		}
	}

	// �S���������玟�̍U��������
	if (beam_on && beams.empty())
	{
		is_shot = false;
		beam_on = false;
	}
}

/// <summary>
/// �U���p�^�[��13�i�r�[���̒i�K�U���j�o�O����
/// </summary>
/// <param name="delta_second">1�t���[��������̎���</param>
void Boss3::Pattrn13(float delta_second)
{
	static int   step = 0;
	static float timer = 0.0f;
	static std::vector<EnemyBeam1*> beams;

	GameObjectManager* objm = Singleton<GameObjectManager>::GetInstance();

	timer += delta_second; // ��FPS�Ȃ� delta_second ���g��

	switch (step)
	{
	case 0: // ��1�i�K�F������1�{
	{
		EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
			Vector2D(location.x, location.y - D_OBJECT_SIZE)
		);
		b->SetBoss3(this);
		beams.push_back(b);
		step = 1;
		timer = 0.0f;
	}
	break;

	case 1: // 2�b��ɑ�2�i�K�F2�{�i���E�j
		if (timer >= 2.0f)
		{
			float offsets[] = { +100.0f, -100.0f };
			for (float ox : offsets)
			{
				EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
					Vector2D(location.x + ox, location.y - D_OBJECT_SIZE)
				);
				b->SetBoss3(this);
				beams.push_back(b);
			}
			step = 2;
			timer = 0.0f;
		}
		break;

	case 2: // 2�b��ɑ�3�i�K�F3�{�i�S�́j
		if (timer >= 2.0f)
		{
			float offsets[] = { +200.0f, 0.0f, -200.0f };
			for (float ox : offsets)
			{
				EnemyBeam1* b = objm->CreateObject<EnemyBeam1>(
					Vector2D(location.x + ox, location.y - D_OBJECT_SIZE)
				);
				b->SetBoss3(this);
				beams.push_back(b);
			}
			step = 3;
		}
		break;

	case 3: // �r�[���ʒu�Ǐ]�ƏI������
		for (size_t i = 0; i < beams.size(); ++i)
		{
			if (beams[i] == nullptr) continue;

			float offset = 0.0f;
			switch (beams.size())
			{
			case 1: offset = 0.0f; break;
			case 3: offset = (i == 0) ? +100.0f : (i == 1) ? -100.0f : 0.0f; break;
			case 6: offset = (i == 0) ? +0.0f : (i == 1) ? +100.0f : (i == 2) ? -100.0f :
				(i == 3) ? +200.0f : (i == 4) ? 0.0f : -200.0f;
				break;
			}

			beams[i]->SetLocation(
				Vector2D(
					location.x + offset,
					(location.y - D_OBJECT_SIZE) + beams[i]->GetBoxSize().y
				)
			);
		}

		// �r�[�������ׂď�������I��
		for (auto it = beams.begin(); it != beams.end(); )
		{
			EnemyBeam1* b = *it;
			if (b != nullptr && b->is_destroy)
			{
				b->SetDestroy();
				it = beams.erase(it);
			}
			else ++it;
		}

		if (beams.empty())
		{
			is_shot = false;
			step = 0;
			timer = 0.0f;
		}
		break;
	}
}
