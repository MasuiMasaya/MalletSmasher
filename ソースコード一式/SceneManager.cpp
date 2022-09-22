#include "SceneManager.h"
#include "SoundEffect.h"

static SCENE_KIND g_scene = Scene_NONE;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
}


HRESULT SceneManager::Init(HWND hWnd, UINT width, UINT height)
{

	HRESULT hr = InitSound();
	if (FAILED(hr))
	{
		MessageBox(hWnd, "‰¹‚Ì‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½", "Error", MB_OK | MB_ICONERROR);
		return hr;
	}

	// ‰¹ŠÖŒW‚Ì‰Šú‰»
	//````````````
	InitSoundEffect();
	InitBGM();
	//````````````

	SetScene(Scene_Title);
	//SetScene(Scene_Game);

	return S_OK;
}

void SceneManager::Uninit()
{
	SetScene(Scene_NONE);


	//````````````
	UninitSoundEffect();
	UninitBGM();
	//````````````
}
void SceneManager::Update()
{
	switch (g_scene)
	{
	case Scene_Title:
		m_pSceneTitle->Update();
		break;
	case Scene_Game:
		m_pSceneGame->Update();
		break;
	case Scene_Result:
		m_pSceneResult->Update();
		break;
	}
}

void SceneManager::Draw()
{
	switch (g_scene)
	{
	case Scene_Title:
		m_pSceneTitle->Draw();
		break;
	case Scene_Game:
		m_pSceneGame->Draw();
		break;
	case Scene_Result:
		m_pSceneResult->Draw();
		break;
	}
}

void SceneManager::SetScene(SCENE_KIND scene)
{

	switch (g_scene)
	{
	case Scene_Title:
		m_pSceneTitle->Uninit();
		break;
	case Scene_Game:
		m_pSceneGame->Uninit();
		break;
	case Scene_Result:
		m_pSceneResult->Uninit();
		break;
	}


	ggfbx::Terminate();

	g_scene = scene;

	// FBX‚Ì‰Šú‰»
	ggfbx::Initialize();

	switch (g_scene)
	{
	case Scene_Title:
		m_pSceneTitle = new SceneTitle();
		m_pSceneTitle->Init();
		break;
	case Scene_Game:
		m_pSceneGame = new SceneGame();
		m_pSceneGame->Init(S_OK);
		break;
	case Scene_Result:
		m_pSceneResult = new SceneResult();
		m_pSceneResult->Init();
		break;
	}
}