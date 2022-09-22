#ifndef __MODEL_H__
#define __MODEL_H__

// �C���N���[�h
#include "DrawBuffer.h"
#include "FBX//FBXPlayer.h"
#include "GameObject.h"
#include "Camera.h"



class Model:public GameObject
{
public:
	Model();
	~Model();

	bool LoadModel(const char* pFilePath);
	bool Init();
	void Uninit();
	void Update();
	void Draw();
	void Draw(Camera* pCamera);

private:
	// �Q�[���̃L�����N�^�[�͕����̃p�[�c�Ő��藧���Ă���
	// ���̃p�[�c�������b�V���ƌĂ�
	// ��{�I�Ɉ�̃��b�V���ň�̒��_�o�b�t�@������
	// ���f�����ƂɃ��b�V���̐����قȂ邽�߁A���_�o�b�t�@�𓮓I�Ɋm�ۂ���
	DrawBuffer* m_pBuffer;

	// FBX�t�@�C������N���X
	FBXPlayer m_fbx;

	Camera* m_pCamera;

	ID3D11ShaderResourceView* m_pPlayerTex;

	float XZangle;
};


#endif // !__MODEL_H__

