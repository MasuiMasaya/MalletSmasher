#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <DirectXMath.h>


using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void Uninit();
	void Update();

	void Bind();
	void Bind2D();

	void Setlook(XMFLOAT3);

	void CalculationCameraPos(XMFLOAT3);

	XMFLOAT3 GetCameraPos();

	XMMATRIX GetView() { return m_viewMat; }

	float GetXZAngle() { return m_xzAngle; }

	void SetCameraPos(XMFLOAT3);

private:
	// �J�����̍s��
	// XMMATRIX...�s��v�Z�p�̌^
	XMMATRIX m_viewMat;


	XMFLOAT3 m_pos;
	XMFLOAT3 m_look;
	XMFLOAT3 m_up;

	float m_far;
	float m_near;
	float m_angle;

	// �ɍ��W
	// XZ����ł̊p�x(0����-Z�̈ʒu)
	float m_xzAngle;

	// XY����̊p�x
	float m_yAngle;

	// ���S����̋���
	float m_radius;

	//// �J�����̈ʒu
	//float m_PosX;
	//float m_PosY;
	//float m_PosZ;

	// �J�����̒����_
	float m_GazePointX;
	float m_GazePointY;
	float m_GazePointZ;
};




#endif // !__CAMERA_H__

