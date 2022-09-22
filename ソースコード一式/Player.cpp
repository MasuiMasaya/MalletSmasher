// �C���N��
#include "Player.h"
#include "Input.h"
#include "Shader.h"
#include "model.h"
#include <cmath>

using namespace DirectX;

// �R���X�g���N�^
Player::Player()
{
	m_pos.y = 1.0f;
	m_pos.x = 0.0f;
	m_pos.z = 1.0f;

	m_pPlayerInfo = nullptr;

}

// �f�X�g���N�^
Player::~Player()
{

}



bool Player::Init()
{
	// ���I�m��
	//--------------------------------
	m_pPlayerInfo = new PlayerInfo();
	//--------------------------------

	// ������
	//--------------------------------
	m_pos = XMFLOAT3(-7.0f, 2.0f, 0.0f);
	m_size = XMFLOAT3(1.5f, 1.5f, 1.5f);
	m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//--------------------------------

	// �v���C���[�̏��̏�����
	//---------------------------------------------------------------
	m_pPlayerInfo->CurPos = m_pos;
	m_pPlayerInfo->PrePos = m_pPlayerInfo->CurPos;
	m_pPlayerInfo->Radius = MALLET_RADIUS;
	m_pPlayerInfo->Move = m_move;
	m_pPlayerInfo->Power = MALLET_DEFAULT_POWER;
	m_pPlayerInfo->SmashButton = false;
	m_pPlayerInfo->SmashFrame = 0;
	m_pPlayerInfo->SmashChargeFrame = 0;
	m_pPlayerInfo->Mass = MALLET_MASS;
	m_pPlayerInfo->State = MalletActive;
	m_pPlayerInfo->HitStopFrame = 0;
	m_pPlayerInfo->Size = m_size;
	//---------------------------------------------------------------


	return true;
}

void  Player::Uninit()
{
	delete m_pPlayerInfo;
}

// �X�V����
void Player::Update(Camera *pCamera)
{
	bool keyW = IsKeyPress('W');// �O
	bool keyA = IsKeyPress('A');// ��
	bool keyS = IsKeyPress('S');// ��
	bool keyD = IsKeyPress('D');// �E
	bool keyJ = IsKeyPress(VK_SHIFT);

	const float Move = MALLET_SPEED;

	// �d��
	m_move.y -= GRAVITY;

	m_CameraXZAngle = pCamera->GetXZAngle() * PI / 180.0f;

	m_pPlayerInfo->PrePos = m_pPlayerInfo->CurPos;

	// �ړ�
	m_pPlayerInfo->Move = XMFLOAT3(0, m_move.y, 0);

	static float SmashDirection = 0;

	if (keyA) 
	{
		m_pPlayerInfo->Move.x += ((-cosf(m_CameraXZAngle)) / fabs((-cosf(m_CameraXZAngle))*(-cosf(m_CameraXZAngle)) + (-sinf(m_CameraXZAngle))*(-sinf(m_CameraXZAngle))))*MALLET_SPEED;
		m_pPlayerInfo->Move.z += ((-sinf(m_CameraXZAngle)) / fabs((-cosf(m_CameraXZAngle))*(-cosf(m_CameraXZAngle)) + (-sinf(m_CameraXZAngle))*(-sinf(m_CameraXZAngle))))*MALLET_SPEED;

		if (m_pPlayerInfo->CurPos.x <= -10.0f)
		{
			m_pPlayerInfo->Move.x = 0.0f;
		}
	}
	if (keyD)
	{
		m_pPlayerInfo->Move.x += ((cosf(m_CameraXZAngle)) / fabs((cosf(m_CameraXZAngle))*(cosf(m_CameraXZAngle)) + (sinf(m_CameraXZAngle))*(sinf(m_CameraXZAngle))))*MALLET_SPEED;
		m_pPlayerInfo->Move.z += ((sinf(m_CameraXZAngle)) / fabs((cosf(m_CameraXZAngle))*(cosf(m_CameraXZAngle)) + (sinf(m_CameraXZAngle))*(sinf(m_CameraXZAngle))))*MALLET_SPEED;

		if (m_pPlayerInfo->CurPos.x + MALLET_RADIUS >= 0.0f)
		{
			m_pPlayerInfo->Move.x = 0.0f;
		}
	}
	if (keyS)
	{
		m_pPlayerInfo->Move.x += ((sinf(m_CameraXZAngle)) / fabs((sinf(m_CameraXZAngle))* (sinf(m_CameraXZAngle)) + (-cosf(m_CameraXZAngle))*(-cosf(m_CameraXZAngle))))*MALLET_SPEED;
		m_pPlayerInfo->Move.z += ((-cosf(m_CameraXZAngle)) / fabs((sinf(m_CameraXZAngle))* (sinf(m_CameraXZAngle)) + (-cosf(m_CameraXZAngle))*(-cosf(m_CameraXZAngle))))*MALLET_SPEED;

		if (m_pPlayerInfo->CurPos.z <= -5)
		{
			m_pPlayerInfo->Move.z = 0.0f;
		}
		
	}		   
	if (keyW)  
	{		   
		m_pPlayerInfo->Move.x += ((-sinf(m_CameraXZAngle)) / fabs((-sinf(m_CameraXZAngle))*(-sinf(m_CameraXZAngle)) + (cosf(m_CameraXZAngle))*(cosf(m_CameraXZAngle))))*MALLET_SPEED;
		m_pPlayerInfo->Move.z += ((cosf(m_CameraXZAngle)) / fabs((-sinf(m_CameraXZAngle))*(-sinf(m_CameraXZAngle)) + (cosf(m_CameraXZAngle))*(cosf(m_CameraXZAngle))))*MALLET_SPEED;

		if (m_pPlayerInfo->CurPos.z >= 5)
		{
			m_pPlayerInfo->Move.z = 0.0f;
		}
	}

	//�X�}�b�V��
	m_pPlayerInfo->SmashButton = false;
	if (keyJ)
	{
		m_pPlayerInfo->SmashButton = true;
	}

	if (m_pPlayerInfo->SmashButton == true)
	{
		// �`���[�W����
		m_pPlayerInfo->SmashChargeFrame++;

		// ����
		m_pPlayerInfo->Move.x *= 0.3f;
		m_pPlayerInfo->Move.z *= 0.3f;
	}

	// �X�}�b�V������
	if (m_pPlayerInfo->SmashButton == false && m_pPlayerInfo->SmashChargeFrame >= SMASH_CHARGE_FRAME)
	{
		SmashDirection = atan2f(m_pPlayerInfo->Move.z, m_pPlayerInfo->Move.x);
		m_pPlayerInfo->SmashFrame = SMASH_FRAME;
		m_pPlayerInfo->SmashChargeFrame = 0;

		if (m_pPlayerInfo->CurPos.x > -0.5f)
		{
			m_pPlayerInfo->CurPos.x = -0.5f;
		}
		else if (m_pPlayerInfo->CurPos.x < -10.25f)
		{
			m_pPlayerInfo->CurPos.x = -10.25f;
		}
	}
	else if (m_pPlayerInfo->SmashButton == false && m_pPlayerInfo->SmashChargeFrame < SMASH_CHARGE_FRAME)
	{
		m_pPlayerInfo->SmashChargeFrame = 0;
	}

	if (m_pPlayerInfo->SmashFrame > 0)
	{
		m_pPlayerInfo->SmashFrame--;
		// �X�}�b�V�����͒��i����
		if (m_pPlayerInfo->CurPos.x > -0.5f)
		{
			m_pPlayerInfo->CurPos.x = -0.5f;
		}
		else if (m_pPlayerInfo->CurPos.x < -10.25f)
		{
			m_pPlayerInfo->CurPos.x = -10.25f;
		}
		else
		{
			m_pPlayerInfo->Move.x = cosf(SmashDirection)*SMASH_SPEED;
			m_pPlayerInfo->Move.z = sinf(SmashDirection)*SMASH_SPEED;
		}
		
	}

	// �ړ��ʍX�V
	m_move.x = m_pPlayerInfo->Move.x;
	m_move.y = m_pPlayerInfo->Move.y;
	m_move.z = m_pPlayerInfo->Move.z;
	

	// ���W�X�V
	m_pPlayerInfo->CurPos.x = m_pos.x += m_move.x;
	m_pPlayerInfo->CurPos.y = m_pos.y += m_move.y;
	m_pPlayerInfo->CurPos.z = m_pos.z += m_move.z;

	// �}���b�g1P�̈ʒu�̍Œ�ۏ�
	if (m_pPlayerInfo->CurPos.y <= -1.0f)
	{
		m_pos = m_pPlayerInfo->CurPos = XMFLOAT3(-7.0f, 3.0f, 0.0f);
	}
}


XMFLOAT3 Player::GetPlayerPos()
{
	return m_pos;
}

float Player::GetCameraXZAngle()
{
	return m_CameraXZAngle;
}

Player::PlayerInfo *Player::GetPlayer()
{
	return m_pPlayerInfo;
}