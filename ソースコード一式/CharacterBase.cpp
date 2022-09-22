// インクル
#include "CharacterBase.h"
#include "Texture.h"



// コンストラクタ
CharacterBase::CharacterBase()
{
	// キャラクターは動くオブジェクトだろう
	m_collisionType = COLLISION_DYNAMIC;

	// 影で使用するテクスチャの読み込み
	LoadTextureFromFile("shadow.png", &m_pShadowTex);

	// 影描画用ポリゴン
	float s = 0.5;
	XMFLOAT4 w(1, 1, 1, 1);
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
		XMFLOAT2 uv;
	};
	Vertex vtx[] =
	{
	{ XMFLOAT3(-s,0,s),w,XMFLOAT2(0,0) },
	{ XMFLOAT3(s,0,s),w,XMFLOAT2(1,0) },
	{ XMFLOAT3(-s,0,-s),w,XMFLOAT2(0,1) },
	{ XMFLOAT3(s,0,-s),w,XMFLOAT2(1,1) },
	};
	m_shadowBuf.CreateVertexBuffer(vtx, sizeof(Vertex), _countof(vtx));

}

// デストラクタ
CharacterBase::~CharacterBase()
{

}

void CharacterBase::Draw()
{

	SHADER->SetWorld(XMMatrixScaling(m_size.x, m_size.y, m_size.z)*XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
	m_pCube->Draw();

	// 影の描画
	// 影はプレイヤーについて移動するがy座標まで移動すると、空中に影ができてしまうので、Y座標は０にする
	// Y軸が0のままだと地面と重なってしまいどちらが先に描画されるか不安定になってしまうので
	// (Zファイティング)少しだけY座標を上げて重なりを回避する
	float scale = (5.0f - m_pos.y) / 5.0f;
	scale = min(1, max(0, scale));

	SHADER->SetWorld(XMMatrixScaling(scale, scale, scale)*XMMatrixTranslation(m_pos.x, 0.001f, m_pos.z));
	//SHADER->SetWorld(XMMatrixTranslation(m_pos.x, 0.001f, m_pos.z));
	SHADER->SetTexture(m_pShadowTex);
	m_shadowBuf.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	SHADER->SetTexture(NULL);

}

