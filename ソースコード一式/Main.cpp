#include "Main.h"
#include "DirectX.h"
#include "Texture.h"
#include "Input.h"
#include "Shader.h"
#include "Defines.h"
#include "FBX/FBXLoader.h"
#include "DrawBuffer.h"
#include "SceneManager.h"


// 定数定義
using namespace DirectX;

// 何個座標を保存するか
#define CONTROL_NUM (100)

// 何フレーム事に軌跡の座標を取るか
#define RECORD_MARGIN (1)


#define POLYLINE_NUM (10)




// グローバル変数
DrawBuffer g_buffer;
DrawBuffer g_polygon;
SceneManager* g_pSceneManager;





XMFLOAT3 g_recordPos[CONTROL_NUM*RECORD_MARGIN];

int PolNum[10];




HRESULT ErrorBox(HRESULT hr, const char* message)
{
	MessageBox(NULL, message, "Error", MB_OK);
	return hr;
}

HRESULT Init(HWND hWnd, UINT width, UINT height)
{
	
	HRESULT hr;
	// FBXの初期化
	ggfbx::Initialize();

	// DirectX
	hr = InitDX(hWnd, width, height, false);
	if (FAILED(hr)) { return ErrorBox(hr, "Failed to DirectX."); }



	// キーボード
	hr = InitKeyboard();
	if (FAILED(hr)) { return ErrorBox(hr, "Failed to Keyboard."); }



	// シェーダ
	hr = SHADER->Init();
	if (FAILED(hr)) { return ErrorBox(hr, "Failed to Shader."); }



	//================ゲームで使うオブジェクトの初期化================================

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
	g_polygon.CreateVertexBuffer(polyVtx, sizeof(Vertex), _countof(polyVtx));

	
	g_pSceneManager = new SceneManager;
	g_pSceneManager->Init(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	return hr;
}

void Uninit()
{
	g_pSceneManager->Uninit();
	delete g_pSceneManager;

	SHADER->Uninit();
	UninitKeyboard();
	UninitDX();
	ggfbx::Terminate();
}

void Update()
{
	g_pSceneManager->Update();
}

void Draw()
{
	g_pSceneManager->Draw();
}


SceneManager* GetSceneAddress()
{
	return g_pSceneManager;
}

// EOF