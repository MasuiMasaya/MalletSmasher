#include "Main.h"
#include "DirectX.h"
#include "Texture.h"
#include "Input.h"
#include "Shader.h"
#include "Defines.h"
#include "FBX/FBXLoader.h"
#include "DrawBuffer.h"
#include "SceneManager.h"


// �萔��`
using namespace DirectX;

// �����W��ۑ����邩
#define CONTROL_NUM (100)

// ���t���[�����ɋO�Ղ̍��W����邩
#define RECORD_MARGIN (1)


#define POLYLINE_NUM (10)




// �O���[�o���ϐ�
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
	// FBX�̏�����
	ggfbx::Initialize();

	// DirectX
	hr = InitDX(hWnd, width, height, false);
	if (FAILED(hr)) { return ErrorBox(hr, "Failed to DirectX."); }



	// �L�[�{�[�h
	hr = InitKeyboard();
	if (FAILED(hr)) { return ErrorBox(hr, "Failed to Keyboard."); }



	// �V�F�[�_
	hr = SHADER->Init();
	if (FAILED(hr)) { return ErrorBox(hr, "Failed to Shader."); }



	//================�Q�[���Ŏg���I�u�W�F�N�g�̏�����================================

	// ���_�̏����������\����
	struct Vertex
	{
		// x,y,z
		float pos[3];

		// r,g,b,a
		float color[4];

		// u,v
		float uv[2];

	};

	// �|���S���̒��_���
	Vertex polyVtx[] =
	{
	{{0, 0,0},{1,1,1,1},{0,0}},	// ����
	{{1, 0,0},{1,1,1,1},{1,0} },	// �E��
	{{0,1,0},{1,1,1,1},{0,1}},	// ����
	{{1,1,0},{1,1,1,1},{1,1} },	// �E��
	};

	// ���_�o�b�t�@�̍쐬
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