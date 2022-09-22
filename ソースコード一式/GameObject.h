#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

// インクル
#include <DirectXMath.h>
#include "Cube.h"
#include "Shader.h"

using namespace DirectX;

// 当たり判定を実施する時のオブジェクトの種類
enum ObjectCollisionType
{
	// 静止している物体
	COLLISION_STATIC,

	// 動いている物体
	COLLISION_DYNAMIC,

	// 貫通する物体
	COLLISION_OVERLAP
};




class GameObject
{
public:
	// コンストラクタ
	GameObject();

	// デストラクタ
	virtual ~GameObject();

	// 四大処理
	virtual bool Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	// ゲッター
	XMFLOAT3 GetPos();
	XMFLOAT3 GetMove();
	XMFLOAT3 GetSize();
	// 今オブジェクトがどんなタイプなのかを判断するため
	ObjectCollisionType GetCollisionType();

	// セッター
	void SetPos(XMFLOAT3 pos);
	void SetSize(XMFLOAT3 size);
	void SetMove(XMFLOAT3 move);

private:

protected:
	Cube* m_pCube;
	XMFLOAT3 m_pos;
	XMFLOAT3 m_size;
	XMFLOAT3 m_move; // 移動量
	ObjectCollisionType m_collisionType;

};




#endif // !__GAME_OBJECT_H__
