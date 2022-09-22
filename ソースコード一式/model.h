#ifndef __MODEL_H__
#define __MODEL_H__

// インクルード
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
	// ゲームのキャラクターは複数のパーツで成り立っている
	// そのパーツ一つ一つをメッシュと呼ぶ
	// 基本的に一つのメッシュで一つの頂点バッファをもつ
	// モデルごとにメッシュの数が異なるため、頂点バッファを動的に確保する
	DrawBuffer* m_pBuffer;

	// FBXファイル操作クラス
	FBXPlayer m_fbx;

	Camera* m_pCamera;

	ID3D11ShaderResourceView* m_pPlayerTex;

	float XZangle;
};


#endif // !__MODEL_H__

