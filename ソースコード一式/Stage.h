#ifndef  __STAGE_H__
#define  __STAGE_H__

#include "Field.h"
#include "GameObject.h"



// �X�e�[�W�̉���
#define STAGE_WIDTH (22.0f)
// �X�e�[�W�̏c��
#define STAGE_HEIGHT (12.0f) 

// �X�e�[�W�̉E���̏��
#define STAGE_RIGHT (STAGE_WIDTH/2.0f)
// �X�e�[�W�̍����̉���
#define	STAGE_LEFT (-STAGE_WIDTH/2.0f)
// �X�e�[�W�̏㑤�̏��
#define STAGE_TOP (STAGE_HEIGHT/2.0f)
// �X�e�[�W�̉����̉���
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

