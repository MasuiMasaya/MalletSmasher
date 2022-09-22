#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include"DirectX.h"
#include "FBX/FBXLoader.h"
#include "Player.h"
#include "Player2.h"
#include "Stage.h"
#include "Collision.h"
#include "Camera.h"
#include "Input.h"
#include "Cube.h"
#include "Puck.h"
#include "Score.h"
#include "StageModel.h"
#include "ConveyorModel.h"

#define AMB (0.8f)
#define DIF (0.7f)

class SceneGame
{
public:
	SceneGame();
	~SceneGame();

	HRESULT Init(HWND hWnd);
	void Uninit();
	void Update();
	void Draw();





private:
	Player* m_pPlayer;
	Player2* m_pPlayer2;
	Stage* m_pStage;
	Collision* m_pCollision;
	Camera* m_pCamera;
	Cube* m_pCube;
	Puck* m_pPuck;
	Score* m_pScore;
	StageModel* m_pStageModel;
	ConveyorModel* m_pConveyorModel;
	


};



#endif // !__SCENE_GAME_H__