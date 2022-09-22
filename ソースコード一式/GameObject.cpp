// インクル
#include "GameObject.h"
#include "Shader.h"

// コンストラクタ
GameObject::GameObject()
	// イニシャライザ
	:m_pCube(nullptr)
	,m_pos(0,0,0)
	,m_size(1,1,1)
	,m_collisionType(COLLISION_STATIC)
	,m_move(0,0,0)
{

}

// デストラクタ
GameObject::~GameObject()
{
	Uninit();
}




// 四大処理
// 初期化処理
bool GameObject::Init()
{
	// キューブの生成
	m_pCube = new Cube();
	m_pCube->SetSize(m_size.x,m_size.y,m_size.z);
	m_pCube->SetPos(m_pos.x, m_pos.y, m_pos.z);

	return true;
}



// 終了処理
void GameObject::Uninit()
{
	if (m_pCube != nullptr)		// nullptr... NULLポインタ
	{
		delete m_pCube;
		m_pCube = NULL;
	}
}



// 更新処理
void GameObject::Update()
{

}



// 描画処理
void GameObject::Draw()
{
	//======================================================================
	// 3D座標変換の基本は行列で計算を行う
	// 例えばモデルを小さくしたい、回転させたい、移動させたい場合
	// それぞれの変換行列を作成する必要がある
	// (これら全てを一度に行う変換は自分で作るしかない)
	// それぞれの変換行列を掛け合わせた行列が最終的なワールド変換行列となる
	// この時、掛け算の順番を意識しないと意図しない結果となるので注意が必要
	// 基本的には拡縮×回転×移動
	// の順番で計算する
	//====================================================================== 
	// ワールド座標への変換
	SHADER->SetWorld(XMMatrixScaling(m_size.x, m_size.y, m_size.z)*XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	// 描画
	m_pCube->Draw();
}




// ゲッター
XMFLOAT3 GameObject::GetPos()
{
	return m_pos;
}



// ゲッター
XMFLOAT3 GameObject::GetSize()
{
	return m_size;
}


// ゲッター
ObjectCollisionType GameObject::GetCollisionType()
{
	return m_collisionType;
}


// ゲッター
XMFLOAT3 GameObject::GetMove()
{
	return m_move;
}


// セッター
void GameObject::SetPos(XMFLOAT3 pos)
{
	m_pos = pos;
}

void GameObject::SetSize(XMFLOAT3 size)
{
	m_size = size;
}

// セッター
void GameObject::SetMove(XMFLOAT3 move)
{
	m_move = move;
}