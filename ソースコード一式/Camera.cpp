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
	// �J�����ʒu�̌v�Z�@
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
	// �ɍ��W�n (�p�x�ƒ����ō��W��\��)
	// �������W�n (X��Y���ō��W��\��)
	//g_pCamera->Update();
	
	// �J�����̊p�x�ύX
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
		//	�J�����̏���x�N�g������ɏ�������Ă���̂łЂ�����Ԃ����G�͎��Ȃ�
		// ��ɂ܂�������������Ԃ̊G������
		// �ʏ�3D�̃c�[���ł̓J�������^�ォ�猩���낷�A�^�����猩�グ��G�ɂȂ�Ȃ��悤�Ɉړ��̐����������Ă���
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
	// �J�����̃p�����[�^��Shader�ɓn��
	EnableZBuffer(true);

	m_viewMat=(XMMatrixLookAtLH(XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 0),XMVectorSet(m_look.x, m_look.y, m_look.z, 0),XMVectorSet(m_up.x, m_up.y, m_up.z, 0)));
	SHADER->SetView(m_viewMat);

	SHADER->SetProjection(XMMatrixPerspectiveFovLH(m_angle*PI / 180.0f, (float)SCREEN_WIDTH/SCREEN_HEIGHT, m_near, m_far));

	// �J�����̍��W���V�F�[�_�ɐݒ�
	SHADER->SetPSCameraPos(m_pos);
}

void Camera::Bind2D()
{

	EnableZBuffer(false);
	// 2D�̓J�����̍��W�n�ւ̕ϊ����ȗ��ł���
	// MatrixIdentity�͒P�ʍs��𐶐�����
	// 1 0 0 0
	// 0 1 0 0
	// 0 0 1 0
	// 0 0 0 1
	SHADER->SetView(XMMatrixIdentity());
	//  2D�̕\�����@�𕽍s���e�ƌĂ�
	// 2D�̌��_���w��ł��镽�s���e�s����쐬
	// ����͉�ʂ̍�������_�Ɏw��
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
