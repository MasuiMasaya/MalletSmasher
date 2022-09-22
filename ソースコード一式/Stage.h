#ifndef  __STAGE_H__
#define  __STAGE_H__

#include "Field.h"
#include "GameObject.h"



// ステージの横幅
#define STAGE_WIDTH (22.0f)
// ステージの縦幅
#define STAGE_HEIGHT (12.0f) 

// ステージの右側の上限
#define STAGE_RIGHT (STAGE_WIDTH/2.0f)
// ステージの左側の下限
#define	STAGE_LEFT (-STAGE_WIDTH/2.0f)
// ステージの上側の上限
#define STAGE_TOP (STAGE_HEIGHT/2.0f)
// ステージの下側の下限
#define STAGE_BOTTOM (-STAGE_HEIGHT/2.0f)




class Stage:public GameObject
{
public:
	Stage();
	~Stage();

	bool Init();
	void Uninit();
	void Update();
	void Draw();

	Field *GetField(int);
	int GetFieldNum();
	XMFLOAT3 GetStagePos();

private:
	Field **m_ppFields;
	int m_nFieldNum;

};






#endif // ! __STAGE_H__

