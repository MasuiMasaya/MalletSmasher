#include "Polygon2D.h"


#define PI (3.141592f)	// 円周率

Polygon2D::Polygon2D():
	 m_pos (0.0f, 0.0f)
	,m_size(1.0f, 1.0f)
	,m_pVtx(NULL)
	,m_angle(0)
{
	m_pVtx = new Poly2DVertex[BUFFER_POINT_NUM];

	m_pVtx[0] = { { -1.0f,-1.0f, 0 },{ 1,1,1,1 },{ 0, 0 } };
	m_pVtx[1] = { {  1.0f,-1.0f, 0 },{ 1,1,1,1 },{ 0, 1 } };
	m_pVtx[2] = { { -1.0f, 1.0f, 0 },{ 1,1,1,1 },{ 1, 0 } };
	m_pVtx[3] = { {  1.0f,-1.0f, 0 },{ 1,1,1,1 },{ 0, 1 } };
	m_pVtx[4] = { {  1.0f, 1.0f, 0 },{ 1,1,1,1 },{ 1, 1 } };
	m_pVtx[5] = { { -1.0f, 1.0f, 0 },{ 1,1,1,1 },{ 1, 0 } };

	m_buffer.CreateVertexBuffer(
		m_pVtx, sizeof(Poly2DVertex), BUFFER_POINT_NUM, true
	);

	m_pos = DirectX::XMFLOAT2(0, 0);
}
Polygon2D::~Polygon2D()
{
	delete[] m_pVtx;
}
void Polygon2D::Update()
{
	
}

// 2Dポリゴン描画関数
void Polygon2D::Draw(ID3D11ShaderResourceView* pTexture)
{
	m_buffer.Write(m_pVtx);

	SHADER->SetWorld(
		DirectX::XMMatrixRotationZ(m_angle) *
		DirectX::XMMatrixScaling(m_size.x, m_size.y, 1.0f) *
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, 0) 
	);

	SHADER->SetTexture(pTexture);
	m_buffer.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	SHADER->SetTexture(NULL);
}

// 座標取得
DirectX::XMFLOAT2 Polygon2D::GetPos()
{
	return m_pos;
}

// サイズ取得
DirectX::XMFLOAT2 Polygon2D::GetSize()
{
	return m_size;
}

// 座標をセットする
void Polygon2D::SetPos(DirectX::XMFLOAT2 pos)
{
	m_pos = DirectX::XMFLOAT2(pos);
}

// サイズをセットする
void Polygon2D::SetSize(DirectX::XMFLOAT2 size)
{
	m_size = DirectX::XMFLOAT2(size);
}

// 回転情報をセットする
void Polygon2D::SetAngle(float angle)
{
	m_angle = angle;
}


// UVをセットする
void Polygon2D::SetUV(DirectX::XMFLOAT2 texturePos, DirectX::XMFLOAT2 textureSize)
{
	m_pVtx[0].uv = { texturePos.x, texturePos.y } ;
	m_pVtx[1].uv = { texturePos.x + textureSize.x, texturePos.y };
	m_pVtx[2].uv = { texturePos.x, texturePos.y + textureSize.y };
	m_pVtx[3].uv = { texturePos.x + textureSize.x, texturePos.y };
	m_pVtx[4].uv = { texturePos.x + textureSize.x, texturePos.y + textureSize.y };
	m_pVtx[5].uv = { texturePos.x, texturePos.y + textureSize.y };
}
