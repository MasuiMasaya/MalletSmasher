#include "SceneGame.h"
#include "SceneManager.h"
#include "Main.h"
#include "SoundEffect.h"


SceneManager* m_pSceneGameAddress;
ID3D11ShaderResourceView* g_pPlayerTex;
ID3D11ShaderResourceView* g_pPuckTex;
ID3D11ShaderResourceView* g_pStageTex;
IXAudio2SourceVoice *g_pSESpeaker;
IXAudio2SourceVoice* g_pStageBGMSpeaker;


// �萔��`
using namespace DirectX;

SceneGame::SceneGame()
{

}

SceneGame::~SceneGame()
{

}

HRESULT SceneGame::Init(HWND hWnd)
{
	// �V�[���A�h���X�̓���
	//-----------------------------------------------------------------------------
	m_pSceneGameAddress = GetSceneAddress();
	//-----------------------------------------------------------------------------


	// ������
	//-----------------------------------------------------------------------------
	m_pCamera = new Camera();
	m_pCamera->Init();

	m_pStage = new Stage();
	m_pStage->Init();

	m_pCollision = new Collision();
	m_pCollision->Init();

	m_pScore = m_pSceneGameAddress->GetTitle()->GetScore();
	m_pScore->GetGameEndScore();

	m_pStageModel = new StageModel();
	m_pStageModel->Init();
	m_pStageModel->LoadModel("Assets/Model/StageModel.fbx");

	m_pPlayer = new Player();
	m_pPlayer->Init();
	m_pPlayer->LoadModel("Assets/Model/Mallet_1P.fbx");

	m_pPlayer2 = new Player2();
	m_pPlayer2->Init();
	m_pPlayer2->LoadModel("Assets/Model/Mallet_2P.fbx");

	m_pPuck = new Puck(m_pPlayer,m_pPlayer2,m_pScore);
	m_pPuck->Init();
	m_pPuck->LoadModel("Assets/Model/Puck.fbx");

	m_pConveyorModel = new ConveyorModel(m_pPuck);
	m_pConveyorModel->Init();
	m_pConveyorModel->LoadModel("Assets/Model/Bumper.fbx");


	g_pStageBGMSpeaker = PlaySound(GetBGM(BGM_GAME_STAGE));
	//-----------------------------------------------------------------------------
	


	return S_OK;
}

void SceneGame::Uninit()
{
	m_pStage->Uninit();
	delete m_pStage;

	m_pStageModel->Uninit();
	delete m_pStageModel;

	m_pConveyorModel->Uninit();
	delete m_pConveyorModel;


	m_pPlayer->Uninit();
	delete m_pPlayer;

	m_pPlayer2->Uninit();
	delete m_pPlayer2;

	m_pPuck->Uninit();
	delete m_pPuck;

	m_pCollision->Uninit();
	delete m_pCollision;

	m_pCamera->Uninit();
	delete m_pCamera;

	m_pScore->Uninit();
	delete m_pScore;

	SAFE_RELEASE(g_pPlayerTex);
	SAFE_RELEASE(g_pPuckTex);
	SAFE_RELEASE(g_pStageTex);

	g_pStageBGMSpeaker->Stop();

}

void SceneGame::Update()
{
	if (m_pScore->GetScore_1P()->Score < m_pScore->GetGameEndScore() && m_pScore->GetScore_2P()->Score < m_pScore->GetGameEndScore())
	{

		UpdateKeyboard();

		m_pConveyorModel->Update();

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
	}
	else
	{
		// �V�[����ς���
		m_pSceneGameAddress->SetScene(Scene_Title);
	}


}

void SceneGame::Draw()
{
	BeginDrawDX();


	SHADER->Bind(VS_WORLD, PS_LAMBERT);

	//���̌����̐ݒ�
	SHADER->SetLightDir(XMFLOAT4(0.0f, 0.5f, -0.7f, 0.0f));

	// �����̐ݒ�
	SHADER->SetLightAmbient(XMFLOAT4(AMB, AMB, AMB, AMB));

	// �g�U���̐ݒ�
	SHADER->SetLightDiffuse(XMFLOAT4(DIF, DIF, DIF, DIF));

	// ���ʌ��̐ݒ�
	//SHADER->SetLightSpecular(XMFLOAT4(AMB, AMB, AMB, AMB));



	static float a = 0.0f;
	
	SHADER->SetWorld(DirectX::XMMatrixRotationY(a += 3.141592f / 180.0f));
	SHADER->SetWorld(DirectX::XMMatrixTranslation(0, 0, 0));


	// 3D�̕`��ݒ�
	m_pCamera->Bind();

	XMMATRIX cameraM = m_pCamera->GetView();
	XMMATRIX invCameraM = XMMatrixInverse(NULL, cameraM);


	// �X�e�[�W�̕`��
	m_pStage->Draw();

	m_pStageModel->Draw();

	if (m_pPuck->GetStage() == ConveyorStage)
	{
		m_pConveyorModel->Draw();
	}

	// �v���C���[�̕`��
	m_pPlayer->Draw();

	// �v���C���[�̕`��
	m_pPlayer2->Draw();

	// �p�b�N�̕`��
	m_pPuck->Draw();



	SHADER->Bind(VS_WORLD, PS_UNLIT);
	// 2D�̕`��ݒ�
	m_pCamera->Bind2D();

	m_pScore->Draw();




	DrawKeyboard();
	EndDrawDX();

}
