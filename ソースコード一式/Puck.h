#pragma once
#include "model.h"
#include "CharacterBase.h"
#include "Shader.h"
#include <cmath>
#include "Stage.h"
#include "Player.h"
#include "Player2.h"
#include "MyCollision.h"
#include "Camera.h"
#include "Score.h"

#define MAX_PUCK (10)
#define PUCK_MASS (2.0f)
#define PUCK_SPEED (0.05f)
#define PUCK_RADIUS (0.5f)
#define MAX_PUCK_SPEED (1.5f)
#define MAX_STAGE (4)

enum PUCK_KIND
{
	NormalPuck,
	DoublePuck,
	GhostPuck,
	MiniPuck,
	BonusPuck,

	MaxPuck
};

enum StageKind
{
	NormalStage,
	TopUnderPairStage,
	NormalOnlyStage,
	ConveyorStage,
	AccelerationStage,
	TutorialStage,

	MaxStage
};

typedef struct
{
	XMFLOAT3 Pos;
	float Radius;
	bool HitConveyorToPuck;
	int Delay;
	float Move;

}ConveyorInfo;

typedef struct
{
	float Top;
	float Bottom;
	float Right;
	float Left;

}GoalInfo;

class Puck :public Model
{
public:
	Puck();
	Puck(Player*, Player2*);
	Puck(Player*, Player2*, Score* );
	~Puck();

	typedef struct
	{
		XMFLOAT3 CurPos;    // ���̈ʒu
		XMFLOAT3 PrePos;    // �ߋ��̈ʒu
		float Radian;        // ���W�A��
		float Radius;        // ���a
		XMFLOAT3 Move;        // ����
		float Mass;            // ����
		PUCK_KIND Kind;        // �p�b�N�̎��
		bool Exist;            // ���݂��邩���݂��Ȃ���
		int Score;            // ���_���̃p�b�N��
		float HitStopFrame;    // �q�b�g�X�g�b�v�����Ƃ��̃t���[��
		XMFLOAT3 Size;

	}PuckInfo;

	virtual bool Init();
	virtual void Uninit();
	virtual void Update();
	//virtual void Draw();

	XMFLOAT3 GetPuckPos();
	void SetPuckPos(XMFLOAT3 pos);
	void SetStage(StageKind kind);

	void CreatePuck(PUCK_KIND kind, XMFLOAT3 pos,XMFLOAT3 move);

	GameObject *GetPuck(int);
	PuckInfo *GetPuckInfo(int);
	ConveyorInfo *GetConveyorInfo();
	StageKind GetStage();

	void UpdateTutorial(int i);
	void UpdateNormalStage(int i);
	void UpdateTopUnderPairStage(int i);
	void UpdateConveyorStage(int i);
	void UpdateNormalOnlyStage(int i);
	void UpdateAccelerationStage(int i);


private:

	// FBX�t�@�C������N���X
	FBXPlayer m_fbx;

	DrawBuffer* m_pBuffer;

	PuckInfo* m_pPuckInfo;

	GameObject* m_pPuckData[MAX_PUCK];

	Player* m_pPlayer1;
	Player2* m_pPlayer2;

	Camera* m_pCamera;

	Score* m_pScore;

	StageKind m_StageKind;

	ConveyorInfo* m_pConveyorInfo;


};