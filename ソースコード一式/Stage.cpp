#include "Stage.h"
#include "Texture.h"

using namespace DirectX;

ID3D11ShaderResourceView* g_pFieldTex;
ID3D11ShaderResourceView* g_pColorTexRed;
ID3D11ShaderResourceView* g_pYukaTex;



// 定数定義
#define StageSizeY (0.1)

Stage::Stage()
	:m_ppFields(NULL)
	,m_nFieldNum(0)
{

}

Stage::~Stage()
{
	Uninit();
}



bool Stage::Init()
{
	struct FieldSetting
	{
		XMFLOAT3 pos;
		XMFLOAT3 size;
	};

	FieldSetting settings[] =
	{
		// 　　　　位置					大きさ
		// 下の板
		{XMFLOAT3(0, 0, 0),XMFLOAT3(STAGE_WIDTH,1,STAGE_HEIGHT),},

		// 床
		{XMFLOAT3(0, -20, 0),XMFLOAT3(500,1,500),},

		// 上下の壁
		{XMFLOAT3(0, 0.75f, 6.15f),XMFLOAT3(STAGE_WIDTH,0.5f,0.3f),},
		{XMFLOAT3(0, 0.75f, -6.15f),XMFLOAT3(STAGE_WIDTH,0.5f,0.3f),},

		// 左右の壁
		{XMFLOAT3(-11.15, 0.75f, 4),XMFLOAT3(0.3,0.5,4.5f),},
		{XMFLOAT3(-11.15, 0.75f, -4),XMFLOAT3(0.3,0.5,4.5f),},

		{XMFLOAT3(11.15f, 0.75f, 4),XMFLOAT3(0.3,0.5,4.5f),},
		{XMFLOAT3(11.15f, 0.75f, -4),XMFLOAT3(0.3,0.5,4.5f),}

		

		
	};

	// 初期データからフィールドの数を計算
	m_nFieldNum = sizeof(settings) / sizeof(settings[0]);
	// ポインタを格納する配列を作成
	m_ppFields = new Field*[m_nFieldNum];
	// それぞれの配列にフィールドをメモリ確保
	for (int i = 0; i < m_nFieldNum; i++)
	{
		m_ppFields[i] = new Field(settings[i].size);
		m_ppFields[i]->SetPos(settings[i].pos);
		m_ppFields[i]->Init();
	}

	LoadTextureFromFile("Assets/Image/blue.png", &g_pFieldTex);

	LoadTextureFromFile("red.png", &g_pColorTexRed);

	LoadTextureFromFile("Assets/Image/green.png", &g_pYukaTex);





	return true;
}



void Stage::Uninit()
{
	if (m_ppFields != NULL)
	{
		for (int i = 0; i < m_nFieldNum; i++)
		{
			// 個別に削除
			delete m_ppFields[i];
		}
		// 配列を削除
		delete[] m_ppFields;
		m_ppFields = NULL;
	}

	SAFE_RELEASE(g_pFieldTex);

	SAFE_RELEASE(g_pColorTexRed);

}



void Stage::Update()
{

}



void Stage::Draw()
{

	/*SHADER->SetTexture(g_pFieldTex);
	m_ppFields[0]->Draw();
	SHADER->SetTexture(NULL);*/

	SHADER->SetTexture(g_pYukaTex);
	m_ppFields[1]->Draw();
	SHADER->SetTexture(NULL);

	/*for (int i = 2; i < m_nFieldNum; i++)
	{
		SHADER->SetTexture(g_pColorTexRed);
		m_ppFields[i]->Draw();
		SHADER->SetTexture(NULL);
	}*/

	
}





Field *Stage::GetField(int index)
{
	if (index < m_nFieldNum)
	{
		return m_ppFields[index];
	}

	return NULL;
}




int Stage::GetFieldNum()
{
	return m_nFieldNum;
}


XMFLOAT3 Stage::GetStagePos()
{
	return m_ppFields[0]->GetPos();
}