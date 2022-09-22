#include "SceneTitle.h"
#include "SceneManager.h"
#include "Main.h"
#include "SoundEffect.h"

#define TITLE_WIDTH (700)
#define TITLE_HEIGHT (150)


SceneManager* m_pSceneTitleAddress;
ID3D11ShaderResourceView *g_pScoreTex;
ID3D11ShaderResourceView *g_pLineTex;
IXAudio2SourceVoice* g_pTitleBGMSpeaker;

SceneTitle::SceneTitle()
{
	SetScore = 1;
	SetStage = 0;
	PushSpace = false;
}
SceneTitle::~SceneTitle()
{

}


void SceneTitle::Init()
{
	//  �|���S���̏��
	//-----------------------------------------------------------------------------
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
	m_polygon.CreateVertexBuffer(polyVtx, sizeof(Vertex), _countof(polyVtx));
	//-----------------------------------------------------------------------------


	// �V�[���A�h���X�̓���
	//-----------------------------------------------------------------------------
	m_pSceneTitleAddress = GetSceneAddress();
	//-----------------------------------------------------------------------------


	// ������
	//-----------------------------------------------------------------------------
	m_pCamera = new Camera();
	m_pCamera->Init();

	m_pStage = new Stage();
	m_pStage->Init();

	m_pCollision = new Collision();
	m_pCollision->Init();

	m_pStageModel = new TitelStageModel();
	m_pStageModel->Init();
	m_pStageModel->LoadModel("Assets/Model/TitleStageModel.fbx");

	m_pPlayer = new Player();
	m_pPlayer->Init();
	m_pPlayer->LoadModel("Assets/Model/TitleMallet_1P.fbx");

	m_pPlayer2 = new Player2();
	m_pPlayer2->Init();
	m_pPlayer2->LoadModel("Assets/Model/TitleMallet_2P.fbx");

	m_pPuck = new Puck(m_pPlayer, m_pPlayer2);
	m_pPuck->Init();
	m_pPuck->SetStage(TutorialStage);
	m_pPuck->LoadModel("Assets/Model/TitlePuck.fbx");

	m_pScore = new Score();
	m_pScore->Init();

	m_MenuKind = PointMenu;


	g_pTitleBGMSpeaker = PlaySound(GetBGM(BGM_TUTORIAL_STAGE));
	g_pTitleBGMSpeaker->SetVolume(1.0f);
	//-----------------------------------------------------------------------------


	// �_���̃Z�b�g
	//-----------------------------------------------------------------------------
	m_pScore->SetGameEndScore(SetScore);
	//-----------------------------------------------------------------------------


	// �e�N�X�`���擾
	//-----------------------------------------------------------------------------
	LoadTextureFromFile("Assets/Image/brack.png", &m_pTitleBackTex);
	LoadTextureFromFile("Assets/Image/Title/malesuma.png", &m_pGameNameLog);
	LoadTextureFromFile("Assets/Image/Title/Pushspace.png", &m_pTitleLog);
	LoadTextureFromFile("Assets/Image/Title/Stage.png", &m_pTitleStartLog);
	LoadTextureFromFile("Assets/Image/Title/StartGame.png", &m_pStageLog);
	LoadTextureFromFile("Assets/Image/Title/Point.png", &m_pPointLog);
	LoadTextureFromFile("Assets/Image/Title/MainRule.png", &m_pMainRulesLog);
	LoadTextureFromFile("Assets/Image/number.png", &g_pScoreTex);
	LoadTextureFromFile("Assets/Image/Title/red.png", &g_pLineTex);
	//-----------------------------------------------------------------------------
	
}

void SceneTitle::Uninit()
{
	SAFE_RELEASE(m_pTitleBackTex);
	SAFE_RELEASE(m_pGameNameLog);
	SAFE_RELEASE(m_pTitleLog);
	SAFE_RELEASE(m_pTitleStartLog);
	SAFE_RELEASE(m_pStageLog);
	SAFE_RELEASE(m_pPointLog);
	SAFE_RELEASE(m_pMainRulesLog);
	SAFE_RELEASE(g_pScoreTex);
	SAFE_RELEASE(g_pLineTex);

	delete m_pCamera;

	m_pStage->Uninit();
	delete m_pStage;

	m_pStageModel->Uninit();
	delete m_pStageModel;

	m_pPlayer->Uninit();
	delete m_pPlayer;

	m_pPlayer2->Uninit();
	delete m_pPlayer2;

	m_pPuck->Uninit();
	delete m_pPuck;

	m_pCollision->Uninit();
	delete m_pCollision;

	g_pTitleBGMSpeaker->Stop();

	//delete m_pScore;

}

void SceneTitle::Update()
{
	UpdateKeyboard();

	// �`���[�g���A���X�e�[�W�̍X�V
	//-----------------------------------------------------------------------------
	m_pPlayer->Update(m_pCamera);

	m_pPlayer2->Update(m_pCamera);

	m_pPuck->Update();

	m_pStage->Update();

	XMFLOAT3 camerapos = m_pCamera->GetCameraPos();

	// �J�����̒����_���X�V
	m_pCamera->Setlook(m_pStage->GetPos());

	// �J�����̈ʒu�v�Z
	m_pCamera->CalculationCameraPos(m_pStage->GetPos());

	// �S�Ă̈ړ�(�X�V����)���I����Ă���S�ẴI�u�W�F�N�g�̓����蔻����s��
	for (int i = 0; i < m_pStage->GetFieldNum(); i++)
	{
		m_pCollision->Register(m_pPlayer, m_pStage->GetField(i));
		m_pCollision->Register(m_pPlayer2, m_pStage->GetField(i));
	}

	m_pCollision->Update();
	//-----------------------------------------------------------------------------


	// �L�[�{�[�h����
	//-----------------------------------------------------------------------------
	if (IsKeyTrigger(VK_SPACE))
	{
		PushSpace = true;
	}

	if (PushSpace == true)
	{
		if (IsKeyTrigger(VK_UP))
		{
			switch (m_MenuKind)
			{
			case PointMenu:
				m_MenuKind = GameStartMenu;
				break;
			case StageMenu:
				m_MenuKind = PointMenu;
				break;
			case GameStartMenu:
				m_MenuKind = StageMenu;
				break;
			default:
				break;
			}
		}

		if (IsKeyTrigger(VK_DOWN))
		{
			switch (m_MenuKind)
			{
			case PointMenu:
				m_MenuKind = StageMenu;
				break;
			case StageMenu:
				m_MenuKind = GameStartMenu;
				break;
			case GameStartMenu:
				m_MenuKind = PointMenu;
				break;
			default:
				break;
			}
		}

		if (IsKeyTrigger(VK_RIGHT))
		{
			switch (m_MenuKind)
			{
			case PointMenu:
				if (SetScore <= MAX_SCORE)
				{
					SetScore += 1;
				}
				else
				{
					SetScore = 1;
				}
				break;
			case StageMenu:

				if (SetStage <= MAX_STAGE)
				{
					SetStage += 1;
				}
				else
				{
					SetStage = 0;
				}

				break;
			case GameStartMenu:
				break;
			default:
				break;
			}
		}

		if (IsKeyTrigger(VK_LEFT))
		{
			switch (m_MenuKind)
			{
			case PointMenu:
				if (SetScore >= 2)
				{
					SetScore -= 1;
				}
				else
				{
					SetScore = 9;
				}
				break;
			case StageMenu:
				if (SetStage >= 1)
				{
					SetStage -= 1;
				}
				else
				{
					SetStage = 4;
				}
				break;
			case GameStartMenu:
				break;
			default:
				break;
			}
		}

		if (IsKeyTrigger(VK_RETURN))
		{
			PushSpace = false;
			// �V�[����ς���
			m_pSceneTitleAddress->SetScene(Scene_Game);
		}
	}
	m_pScore->SetGameEndScore(SetScore);

	//-----------------------------------------------------------------------------

}

void SceneTitle::Draw()
{
	BeginDrawDX();
	// �V�F�[�_
	SHADER->Bind(VS_WORLD, PS_UNLIT);

	SHADER->SetLightAmbient(XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f));

	SHADER->Bind(VS_WORLD, PS_LAMBERT);

	//���̌����̐ݒ�
	SHADER->SetLightDir(XMFLOAT4(0.0f, 0.5f, -0.7f, 0.0f));

	// �����̐ݒ�
	SHADER->SetLightAmbient(XMFLOAT4(AMB, AMB, AMB, AMB));

	// �g�U���̐ݒ�
	SHADER->SetLightDiffuse(XMFLOAT4(DIF, DIF, DIF, DIF));
	
	// --��������

	// �ŏI�I�ɉ�ʂɕ`�悳���܂łɁAGPU��
	// 1. ���_�����ۂɉ�ʂɕ`�悳���ʒu�̌v�Z
	// 2. �`�悳����ʂ̐F�̌v�Z���s����
	// ���̏������V�F�[�_�ƌĂ�( 1 �͒��_�V�F�[�_�A2 �̓s�N�Z���V�F�[�_)
	// ���_�o�b�t�@��GPU�ɓn�������ł͉�ʂɕ\�����ꂸ�A�ǂ̌v�Z�ŉ�ʂɕ\������̂�
	// (�V�F�[�_)��ݒ肷��K�v������


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

	// 3D�̕`��ݒ�
	m_pCamera->Bind();

	// �X�e�[�W�̕`��
	m_pStage->Draw();

	m_pStageModel->Draw();


	// �v���C���[�̕`��
	m_pPlayer->Draw();
	m_pPlayer2->Draw();

	// �p�b�N�̕`��
	m_pPuck->Draw();

	SHADER->Bind(VS_WORLD, PS_UNLIT);
	// 2D�̕`��ݒ�
	m_pCamera->Bind2D();


	// �^�C�g�����S�̔z�u
	//-----------------------------------------------------------------------------
	SHADER->SetWorld(XMMatrixScaling(750, 200, 1)*XMMatrixTranslation(1280/2-400, 550, 0));
	SHADER->SetTexture(m_pGameNameLog);
	m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	SHADER->SetTexture(NULL);
	//-----------------------------------------------------------------------------

	if (PushSpace == true)
	{
		// �Z���N�g��ʂ̔z�u
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(854, 480, 1)*XMMatrixTranslation(1280 * 0.5f - 854 * 0.5f, 720 * 0.5f - 480 * 0.5f, 0));
		SHADER->SetTexture(m_pTitleBackTex);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------

		//���[���̃��S
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.5f - 300 * 0.5f, 720 * 0.25f - 100 * 0.5f, 0));
		SHADER->SetTexture(m_pMainRulesLog);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------


		//�J�[�\���̕\��
		//-----------------------------------------------------------------------------
		switch (m_MenuKind)
		{
		case PointMenu:
			SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.25f - 300 * 0.25f, 720 * 0.5f - 150, 0));
			SHADER->SetTexture(g_pLineTex);
			m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			SHADER->SetTexture(NULL);

			break;
		case StageMenu:
			SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.25f - 300 * 0.25f, 720 * 0.75f - 180, 0));
			SHADER->SetTexture(g_pLineTex);
			m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			SHADER->SetTexture(NULL);
			break;
		case GameStartMenu:
			SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.5f - 300 * 0.5f, 720 * 0.75f - 100 * 0.5f, 0));
			SHADER->SetTexture(g_pLineTex);
			m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			SHADER->SetTexture(NULL);
			break;
		default:
			break;
		}
		//-----------------------------------------------------------------------------


		//���[���̃��S
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.5f - 300 * 0.5f, 720 * 0.75f - 100 * 0.5f, 0));
		SHADER->SetTexture(m_pStageLog);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------
		

		//�|�C���g�̃��S
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.25f - 300 * 0.25f, 720 * 0.5f - 150 , 0));
		SHADER->SetTexture(m_pPointLog);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------


		//�X�e�[�W�̃��S
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.25f - 300 * 0.25f, 720 * 0.75f - 180, 0));
		SHADER->SetTexture(m_pTitleStartLog);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------
				

		//�|�C���g�̐��l
		//-----------------------------------------------------------------------------
		SHADER->Bind(VS_TEXANIME, PS_PHONG);
		float work1 = (float)m_pScore->GetGameEndScore() * 0.1f;

		SHADER->SetWorld(XMMatrixScaling(80, 100, 1) * XMMatrixTranslation(800, 720 * 0.5f - 150, 0));
		SHADER->SetTextureScale(XMFLOAT2(0.1f, 1.0f));
		SHADER->SetTextureOffset(XMFLOAT2(work1, 0));
		SHADER->SetTexture(g_pScoreTex);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//-----------------------------------------------------------------------------


		//�X�e�[�W�̐��l
		//-----------------------------------------------------------------------------
		SHADER->Bind(VS_TEXANIME, PS_PHONG);
		float work2 = (float)SetStage * 0.1f;

		SHADER->SetWorld(XMMatrixScaling(80, 100, 1) * XMMatrixTranslation(800, 720 * 0.75f - 180, 0));
		SHADER->SetTextureScale(XMFLOAT2(0.1f, 1.0f));
		SHADER->SetTextureOffset(XMFLOAT2(work2, 0));
		SHADER->SetTexture(g_pScoreTex);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//-----------------------------------------------------------------------------


		SHADER->Bind(VS_WORLD, PS_UNLIT);		
		
	}



	DrawKeyboard();
	EndDrawDX();
}