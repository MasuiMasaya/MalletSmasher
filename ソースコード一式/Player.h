#ifndef __PLAYER_H__
#define __PLAYER_H__


#include "CharacterBase.h"
#include "model.h"
#include "Camera.h"


// �萔��`
//----------------------------------------------------------------------
#define MALLET_RADIUS (0.75f)			// ���a
#define MALLET_MASS (5.0f)				// ����
#define MALLET_DEFAULT_POWER (0.9f)		// �}���b�g�̃f�t�H���g�̗�
#define PI (3.14159265359f)				// �~����
#define SNASH_POWER (1.5f)				// ��
#define MOVE (15.0f/60)					// 1�b(60FPS)��2m�i��
#define MALLET_SPEED (MOVE)				// �}���b�g�̃X�s�[�h
#define SMASH_CHARGE_FRAME    (45)		// �X�}�b�V���ɕK�v�ȗ��ߎ���    
#define SMASH_FRAME (5)					// �X�}�b�V�������t���[��
#define SMASH_SPEED (MALLET_SPEED*2.5f)	// �X�}�b�V���̎��̗�
#define GRAVITY (0.005f)
//----------------------------------------------------------------------

// �񋓑�
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

	// �\����
	typedef struct
	{
		XMFLOAT3 CurPos;			// ���݂̍��W
		XMFLOAT3 PrePos;			// �ߋ��̍��W
		float Radius;				// ���a
		XMFLOAT3 Move;				// �ړ���
		float Power;				// ��
		bool SmashButton;			// �X�}�b�V��
		int SmashFrame;				// �X�}�b�V���̗L���t���[��
		int SmashChargeFrame;		// �X�}�b�V���{�^�������Ă��鎞��
		float Mass;					// ����
		PlayerState State;			// �}���b�g�̏��
		int HitStopFrame;			// �}���b�g�̃q�b�g�X�g�b�v
		XMFLOAT3 Size;				// �T�C�Y

	}PlayerInfo;


	// �Q�b�^�[
	XMFLOAT3 GetPlayerPos();

	float GetCameraXZAngle();

	virtual bool Init();

	virtual void Uninit();

	// �I�[�o�[���C�h
	virtual void Update(Camera *pCamera);

	PlayerInfo *GetPlayer();

private:
	float m_CameraXZAngle;

	DrawBuffer* m_pBuffer;

	// FBX�t�@�C������N���X
	FBXPlayer m_fbx;

	PlayerInfo* m_pPlayerInfo;

};




#endif // !__PLAYER_H__
