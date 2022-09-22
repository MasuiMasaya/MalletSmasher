#include "Camera.h"
#include "DirectX.h"
#include "Shader.h"
#include "Input.h"
#include "Defines.h"
#include "Player.h"


using namespace DirectX;

Camera::Camera()
	:m_radius(45.0f),m_pos(0, 0, m_radius), m_look(0, 0, 0)
	, m_up(0, 1, 0),m_angle(20.0f),m_near(0.5f),m_far(500.0f)
	,m_xzAngle(0.0f),m_yAngle(0.0f)
{

}
Camera::~Camera()
{

}


void Camera::CalculationCameraPos(XMFLOAT3 pos)
{
	// カメラ位置の計算　
	float rad = m_xzAngle * PI / 180.0f;
	float yrad = m_yAngle * PI / 180.0f;
	m_pos.x = (cos(yrad)*sin(rad)*m_radius) + pos.x;
	m_pos.z = (cos(yrad)*-cos(rad)*m_radius) + pos.z;
	m_pos.y = sin(yrad)*m_radius+pos.y;

}

void Camera::Init()
{
	m_yAngle = 43.0f;
}
void Camera::Uninit()
{

}
void Camera::Update()
{
	// 極座標系 (角度と長さで座標を表す)
	// 直交座標系 (X軸Y軸で座標を表す)
	//g_pCamera->Update();
	
	// カメラの角度変更
	if (IsKeyPress(VK_RIGHT))
	{
		m_xzAngle -= 1.0f;
	}
	if (IsKeyPress(VK_LEFT))
	{
		m_xzAngle += 1.0f;
	}
	if (IsKeyPress(VK_UP))
	{
		m_yAngle += 1.0f;
		//	カメラの上方ベクトルが常に上を向いているのでひっくり返った絵は取れない
		// 常にまっすぐ立った状態の絵が取れる
		// 通常3Dのツールではカメラが真上から見下ろす、真下から見上げる絵にならないように移動の制限をかけている
		if (m_yAngle >= 90.0f)
		{
			m_yAngle = 89.0f;
		}
	}
	if (IsKeyPress(VK_DOWN))
	{
		m_yAngle -= 1.0f;
		if (m_yAngle <= 0.0f)
		{
			m_yAngle = 0.0f;
		}
	}
	
	



}

void Camera::Bind()
{
	// カメラのパラメータをShaderに渡す
	EnableZBuffer(true);

	m_viewMat=(XMMatrixLookAtLH(XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0),XMVectorSet(m_look.x, m_look.y, m_look.z, 0),XMVectorSet(m_up.x, m_up.y, m_up.z, 0)));
	SHADER->SetView(m_viewMat);

	SHADER->SetProjection(XMMatrixPerspectiveFovLH(m_angle*PI / 180.0f, (float)SCREEN_WIDTH/SCREEN_HEIGHT, m_near, m_far));

	// カメラの座標をシェーダに設定
	SHADER->SetPSCameraPos(m_pos);
}

void Camera::Bind2D()
{

	EnableZBuffer(false);
	// 2Dはカメラの座標系への変換を省略できる
	// MatrixIdentityは単位行列を生成する
	// 1 0 0 0
	// 0 1 0 0
	// 0 0 1 0
	// 0 0 0 1
	SHADER->SetView(XMMatrixIdentity());
	//  2Dの表示方法を平行投影と呼ぶ
	// 2Dの原点を指定できる平行投影行列を作成
	// 今回は画面の左上を原点に指定
	SHADER->SetProjection(XMMatrixOrthographicOffCenterLH(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 300));
}

void Camera::Setlook(XMFLOAT3 pos)
{
	m_look = pos;
}


void Camera::SetCameraPos(XMFLOAT3 pos)
{
	m_pos = pos;
}

XMFLOAT3 Camera::GetCameraPos()
{
	return m_pos;
}
