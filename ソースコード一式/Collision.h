#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "GameObject.h"
#include <vector>

using namespace std;

// 定数
// 当たり判定を行う組み合わせの最大数
// 本当はリスト構造で制限のないほうがいい
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
	// AとB二つの当たり判定が1000通りある
	GameObject *m_pList[MAX_COLLISION_REGISTER][2];
	int m_registerNum;	// 登録した数

	

private:
	// 動いてるのと止まってるのの当たり判定
	void Push(GameObject *pDynamic, GameObject *pStatic);

};



#endif // !__COLLISION_H__
