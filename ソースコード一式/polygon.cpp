#include "polygon.h"

CPolygon::CPolygon()
{

}

CPolygon::~CPolygon()
{

}

void CPolygon::Init()
{
	struct Vertex
	{
		// x,y,z
		float pos[3];

		// r,g,b,a
		float color[4];

		// u,v
		float uv[2];

	};

	Vertex polyVtx[] =
	{
	{{0, 0,0},{1,1,1,1},{0,0}},	// 左上
	{{1, 0,0},{1,1,1,1},{1,0} },	// 右上
	{{0,1,0},{1,1,1,1},{0,1}},	// 左下
	{{1,1,0},{1,1,1,1},{1,1} },	// 右下
	};

	// 頂点バッファの作成
	m_polygon.CreateVertexBuffer(polyVtx, sizeof(Vertex), _countof(polyVtx));
}

void CPolygon::Uninit()
{

}

void CPolygon::Update()
{

}

void CPolygon::Draw()
{
	//SHADER->SetWorld(XMMatrixScaling(SCREEN_WIDTH, SCREEN_HEIGHT, 1)*XMMatrixTranslation(0, 0, 0));

	//SHADER->SetTexture(NULL);
	m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//SHADER->SetTexture(NULL);

}
void CPolygon::SetPolygon(XMFLOAT3 polygonsize, XMFLOAT3 polygonpos)
{
	SHADER->SetWorld(XMMatrixScaling(polygonsize.x, polygonsize.y, 1)*XMMatrixTranslation(polygonpos.x, polygonpos.y, polygonpos.z));
	Draw();
}