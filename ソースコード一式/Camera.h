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
	// カメラの行列
	// XMMATRIX...行列計算用の型
	XMMATRIX m_viewMat;


	XMFLOAT3 m_pos;
	XMFLOAT3 m_look;
	XMFLOAT3 m_up;

	float m_far;
	float m_near;
	float m_angle;

	// 極座標
	// XZ軸上での角度(0°で-Zの位置)
	float m_xzAngle;

	// XY軸上の角度
	float m_yAngle;

	// 中心からの距離
	float m_radius;

	//// カメラの位置
	//float m_PosX;
	//float m_PosY;
	//float m_PosZ;

	// カメラの注視点
	float m_GazePointX;
	float m_GazePointY;
	float m_GazePointZ;
};




#endif // !__CAMERA_H__

