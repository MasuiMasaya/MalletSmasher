#include "Line.h"


//	コンストラクタ
Line::Line()
	:m_drawNum(0)
{
	// 描画する直線の頂点情報
	//Vertex vtx[] =
	//{
	//	{{0,0,0},{1,1,1,1},{0,0}}, // 視点
	//	{{5,5,5},{1,1,1,1},{0,0}}, // 終点
	//};

	for (int i = 0; i < MAX_LINE_VTX; i++)
	{
		m_vtx[i].color.w = 1.0f;
		m_vtx[i].color.x = 1.0f;
		m_vtx[i].color.y = 1.0f;
		m_vtx[i].color.z = 1.0f;
	}
	// バッファ作成
	m_buffer.CreateVertexBuffer(m_vtx, sizeof(Vertex), MAX_LINE_VTX, true);

}

// デストラクタ
Line::~Line()
{
}



void Line::Draw()
{
#if 0 // 書き込みテスト
	static float f = 0.0f;
	// 描画する直線の頂点情報
	Vertex vtx[] =
	{
		{{0,0,0},{1,1,1,1},{0,0}},         // 始点
		{{sinf(f),5,0},{1,1,1,1},{0,0}}, // 終点
	};
	f += 0.01f;
	m_buffer.Write(vtx);
#endif

	// 使用していない頂点データは初期化
	for (int i = m_drawNum; i < MAX_LINE_VTX; i++)
	{
		m_vtx[i].pos.x = m_vtx[i].pos.y = m_vtx[i].pos.z = 0.0f;
	}

	// 描画までに溜めた直線の情報を頂点バッファへ書き込み
	m_buffer.Write(m_vtx);

	// 頂点を描画する時のプリミティブ頂点バッファ内のデータを二つずつ読み取って直線を描画
	m_buffer.Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	m_drawNum = 0;
}

void Line::SetLine(XMFLOAT3 start, XMFLOAT3 end)
{
	// 頂点配列の先頭から直線のデータを格納
	if (m_drawNum < MAX_LINE_VTX)
	{
		m_vtx[m_drawNum].pos = start;
		++m_drawNum;
		m_vtx[m_drawNum].pos = end;
		++m_drawNum;
	}
}