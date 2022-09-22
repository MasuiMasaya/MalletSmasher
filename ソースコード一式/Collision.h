#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "GameObject.h"
#include <vector>

using namespace std;

// �萔
// �����蔻����s���g�ݍ��킹�̍ő吔
// �{���̓��X�g�\���Ő����̂Ȃ��ق�������
#define MAX_COLLISION_REGISTER (1000)


class Collision
{
public:
	Collision();
	~Collision();

	bool Init();
	void Uninit();
	bool Update();

	void Register(GameObject *pA, GameObject *pB);


private:
	// A��B��̓����蔻�肪1000�ʂ肠��
	GameObject *m_pList[MAX_COLLISION_REGISTER][2];
	int m_registerNum;	// �o�^������

	

private:
	// �����Ă�̂Ǝ~�܂��Ă�̂̓����蔻��
	void Push(GameObject *pDynamic, GameObject *pStatic);

};



#endif // !__COLLISION_H__
