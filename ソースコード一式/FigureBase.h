#pragma once

// �C���N��
#include "DrawBuffer.h"
#include <DirectXmath.h>

using namespace DirectX;

class Figure
{
public:
	Figure();
	~Figure();

	//// ���_�o�b�t�@
	//struct Vertex
	//{
	//	float pos[3];
	//	float color[4];
	//	float uv[2];
	//};

	struct  Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
		XMFLOAT2 uv;
	};




	void SetPos(float x, float y, float z);
	void SetSize(float w, float h, float d);

	virtual void Draw();
private:
	float m_posX, m_posY, m_posZ;
	float m_width, m_height, m_depth;

protected:
	DrawBuffer m_buffer;

};

