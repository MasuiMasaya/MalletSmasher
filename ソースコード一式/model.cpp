#include "model.h"
#include "Shader.h"
#include "Input.h"
#include "Player.h"
#include "SceneGame.h"
#include "Texture.h"

using namespace DirectX;

SceneGame* g_pSceneGameAddress;


// 定数定義
// 1秒(60FPS)で2m進む
#define MOVE (5.0f/60)


Model::Model()
	:m_pBuffer(NULL)
{
	// キャラクターは動くオブジェクトだろう
	m_collisionType = COLLISION_DYNAMIC;
}
Model::~Model()
{
	if (m_pBuffer != NULL)
	{
		delete[] m_pBuffer;
	}
}




bool Model::LoadModel(const char* pFilePath)
{
	HRESULT hr;

	// モデルファイルの読み込み
	hr= m_fbx.LoadModel(pFilePath);
	if (FAILED(hr))
	{
		return false;
	}

	// モデルのメッシュの数だけ,頂点バッファの作成
	int meshNum = m_fbx.GetMeshNum();
	m_pBuffer = new DrawBuffer[meshNum];
	for (int i = 0; i < meshNum; ++i)
	{
		// メッシュごとに頂点バッファを作成
		m_pBuffer[i].CreateVertexBuffer(
			m_fbx.GetVertexData(i), 
			m_fbx.GetVertexSize(i), 
			m_fbx.GetVertexCount(i)
		);
		// インデックスバッファの作成
		m_pBuffer[i].CreateIndexBuffer(m_fbx.GetIndexData(i), m_fbx.GetIndexCount(i));
	}


	return true;
}

bool Model::Init()
{

	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_size = XMFLOAT3(200, 200, 200);

	return true;
}

void Model::Uninit()
{

}



// 更新処理
void Model::Update()
{
	
	//g_pplayerpos->GetPlayerPos();
}




void Model::Draw(Camera *pCamera)
{


	int meshNum = m_fbx.GetMeshNum();
	for (int i = 0; i < meshNum; ++i)
	{
		SHADER->SetWorld(XMMatrixScaling(m_size.x/200, m_size.y/200, m_size.z/200)*XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
		SHADER->SetTexture(m_fbx.GetTexture(i));

		m_pBuffer[i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}



void Model::Draw()
{


	int meshNum = m_fbx.GetMeshNum();
	for (int i = 0; i < meshNum; ++i)
	{
		SHADER->SetWorld(XMMatrixScaling(m_size.x / 200, m_size.y / 200, m_size.z / 200)*XMMatrixTranslation(m_pos.x, m_pos.y-0.75f, m_pos.z));
		SHADER->SetTexture(m_fbx.GetTexture(i));
		
		m_pBuffer[i].Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}