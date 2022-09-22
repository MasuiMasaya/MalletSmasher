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


// 定数定義
using namespace DirectX;

SceneGame::SceneGame()
{

}

SceneGame::~SceneGame()
{

}

HRESULT SceneGame::Init(HWND hWnd)
{
	// シーンアドレスの入手
	//-----------------------------------------------------------------------------
	m_pSceneGameAddress = GetSceneAddress();
	//-----------------------------------------------------------------------------


	// 初期化
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

		// カメラの注視点を更新
		m_pCamera->Setlook(m_pStage->GetPos());

		// カメラの位置計算
		m_pCamera->CalculationCameraPos(m_pStage->GetPos());

		// 全ての移動(更新処理)が終わってから全てのオブジェクトの当たり判定を行う
		for (int i = 0; i < m_pStage->GetFieldNum(); i++)
		{
			m_pCollision->Register(m_pPlayer, m_pStage->GetField(i));
			m_pCollision->Register(m_pPlayer2, m_pStage->GetField(i));
		}

		m_pCollision->Update();
	}
	else
	{
		// シーンを変える
		m_pSceneGameAddress->SetScene(Scene_Title);
	}


}

void SceneGame::Draw()
{
	BeginDrawDX();


	SHADER->Bind(VS_WORLD, PS_LAMBERT);

	//光の向きの設定
	SHADER->SetLightDir(XMFLOAT4(0.0f, 0.5f, -0.7f, 0.0f));

	// 環境光の設定
	SHADER->SetLightAmbient(XMFLOAT4(AMB, AMB, AMB, AMB));

	// 拡散光の設定
	SHADER->SetLightDiffuse(XMFLOAT4(DIF, DIF, DIF, DIF));

	// 鏡面光の設定
	//SHADER->SetLightSpecular(XMFLOAT4(AMB, AMB, AMB, AMB));



	static float a = 0.0f;
	
	SHADER->SetWorld(DirectX::XMMatrixRotationY(a += 3.141592f / 180.0f));
	SHADER->SetWorld(DirectX::XMMatrixTranslation(0, 0, 0));


	// 3Dの描画設定
	m_pCamera->Bind();

	XMMATRIX cameraM = m_pCamera->GetView();
	XMMATRIX invCameraM = XMMatrixInverse(NULL, cameraM);


	// ステージの描画
	m_pStage->Draw();

	m_pStageModel->Draw();

	if (m_pPuck->GetStage() == ConveyorStage)
	{
		m_pConveyorModel->Draw();
	}

	// プレイヤーの描画
	m_pPlayer->Draw();

	// プレイヤーの描画
	m_pPlayer2->Draw();

	// パックの描画
	m_pPuck->Draw();



	SHADER->Bind(VS_WORLD, PS_UNLIT);
	// 2Dの描画設定
	m_pCamera->Bind2D();

	m_pScore->Draw();




	DrawKeyboard();
	EndDrawDX();

}
