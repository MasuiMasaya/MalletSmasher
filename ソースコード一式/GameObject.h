#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

// �C���N��
#include <DirectXMath.h>
#include "Cube.h"
#include "Shader.h"

using namespace DirectX;

// �����蔻������{���鎞�̃I�u�W�F�N�g�̎��
enum ObjectCollisionType
{
	// �Î~���Ă��镨��
	COLLISION_STATIC,

	// �����Ă��镨��
	COLLISION_DYNAMIC,

	// �ђʂ��镨��
	COLLISION_OVERLAP
};




class GameObject
{
public:
	// �R���X�g���N�^
	GameObject();

	// �f�X�g���N�^
	virtual ~GameObject();

	// �l�又��
	virtual bool Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	// �Q�b�^�[
	XMFLOAT3 GetPos();
	XMFLOAT3 GetMove();
	XMFLOAT3 GetSize();
	// ���I�u�W�F�N�g���ǂ�ȃ^�C�v�Ȃ̂��𔻒f���邽��
	ObjectCollisionType GetCollisionType();

	// �Z�b�^�[
	void SetPos(XMFLOAT3 pos);
	void SetSize(XMFLOAT3 size);
	void SetMove(XMFLOAT3 move);

private:

protected:
	Cube* m_pCube;
	XMFLOAT3 m_pos;
	XMFLOAT3 m_size;
	XMFLOAT3 m_move; // �ړ���
	ObjectCollisionType m_collisionType;

};




#endif // !__GAME_OBJECT_H__
