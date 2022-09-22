#pragma once
//===============================================
//
//	Polygon2D.h
//
//===============================================
//	�ŏI�X�V��:12��21��
//===============================================
#include "DrawBuffer.h"
#include <DirectXMath.h>
#include "Texture.h"
#include "Shader.h"

// �萔�錾
#define BUFFER_POINT_NUM (6)	// �g�p���钸�_�o�b�t�@�̐�

// �\���̐錾
struct Poly2DVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
};

class Polygon2D
{
public:
	Polygon2D();
	~Polygon2D();

	virtual void Update();
	virtual void Draw(ID3D11ShaderResourceView* pTexture);

	DirectX::XMFLOAT2 GetPos();
	DirectX::XMFLOAT2 GetSize();

	// �e�N�X�`���̃T�C�Y����n��
	// ����:
	// DirectX::XMFLOAT2 (���W)
	virtual void SetPos(DirectX::XMFLOAT2 pos);

	// �e�N�X�`���̃T�C�Y����n��
	// ����:
	// DirectX::XMFLOAT2 (�T�C�Y)
	virtual void SetSize(DirectX::XMFLOAT2 size);

	// �e�N�X�`����UV����n��
	// ����:
	// DirectX::XMFLOAT2 (UV���W)
	// DirectX::XMFLOAT2 (UV�T�C�Y)
	void SetUV(DirectX::XMFLOAT2 texturePos, DirectX::XMFLOAT2 textureSize);

	// �e�N�X�`���̉�]����n��
	// ����:
	// float (�~����������Ɣ���])
	void SetAngle(float angle);

protected:
	// ���W
	DirectX::XMFLOAT2 m_pos;
	// �T�C�Y
	DirectX::XMFLOAT2 m_size;
	// UV���W(UV�̍���̍��W)
	DirectX::XMFLOAT2 m_texturePos;
	// UV�T�C�Y
	DirectX::XMFLOAT2 m_textureSize;
	// ��]�x��
	float m_angle;
	
	DrawBuffer m_buffer;

private:
	Poly2DVertex* m_pVtx;
};