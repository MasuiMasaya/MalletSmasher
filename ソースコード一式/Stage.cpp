#include "Stage.h"
#include "Texture.h"

using namespace DirectX;

ID3D11ShaderResourceView* g_pFieldTex;
ID3D11ShaderResourceView* g_pColorTexRed;
ID3D11ShaderResourceView* g_pYukaTex;



// �萔��`
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
		// �@�@�@�@�ʒu					�傫��
		// ���̔�
		{XMFLOAT3(0, 0, 0),XMFLOAT3(STAGE_WIDTH,1,STAGE_HEIGHT),},

		// ��
		{XMFLOAT3(0, -20, 0),XMFLOAT3(500,1,500),},

		// �㉺�̕�
		{XMFLOAT3(0, 0.75f, 6.15f),XMFLOAT3(STAGE_WIDTH,0.5f,0.3f),},
		{XMFLOAT3(0, 0.75f, -6.15f),XMFLOAT3(STAGE_WIDTH,0.5f,0.3f),},

		// ���E�̕�
		{XMFLOAT3(-11.15, 0.75f, 4),XMFLOAT3(0.3,0.5,4.5f),},
		{XMFLOAT3(-11.15, 0.75f, -4),XMFLOAT3(0.3,0.5,4.5f),},

		{XMFLOAT3(11.15f, 0.75f, 4),XMFLOAT3(0.3,0.5,4.5f),},
		{XMFLOAT3(11.15f, 0.75f, -4),XMFLOAT3(0.3,0.5,4.5f),}

		

		
	};

	// �����f�[�^����t�B�[���h�̐����v�Z
	m_nFieldNum = sizeof(settings) / sizeof(settings[0]);
	// �|�C���^���i�[����z����쐬
	m_ppFields = new Field*[m_nFieldNum];
	// ���ꂼ��̔z��Ƀt�B�[���h���������m��
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
			// �ʂɍ폜
			delete m_ppFields[i];
		}
		// �z����폜
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