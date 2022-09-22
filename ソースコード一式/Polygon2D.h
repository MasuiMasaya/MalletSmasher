#pragma once
//===============================================
//
//	Polygon2D.h
//
//===============================================
//	最終更新日:12月21日
//===============================================
#include "DrawBuffer.h"
#include <DirectXMath.h>
#include "Texture.h"
#include "Shader.h"

// 定数宣言
#define BUFFER_POINT_NUM (6)	// 使用する頂点バッファの数

// 構造体宣言
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

	// テクスチャのサイズ情報を渡す
	// 引数:
	// DirectX::XMFLOAT2 (座標)
	virtual void SetPos(DirectX::XMFLOAT2 pos);

	// テクスチャのサイズ情報を渡す
	// 引数:
	// DirectX::XMFLOAT2 (サイズ)
	virtual void SetSize(DirectX::XMFLOAT2 size);

	// テクスチャのUV情報を渡す
	// 引数:
	// DirectX::XMFLOAT2 (UV座標)
	// DirectX::XMFLOAT2 (UVサイズ)
	void SetUV(DirectX::XMFLOAT2 texturePos, DirectX::XMFLOAT2 textureSize);

	// テクスチャの回転情報を渡す
	// 引数:
	// float (円周率を入れると半回転)
	void SetAngle(float angle);

protected:
	// 座標
	DirectX::XMFLOAT2 m_pos;
	// サイズ
	DirectX::XMFLOAT2 m_size;
	// UV座標(UVの左上の座標)
	DirectX::XMFLOAT2 m_texturePos;
	// UVサイズ
	DirectX::XMFLOAT2 m_textureSize;
	// 回転度数
	float m_angle;
	
	DrawBuffer m_buffer;

private:
	Poly2DVertex* m_pVtx;
};