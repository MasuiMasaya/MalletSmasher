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
	m_pPolygon.CreateVertexBuffer(polyVtx, sizeof(Vertex), _countof(polyVtx));


	m_pCamera = new Camera();
	m_pCamera->Init();

	//g_pPoly2D = new Polygon2D();

	g_pSceneR = GetSceneAddress();


	// �^�C�g�����S�̃e�N�X�`���擾
	LoadTextureFromFile("pushspace.png", &g_pResultTex);

	// �^�C�g�����S�̃e�N�X�`���擾
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


	// �V�F�[�_
	SHADER->Bind(VS_WORLD, PS_UNLIT);

	SHADER->SetLightAmbient(XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f));

	//==========================================================
	// 1,�I�u�W�F�N�g���ƂɃ��[�J�����W������(���_)
	// 2,���[�J�����W�����[���h���W�ɕϊ�
	// 3,���[���h���W���J�����̍��W�ɕϊ�
	// 4,�J�����̉�p�Ɏ��܂�͈͂ɕϊ�(�X�N���[�����W)
	//==========================================================
	static float a = 0.0f;

	// ���[���h���W�֕ϊ�
	// XMMatrixRotationX X���ŉ�]
	// XMMatrixRotationY Y���ŉ�]
	// XMMatrixRotationZ Z���ŉ�]
	// XMMatrixScaling �g�k
	// ���ꂼ��̕ϊ��s���DirectX���v�Z���Ă����
	SHADER->SetWorld(DirectX::XMMatrixRotationY(a += 3.141592f / 180.0f));
	SHADER->SetWorld(DirectX::XMMatrixTranslation(0, 0, 0));


	// 2D�̕`��ݒ�
	m_pCamera->Bind2D();

	// ���U���g�w�i
	SHADER->SetWorld(XMMatrixScaling(1280, 720, 1)*XMMatrixTranslation(0, 0, 0));

	SHADER->SetTexture(g_pResultUITex);
	m_pPolygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	SHADER->SetTexture(NULL);


	// �^�C�g�����S
	SHADER->SetWorld(XMMatrixScaling(Result_WIDTH, Result_HEIGHT, 1)*XMMatrixTranslation(SCREEN_WIDTH / 2 - Result_WIDTH / 2, SCREEN_HEIGHT - Result_HEIGHT, 0));

	SHADER->SetTexture(g_pResultTex);
	m_pPolygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	SHADER->SetTexture(NULL);




	DrawKeyboard();
	EndDrawDX();
}