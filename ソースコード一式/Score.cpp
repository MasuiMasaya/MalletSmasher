#include "Score.h"


// 定数定義
using namespace DirectX;

ID3D11ShaderResourceView *g_pScore_1PTex;
ID3D11ShaderResourceView *g_pScore_2PTex;
Polygon2D* m_pScoreUV;


Score::Score()
{
	
}

Score::~Score()
{
}

void Score::Init()
{

	//  ポリゴンの情報
	//-----------------------------------------------------------------------------
	struct Vertex
	{
		// x,y,z
		float pos[3];

		// r,g,b,a
		float color[4];

		// u,v
		float uv[2];

	};

	// ポリゴンの頂点情報
	Vertex polyVtx[] =
	{
	{{0, 0,0},{1,1,1,1},{0,0}},	// 左上
	{{1, 0,0},{1,1,1,1},{1,0} },// 右上
	{{0,1,0},{1,1,1,1},{0,1}},	// 左下
	{{1,1,0},{1,1,1,1},{1,1} },	// 右下
	};

	// 頂点バッファの作成
	m_polygon.CreateVertexBuffer(polyVtx, sizeof(Vertex), _countof(polyVtx));
	//-----------------------------------------------------------------------------


	m_pScore1P = new ScoreInfo();
	m_pScore2P = new ScoreInfo();
	m_pEndScore = new ScoreInfo();

	m_pScore1P->Score = 0;
	m_pScore2P->Score = 0;
	m_pEndScore->Score = 1;

	// テクスチャの貼り付け
	LoadTextureFromFile("Assets/Image/number.png", &g_pScore_1PTex);

	LoadTextureFromFile("Assets/Image/number.png", &g_pScore_2PTex);

}

void Score::Uninit()
{
	SAFE_RELEASE(g_pScore_1PTex);
	SAFE_RELEASE(g_pScore_2PTex);
}

void Score::Update()
{

}

void Score::Draw()
{
	float work1P = (float)m_pScore1P->Score * 0.1f;
	float work2P = (float)m_pScore2P->Score * 0.1f;


	SHADER->Bind(VS_TEXANIME, PS_PHONG);

	SHADER->SetWorld(XMMatrixScaling(80, 100, 1) * XMMatrixTranslation(350, 30, 0));
	SHADER->SetTextureScale(XMFLOAT2(0.1f, 1.0f));
	SHADER->SetTextureOffset(XMFLOAT2(work1P, 0));
	SHADER->SetTexture(g_pScore_1PTex);
	m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SHADER->SetWorld(XMMatrixScaling(80, 100, 1) * XMMatrixTranslation(800, 30, 0));
	SHADER->SetTextureScale(XMFLOAT2(0.1f, 1.0f));
	SHADER->SetTextureOffset(XMFLOAT2(work2P, 0));
	SHADER->SetTexture(g_pScore_2PTex);
	m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SHADER->Bind(VS_WORLD, PS_UNLIT);
}

void Score::AddScore_1P(int score)
{
	m_pScore1P->Score += score;
}
void Score::AddScore_2P(int score)
{
	m_pScore2P->Score += score;
}

void Score::SetGameEndScore(int score)
{
	m_GameEndScore = score;
}

ScoreInfo *Score::GetScore_1P()
{
	return m_pScore1P;
}

ScoreInfo *Score::GetScore_2P()
{
	return m_pScore2P;
}

int Score::GetGameEndScore()
{
	return m_GameEndScore;
}
