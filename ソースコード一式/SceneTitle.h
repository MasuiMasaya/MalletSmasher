#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

#include "Camera.h"
#include "model.h"
#include "Texture.h"
#include "Polygon2D.h"
#include "Score.h"
#include "Puck.h"
#include "Player.h"
#include "Player2.h"
#include "Stage.h"
#include "TitelStageModel.h"
#include "Collision.h"


enum MenuKind
{
	PointMenu,
	StageMenu,
	GameStartMenu
};


class SceneTitle
{
public:
	SceneTitle();
	~SceneTitle();

	void Init();
	void Uninit();
	void Update();
	void Draw();
	Score * GetScore() { return m_pScore; }

private:
	DrawBuffer m_polygon;
	ID3D11ShaderResourceView* m_pTitleTex;
	ID3D11ShaderResourceView* m_pTitleTex2;
	Model* m_pModel;
	Camera* m_pCamera;
	Score* m_pScore;
	int SetScore;
	int SetStage;
	Player* m_pPlayer;
	Player2* m_pPlayer2;
	Stage* m_pStage;
	Collision* m_pCollision;
	Puck* m_pPuck;
	TitelStageModel* m_pStageModel;
	bool PushSpace;
	MenuKind m_MenuKind;



	ID3D11ShaderResourceView* m_pTitleBackTex;
	ID3D11ShaderResourceView* m_pGameNameLog;
	ID3D11ShaderResourceView* m_pTitleLog;
	ID3D11ShaderResourceView* m_pTitleStartLog;
	ID3D11ShaderResourceView* m_pStageLog;
	ID3D11ShaderResourceView* m_pPointLog;
	ID3D11ShaderResourceView* m_pMainRulesLog;

};

#endif // !__SCENE_TITLE_H__