#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "CharacterBase.h"
#include "model.h"
#include "Camera.h"


// 定数定義
//----------------------------------------------------------------------
#define MALLET_RADIUS (0.75f)			// 半径
#define MALLET_MASS (5.0f)				// 質量
#define MALLET_DEFAULT_POWER (0.9f)		// マレットのデフォルトの力
#define PI (3.14159265359f)				// 円周率
#define SNASH_POWER (1.5f)				// 力
#define MOVE (15.0f/60)					// 1秒(60FPS)で2m進む
#define MALLET_SPEED (MOVE)				// マレットのスピード
#define SMASH_CHARGE_FRAME    (45)		// スマッシュに必要な溜め時間    
#define SMASH_FRAME (5)					// スマッシュ持続フレーム
#define SMASH_SPEED (MALLET_SPEED*2.5f)	// スマッシュの時の力
#define GRAVITY (0.005f)
//----------------------------------------------------------------------

// 列挙体
enum PlayerState
{
	MalletActive,
	MalletNonActive
};

class Player :public Model
{
public:
	Player();
	virtual ~Player();

	// 構造体
	typedef struct
	{
		XMFLOAT3 CurPos;			// 現在の座標
		XMFLOAT3 PrePos;			// 過去の座標
		float Radius;				// 半径
		XMFLOAT3 Move;				// 移動量
		float Power;				// 力
		bool SmashButton;			// スマッシュ
		int SmashFrame;				// スマッシュの有効フレーム
		int SmashChargeFrame;		// スマッシュボタン押している時間
		float Mass;					// 質量
		PlayerState State;			// マレットの状態
		int HitStopFrame;			// マレットのヒットストップ
		XMFLOAT3 Size;				// サイズ

	}PlayerInfo;


	// ゲッター
	XMFLOAT3 GetPlayerPos();

	float GetCameraXZAngle();

	virtual bool Init();

	virtual void Uninit();

	// オーバーライド
	virtual void Update(Camera *pCamera);

	PlayerInfo *GetPlayer();

private:
	float m_CameraXZAngle;

	DrawBuffer* m_pBuffer;

	// FBXファイル操作クラス
	FBXPlayer m_fbx;

	PlayerInfo* m_pPlayerInfo;

};




#endif // !__PLAYER_H__
