#include "SceneResult.h"
#include "SceneManager.h"
#include "SceneGame.h"
#include "Main.h"
#include "Texture.h"
#include "DrawBuffer.h"
#include "Polygon2D.h"

#define Result_WIDTH (700)
#define Result_HEIGHT (150)

ID3D11ShaderResourceView* g_pResultUITex;
ID3D11ShaderResourceView* g_pResultTex;
SceneManager* g_pSceneR;
DWORD g_pGetTime;
DrawBuffer m_pPolygon;
//Polygon2D* g_pPoly2D;

SceneResult::SceneResult()
{
}

SceneResult ::~SceneResult()
{
}

void SceneResult::Init()
{

	// 頂点の情報を持った構造体
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
	{{1, 0,0},{1,1,1,1},{1,0} },	// 右上
	{{0,1,0},{1,1,1,1},{0,1}},	// 左下
	{{1,1,0},{1,1,1,1},{1,1} },	// 右下
	};
	// 頂点バッファの作成
	m_pPolygon.CreateVertexBuffer(polyVtx, sizeof(Vertex), _countof(polyVtx));


	m_pCamera = new Camera();
	m_pCamera->Init();

	//g_pPoly2D = new Polygon2D();

	g_pSceneR = GetSceneAddress();


	// タイトルロゴのテクスチャ取得
	LoadTextureFromFile("pushspace.png", &g_pResultTex);

	// タイトルロゴのテクスチャ取得
	//LoadTextureFromFile("result.jpg", &g_pResultUITex);
}
void SceneResult::Uninit()
{
	//delete g_pPoly2D;

	m_pCamera->Uninit();
	delete m_pCamera;

	SAFE_RELEASE(g_pResultUITex);

	SAFE_RELEASE(g_pResultTex);
}
void SceneResult::Update()
{
	UpdateKeyboard();

	if (IsKeyTrigger(VK_SPACE))
	{
		g_pSceneR->SetScene(Scene_Title);
	}

}
void SceneResult::Draw()
{
	BeginDrawDX();


	// シェーダ
	SHADER->Bind(VS_WORLD, PS_UNLIT);

	SHADER->SetLightAmbient(XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f));

	//==========================================================
	// 1,オブジェクトごとにローカル座標を持つ(頂点)
	// 2,ローカル座標をワールド座標に変換
	// 3,ワールド座標をカメラの座標に変換
	// 4,カメラの画角に収まる範囲に変換(スクリーン座標)
	//==========================================================
	static float a = 0.0f;

	// ワールド座標へ変換
	// XMMatrixRotationX X軸で回転
	// XMMatrixRotationY Y軸で回転
	// XMMatrixRotationZ Z軸で回転
	// XMMatrixScaling 拡縮
	// それぞれの変換行列をDirectXが計算してくれる
	SHADER->SetWorld(DirectX::XMMatrixRotationY(a += 3.141592f / 180.0f));
	SHADER->SetWorld(DirectX::XMMatrixTranslation(0, 0, 0));


	// 2Dの描画設定
	m_pCamera->Bind2D();

	// リザルト背景
	SHADER->SetWorld(XMMatrixScaling(1280, 720, 1)*XMMatrixTranslation(0, 0, 0));

	SHADER->SetTexture(g_pResultUITex);
	m_pPolygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	SHADER->SetTexture(NULL);


	// タイトルロゴ
	SHADER->SetWorld(XMMatrixScaling(Result_WIDTH, Result_HEIGHT, 1)*XMMatrixTranslation(SCREEN_WIDTH / 2 - Result_WIDTH / 2, SCREEN_HEIGHT - Result_HEIGHT, 0));

	SHADER->SetTexture(g_pResultTex);
	m_pPolygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	SHADER->SetTexture(NULL);




	DrawKeyboard();
	EndDrawDX();
}