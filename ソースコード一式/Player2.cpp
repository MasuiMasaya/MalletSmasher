// インクル
#include "Player2.h"
#include "Input.h"
#include "Shader.h"
#include "model.h"
#include <cmath>

using namespace DirectX;

// コンストラクタ
Player2::Player2()
{
	m_pos.y = 1.0f;
	m_pos.x = 0.0f;
	m_pos.z = 1.0f;

	m_pPlayer2Info = nullptr;
}

// デストラクタ
Player2::~Player2()
{

}



bool Player2::Init()
{
	// 動的確保
	//--------------------------------
	m_pPlayer2Info = new Player2Info();
	//--------------------------------

	// 初期化
	//--------------------------------
	m_pos =  XMFLOAT3(7.0f, 2.0f, 0.0f);
	m_size = XMFLOAT3(1.5f, 1.5f, 1.5f);
	m_move = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//--------------------------------

	// プレイヤーの情報の初期化
	//---------------------------------------------------------------
	m_pPlayer2Info->CurPos = m_pos;
	m_pPlayer2Info->PrePos = m_pPlayer2Info->CurPos;
	m_pPlayer2Info->Radius = MALLET_RADIUS;
	m_pPlayer2Info->Move = m_move;
	m_pPlayer2Info->Power = MALLET_DEFAULT_POWER;
	m_pPlayer2Info->SmashButton = false;
	m_pPlayer2Info->SmashFrame = 0;
	m_pPlayer2Info->SmashChargeFrame = 0;
	m_pPlayer2Info->Mass = MALLET_MASS;
	m_pPlayer2Info->State = Mallet2Active;
	m_pPlayer2Info->HitStopFrame = 0;
	m_pPlayer2Info->Size = m_size;
	//---------------------------------------------------------------

	return true;
}

void Player2::Uninit()
{
	delete m_pPlayer2Info;
}

// 更新処理
void Player2::Update(Camera *pCamera)
{
	bool keyW = IsKeyPress('I');// 前
	bool keyA = IsKeyPress('J');// 左
	bool keyS = IsKeyPress('K');// 後
	bool keyD = IsKeyPress('L');// 右
	bool keyJ = IsKeyPress('B');

	const float Move = MALLET_SPEED;

	// 重力
	m_move.y -= GRAVITY;

	m_CameraXZAngle = pCamera->GetXZAngle()*3.14159265359f / 180.0f;

	m_pPlayer2Info->PrePos = m_pPlayer2Info->CurPos;

	// 移動
	m_pPlayer2Info->Move = XMFLOAT3(0, m_move.y, 0);

	static float SmashDirection = 0;

	if (keyA)
	{
		m_pPlayer2Info->Move.x += ((-cosf(m_CameraXZAngle)) / fabs((-cosf(m_CameraXZAngle))*(-cosf(m_CameraXZAngle)) + (-sinf(m_CameraXZAngle))*(-sinf(m_CameraXZAngle))))*MALLET_SPEED;
		m_pPlayer2Info->Move.z += ((-sinf(m_CameraXZAngle)) / fabs((-cosf(m_CameraXZAngle))*(-cosf(m_CameraXZAngle)) + (-sinf(m_CameraXZAngle))*(-sinf(m_CameraXZAngle))))*MALLET_SPEED;

		if (m_pPlayer2Info->CurPos.x - MALLET_RADIUS <= 0.0f)
		{
			m_pPlayer2Info->Move.x = 0.0f;
		}
	}
	if (keyD)
	{
		m_pPlayer2Info->Move.x += ((cosf(m_CameraXZAngle)) / fabs((cosf(m_CameraXZAngle))*(cosf(m_CameraXZAngle)) + (sinf(m_CameraXZAngle))*(sinf(m_CameraXZAngle))))*MALLET_SPEED;
		m_pPlayer2Info->Move.z += ((sinf(m_CameraXZAngle)) / fabs((cosf(m_CameraXZAngle))*(cosf(m_CameraXZAngle)) + (sinf(m_CameraXZAngle))*(sinf(m_CameraXZAngle))))*MALLET_SPEED;

		if (m_pPlayer2Info->CurPos.x >= 10.0f)
		{
			m_pPlayer2Info->Move.x = 0.0f;
		}
	}
	if (keyS)
	{
		m_pPlayer2Info->Move.x += ((sinf(m_CameraXZAngle)) / fabs((sinf(m_CameraXZAngle))* (sinf(m_CameraXZAngle)) + (-cosf(m_CameraXZAngle))*(-cosf(m_CameraXZAngle))))*MALLET_SPEED;
		m_pPlayer2Info->Move.z += ((-cosf(m_CameraXZAngle)) / fabs((sinf(m_CameraXZAngle))* (sinf(m_CameraXZAngle)) + (-cosf(m_CameraXZAngle))*(-cosf(m_CameraXZAngle))))*MALLET_SPEED;

		if (m_pPlayer2Info->CurPos.z <= -5)
		{
			m_pPlayer2Info->Move.z = 0.0f;
		}

	}
	if (keyW)
	{
		m_pPlayer2Info->Move.x += ((-sinf(m_CameraXZAngle)) / fabs((-sinf(m_CameraXZAngle))*(-sinf(m_CameraXZAngle)) + (cosf(m_CameraXZAngle))*(cosf(m_CameraXZAngle))))*MALLET_SPEED;
		m_pPlayer2Info->Move.z += ((cosf(m_CameraXZAngle)) / fabs((-sinf(m_CameraXZAngle))*(-sinf(m_CameraXZAngle)) + (cosf(m_CameraXZAngle))*(cosf(m_CameraXZAngle))))*MALLET_SPEED;

		if (m_pPlayer2Info->CurPos.z >= 5)
		{
			m_pPlayer2Info->Move.z = 0.0f;
		}
	}

	//スマッシュ
	m_pPlayer2Info->SmashButton = false;
	if (keyJ)
	{
		m_pPlayer2Info->SmashButton = true;
	}

	if (m_pPlayer2Info->SmashButton == true)
	{
		// チャージ時間
		m_pPlayer2Info->SmashChargeFrame++;

		// 減速
		m_pPlayer2Info->Move.x *= 0.3f;
		m_pPlayer2Info->Move.z *= 0.3f;
	}

	// スマッシュ発動
	if (m_pPlayer2Info->SmashButton == false && m_pPlayer2Info->SmashChargeFrame >= SMASH_CHARGE_FRAME)
	{
		SmashDirection = atan2f(m_pPlayer2Info->Move.z, m_pPlayer2Info->Move.x);
		m_pPlayer2Info->SmashFrame = SMASH_FRAME;
		m_pPlayer2Info->SmashChargeFrame = 0;
	}
	else if (m_pPlayer2Info->SmashButton == false && m_pPlayer2Info->SmashChargeFrame < SMASH_CHARGE_FRAME)
	{
		m_pPlayer2Info->SmashChargeFrame = 0;
	}

	if (m_pPlayer2Info->SmashFrame > 0)
	{
		m_pPlayer2Info->SmashFrame--;
		// スマッシュ中は直進する
		m_pPlayer2Info->Move.x = cosf(SmashDirection)*SMASH_SPEED;
		m_pPlayer2Info->Move.z = sinf(SmashDirection)*SMASH_SPEED;
	}



	// 移動量更新
	m_move.x = m_pPlayer2Info->Move.x;
	m_move.y = m_pPlayer2Info->Move.y;
	m_move.z = m_pPlayer2Info->Move.z;


	// 座標更新
	m_pPlayer2Info->CurPos.x = m_pos.x += m_move.x;
	m_pPlayer2Info->CurPos.y = m_pos.y += m_move.y;
	m_pPlayer2Info->CurPos.z = m_pos.z += m_move.z;


	// マレット2Pの位置の最低保証
	if (m_pPlayer2Info->CurPos.y <= -1.0f)
	{
		m_pos=m_pPlayer2Info->CurPos = XMFLOAT3(7.0f, 3.0f, 0.0f);
	}

}


XMFLOAT3 Player2::GetPlayerPos()
{
	return m_pos;
}

float Player2::GetCameraXZAngle()
{
	return m_CameraXZAngle;
}

Player2::Player2Info *Player2::GetPlayer2()
{
	return m_pPlayer2Info;
}