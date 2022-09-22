#include "PolyLine.h"
#include "Texture.h"

//--- 定数定義
#define MAX_POLYLINE_NUM (10)			// 作成できるポリライン上限
#define MAX_POLYLINE_VERTEX_NUM (1000)	// 全体で使用できる頂点数

#define POLYLINEINTERVAL (4)

// 縮退用の頂点数
#define DEGENERATE_VTX (2)   

PolyLine::PolyLine()
	: m_pVtx(NULL)
	, m_pVertexNum(NULL)
{

	LoadTextureFromFile("siro.png", &m_pPolyLineTex);
	// 頂点生成
	m_pVtx = new PolyLineVertex[MAX_POLYLINE_VERTEX_NUM];
	for (int i = 0; i < MAX_POLYLINE_VERTEX_NUM; ++i)
	{
		m_pVtx[i].pos = DirectX::XMFLOAT3(0, 0, 0);
		m_pVtx[i].color = DirectX::XMFLOAT4(0.3, 1, 0.3, 1);
		if (i % POLYLINEINTERVAL == 0)
		{
			m_pVtx[i].uv = DirectX::XMFLOAT2(0, 0);
		}
		else if (i % POLYLINEINTERVAL == 1)
		{
			m_pVtx[i].uv = DirectX::XMFLOAT2(1, 0);
		}
		else if (i % POLYLINEINTERVAL == 2)
		{
			m_pVtx[i].uv = DirectX::XMFLOAT2(0, 1);
		}
		else if (i % POLYLINEINTERVAL == 3)
		{
			m_pVtx[i].uv = DirectX::XMFLOAT2(1, 1);
		}
	}
	m_buffer.CreateVertexBuffer(m_pVtx, sizeof(PolyLineVertex),
		MAX_POLYLINE_VERTEX_NUM, true);

	// ポリライン頂点数
	m_pVertexNum = new int[MAX_POLYLINE_NUM];
	for (int i = 0; i < MAX_POLYLINE_NUM; ++i)
	{
		m_pVertexNum[i] = 0;
	}
}
PolyLine::~PolyLine()
{
	delete[] m_pVertexNum;
	delete[] m_pVtx;
}

void PolyLine::Draw()
{
	m_buffer.Write(m_pVtx);
	SHADER->SetTexture(m_pPolyLineTex);
	m_buffer.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SHADER->SetTexture(NULL);
}

int PolyLine::MakePolyLine(int vtxNum)
{

	// 全体で使用している頂点数をカウント
	int useVtxNum = 0;

	// 先頭から使用していないポリラインを検索
	for (int i = 0; i < MAX_POLYLINE_NUM; ++i)
	{
		useVtxNum += m_pVertexNum[i];
		// 使用領域はスキップ
		if (m_pVertexNum[i] > 0)
		{
			continue;
		}
		// 未使用領域までに使用している頂点数と、今から使用する頂点数の総和が、ポリラインで扱える頂点の総数を超えていないか
		if (useVtxNum + vtxNum >= MAX_POLYLINE_VERTEX_NUM)
		{
			break;
		}

		// 未使用領域を見つけたら使用する頂点数を設定
		// クラス内で縮退処理を行うため引数の頂点数に縮退数を足し合わせた数で確保
		m_pVertexNum[i] = vtxNum+DEGENERATE_VTX;
		return i;
	}

	// ポリラインが作成できなかった
	return -1;
}



// ポリラインの頂点を設定
void PolyLine::SetVertex(int polyIndex, int vtxIndex, DirectX::XMFLOAT3 pos)
{
	// 指定されたポリラインの最初の頂点が、全体の頂点のうち、何番目の頂点か求める
	int startPolyIdx = 0;
	for (int i = 0; i < polyIndex; i++)
	{
		startPolyIdx += m_pVertexNum[i];
	}
	m_pVtx[startPolyIdx + vtxIndex].pos = pos;


	// 設定する頂点がポリラインのスタート位置の頂点なら縮退処理をおこなう
	if (vtxIndex == 0 && polyIndex != 0)
	{
		m_pVtx[startPolyIdx - 1].pos = pos;
	}

	// ポリラインを構成すると最後の頂点なら縮退処理を行う
	if (vtxIndex == m_pVertexNum[polyIndex] - DEGENERATE_VTX - 1)
	{
		m_pVtx[startPolyIdx + vtxIndex + 1].pos = pos;
	}
}




void PolyLine::SetUV(int polyIndex, int vtxIndex, DirectX::XMFLOAT2 uv)
{
	// 指定されたポリラインの最初の頂点が、全体の頂点のうち、何番目の頂点か求める
	int startPolyIdx = 0;
	for (int i = 0; i < polyIndex; i++)
	{
		startPolyIdx += m_pVertexNum[i];
	}
	m_pVtx[startPolyIdx + vtxIndex].uv = uv;


	// 設定する頂点がポリラインのスタート位置の頂点なら縮退処理をおこなう
	if (vtxIndex == 0 && polyIndex != 0)
	{
		m_pVtx[startPolyIdx - 1].uv = uv;
	}

	// ポリラインを構成すると最後の頂点なら縮退処理を行う
	if (vtxIndex == m_pVertexNum[polyIndex] - DEGENERATE_VTX - 1)
	{
		m_pVtx[startPolyIdx + vtxIndex + 1].uv = uv;
	}
}





int PolyLine::GetPolyLineNum()
{
	int cnt = 0;
	for (int i = 0; i < MAX_POLYLINE_NUM; ++i)
	{
		if (m_pVertexNum[i] > 0)
		{
			cnt++;
		}
		else
		{
			break;
		}
	}
	return cnt;
}



int PolyLine::GetVertexNum(int polyIndex)
{
	if (polyIndex < MAX_POLYLINE_NUM)
	{
		return m_pVertexNum[polyIndex] - 2;
	}
	return 0;
}
