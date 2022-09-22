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
	//  ポリゴンの情報
	//-----------------------------------------------------------------------------
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
	m_polygon.CreateVertexBuffer(polyVtx, sizeof(Vertex), _countof(polyVtx));
	//-----------------------------------------------------------------------------


	// シーンアドレスの入手
	//-----------------------------------------------------------------------------
	m_pSceneTitleAddress = GetSceneAddress();
	//-----------------------------------------------------------------------------


	// 初期化
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


	// 点数のセット
	//-----------------------------------------------------------------------------
	m_pScore->SetGameEndScore(SetScore);
	//-----------------------------------------------------------------------------


	// テクスチャ取得
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

	// チュートリアルステージの更新
	//-----------------------------------------------------------------------------
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
	//-----------------------------------------------------------------------------


	// キーボード操作
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
			// シーンを変える
			m_pSceneTitleAddress->SetScene(Scene_Game);
		}
	}
	m_pScore->SetGameEndScore(SetScore);

	//-----------------------------------------------------------------------------

}

void SceneTitle::Draw()
{
	BeginDrawDX();
	// シェーダ
	SHADER->Bind(VS_WORLD, PS_UNLIT);

	SHADER->SetLightAmbient(XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f));

	SHADER->Bind(VS_WORLD, PS_LAMBERT);

	//光の向きの設定
	SHADER->SetLightDir(XMFLOAT4(0.0f, 0.5f, -0.7f, 0.0f));

	// 環境光の設定
	SHADER->SetLightAmbient(XMFLOAT4(AMB, AMB, AMB, AMB));

	// 拡散光の設定
	SHADER->SetLightDiffuse(XMFLOAT4(DIF, DIF, DIF, DIF));
	
	// --ここから

	// 最終的に画面に描画されるまでに、GPUで
	// 1. 頂点が実際に画面に描画される位置の計算
	// 2. 描画される画面の色の計算が行われる
	// この処理をシェーダと呼ぶ( 1 は頂点シェーダ、2 はピクセルシェーダ)
	// 頂点バッファをGPUに渡すだけでは画面に表示されず、どの計算で画面に表示するのか
	// (シェーダ)を設定する必要もある


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

	// 3Dの描画設定
	m_pCamera->Bind();

	// ステージの描画
	m_pStage->Draw();

	m_pStageModel->Draw();


	// プレイヤーの描画
	m_pPlayer->Draw();
	m_pPlayer2->Draw();

	// パックの描画
	m_pPuck->Draw();

	SHADER->Bind(VS_WORLD, PS_UNLIT);
	// 2Dの描画設定
	m_pCamera->Bind2D();


	// タイトルロゴの配置
	//-----------------------------------------------------------------------------
	SHADER->SetWorld(XMMatrixScaling(750, 200, 1)*XMMatrixTranslation(1280/2-400, 550, 0));
	SHADER->SetTexture(m_pGameNameLog);
	m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	SHADER->SetTexture(NULL);
	//-----------------------------------------------------------------------------

	if (PushSpace == true)
	{
		// セレクト画面の配置
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(854, 480, 1)*XMMatrixTranslation(1280 * 0.5f - 854 * 0.5f, 720 * 0.5f - 480 * 0.5f, 0));
		SHADER->SetTexture(m_pTitleBackTex);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------

		//ルールのロゴ
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.5f - 300 * 0.5f, 720 * 0.25f - 100 * 0.5f, 0));
		SHADER->SetTexture(m_pMainRulesLog);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------


		//カーソルの表示
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


		//ルールのロゴ
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.5f - 300 * 0.5f, 720 * 0.75f - 100 * 0.5f, 0));
		SHADER->SetTexture(m_pStageLog);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------
		

		//ポイントのロゴ
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.25f - 300 * 0.25f, 720 * 0.5f - 150 , 0));
		SHADER->SetTexture(m_pPointLog);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------


		//ステージのロゴ
		//-----------------------------------------------------------------------------
		SHADER->SetWorld(XMMatrixScaling(300, 100, 1)*XMMatrixTranslation(1280 * 0.25f - 300 * 0.25f, 720 * 0.75f - 180, 0));
		SHADER->SetTexture(m_pTitleStartLog);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		SHADER->SetTexture(NULL);
		//-----------------------------------------------------------------------------
				

		//ポイントの数値
		//-----------------------------------------------------------------------------
		SHADER->Bind(VS_TEXANIME, PS_PHONG);
		float work1 = (float)m_pScore->GetGameEndScore() * 0.1f;

		SHADER->SetWorld(XMMatrixScaling(80, 100, 1) * XMMatrixTranslation(800, 720 * 0.5f - 150, 0));
		SHADER->SetTextureScale(XMFLOAT2(0.1f, 1.0f));
		SHADER->SetTextureOffset(XMFLOAT2(work1, 0));
		SHADER->SetTexture(g_pScoreTex);
		m_polygon.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		//-----------------------------------------------------------------------------


		//ステージの数値
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