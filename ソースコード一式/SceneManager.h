#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

// インクルード
#include "DirectX.h"
#include "Defines.h"
#include "FBX/FBXLoader.h"

#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneResult.h"


// 列挙型
enum SCENE_KIND
{
	Scene_NONE,
	Scene_Title,
	Scene_Game,
	Scene_Result,

	Max_Scene

};


class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	HRESULT Init(HWND hWnd, UINT width, UINT height);
	void Uninit();
	void Update();
	void Draw();
	void SetScene(SCENE_KIND scene);
	SceneTitle* GetTitle() { return m_pSceneTitle; }

private:
	SceneTitle* m_pSceneTitle;
	SceneGame* m_pSceneGame;
	SceneResult* m_pSceneResult;
	SCENE_KIND m_Kind;

};

#endif // !__SCENEMANAGER_H__
