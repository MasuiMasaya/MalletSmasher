 #include "Puck.h"
#include "SoundEffect.h"
#include "myVector.h"

using namespace DirectX;

GoalInfo g_StageData;
IXAudio2SourceVoice *g_pPuckSESpeaker;

// 関数
void CircleColProcess(Player::PlayerInfo *a,Puck::PuckInfo *b);
void CircleColProcess(Player2::Player2Info *a, Puck::PuckInfo *b);
void PuckColProcess(Puck:: PuckInfo *a, Puck::PuckInfo *b);

Puck::Puck()
{
	m_pos = XMFLOAT3(0.5f, 0.0f, 0.0f);

	m_StageKind = NormalStage;

}

Puck::Puck(Player* p1, Player2* p2)
{
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pPlayer1 = p1;
	m_pPlayer2 = p2;
}

Puck::Puck(Player* p1, Player2* p2,Score* p3 )
{
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pPlayer1 = p1;
	m_pPlayer2 = p2;
	m_pScore = p3;
}

Puck::~Puck()
{

}

bool Puck::Init()
{
	// 動的確保
	//---------------------------------------------------------------
	m_pPuckInfo = new PuckInfo [MAX_PUCK];

	for (int i = 0; i < MAX_PUCK; i++)
	{
		m_pPuckData[i] = GetPuck(i);
	}

	m_pConveyorInfo = new ConveyorInfo();

	//--------------------------------------------------------------


	//初期化
	//---------------------------------------------------------------
	m_pPlayer1->Init();
	m_pPlayer2->Init();
	m_pPlayer1->GetPlayer();
	m_pPlayer2->GetPlayer2();

	m_pConveyorInfo->Pos = XMFLOAT3(0.0f, 1.25f, 3.0f);
	m_pConveyorInfo->Radius = 0.5f;
	m_pConveyorInfo->HitConveyorToPuck = true;
	m_pConveyorInfo->Delay = 0;
	m_pConveyorInfo->Move = -0.1f;


	m_StageKind = NormalStage;
	//m_StageKind = ConveyorStage;

	// 音関係の初期化
	//～～～～～～～～～～～～
	InitSoundEffect();
	//～～～～～～～～～～～～
	//---------------------------------------------------------------


	// パックの初期化
	//---------------------------------------------------------------
	m_pos = XMFLOAT3(0.0f, 1.25f, 0.0f);
	m_size = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_move.x = 0;
	m_move.y = 0;
	m_move.z = 0;
	//---------------------------------------------------------------


	// パックの情報の初期化
	//---------------------------------------------------------------
	for (int i = 0; i < MAX_PUCK; i++)
	{
		m_pPuckInfo[i].CurPos = m_pos;
		m_pPuckInfo[i].Move = m_move;
		m_pPuckInfo[i].PrePos = m_pPuckInfo[i].CurPos;
		m_pPuckInfo[i].Radius = PUCK_RADIUS;
		m_pPuckInfo[i].Mass = PUCK_MASS;
		m_pPuckInfo[i].Kind = NormalPuck;
		m_pPuckInfo[i].Score = 1;
		m_pPuckInfo[i].Exist = false;
		m_pPuckInfo[i].HitStopFrame = 0.0f;
		m_pPuckInfo[i].Size = m_size;
	}
	m_pPuckInfo[0].Exist = true;
	//---------------------------------------------------------------


	// ステージデータの初期化
	//--------------------------------------
	g_StageData.Top = (PUCK_RADIUS * 2 + MALLET_RADIUS);
	g_StageData.Bottom = (PUCK_RADIUS * 2 + MALLET_RADIUS)*-1;
	g_StageData.Right = STAGE_WIDTH / 2;
	g_StageData.Left = (STAGE_WIDTH / 2)*-1;
	//--------------------------------------

	return true;
}
void Puck::Uninit()
{
	delete[] m_pPuckInfo;
}	
void Puck::Update()
{
	// プレイヤーの情報をもらう
	//---------------------------------------------------------------
	m_pPlayer1->GetPlayer();
	m_pPlayer2->GetPlayer2();
	//---------------------------------------------------------------


	// 更新処理
	//---------------------------------------------------------------
	for (int i = 0; i < MAX_PUCK; i++)
	{
		// 使っていないのは処理しない
		//---------------------------------------------------------------
		if (m_pPuckInfo[i].Exist == false)
		{
			continue;
		}
		//---------------------------------------------------------------


		// 座標の更新
		//---------------------------------------------------------------
		m_pPuckInfo[i].PrePos = m_pPuckInfo[i].CurPos;
		m_pPuckInfo[i].CurPos.x += m_pPuckInfo[i].Move.x;
		m_pPuckInfo[i].CurPos.z += m_pPuckInfo[i].Move.z;
		//---------------------------------------------------------------


		// 加速制限
		//---------------------------------------------------------------
		if (m_pPuckInfo[i].Move.z > MAX_PUCK_SPEED)
		{
			m_pPuckInfo[i].Move.z = MAX_PUCK_SPEED;
		}
		//---------------------------------------------------------------
		

		// ステージに合わせて壁との当たり判定を切り替える
		//---------------------------------------------------------------
		switch (m_StageKind)
		{
		case NormalStage:
			UpdateNormalStage(i);
			break;
		case TopUnderPairStage:
			UpdateTopUnderPairStage(i);
			break;
		case NormalOnlyStage:
			UpdateNormalOnlyStage(i);
			break;
		case ConveyorStage:
			UpdateConveyorStage(i);
			break;
		case AccelerationStage:
			UpdateAccelerationStage(i);
			break;
		case MaxStage:
			break;
		case TutorialStage:
			UpdateTutorial(i);
			break;
		default:
			break;
		}
		//---------------------------------------------------------------

		// 現在の位置の更新
		//---------------------------------------------------------------
		m_pos = m_pPuckInfo[i].CurPos;
		m_move = m_pPuckInfo[i].Move;
		m_size = m_pPuckInfo[i].Size;
		//---------------------------------------------------------------


		// マレット1Pとパックの当たり判定
		//--------------------------------------------------------------
		if (CheckCollisionCircleToCircle(m_pPlayer1->GetPlayer()->CurPos.x, m_pPlayer1->GetPlayer()->CurPos.z, m_pPuckInfo[i].CurPos.x, m_pPuckInfo[i].CurPos.z, m_pPlayer1->GetPlayer()->Radius, m_pPuckInfo[i].Radius))
		{
			myVector2 vec1(m_pPlayer1->GetPlayer()->PrePos.x - m_pPlayer1->GetPlayer()->CurPos.x, m_pPlayer1->GetPlayer()->PrePos.z - m_pPlayer1->GetPlayer()->CurPos.z);
			myVector2 vec2(m_pPuckInfo[i].PrePos.x - m_pPuckInfo[i].CurPos.x, m_pPuckInfo[i].PrePos.z - m_pPuckInfo[i].CurPos.z);
			myVector2 vec3 = vec2 - vec1;
			float rad = atan2f(m_pPuckInfo[i].PrePos.z - m_pPlayer1->GetPlayer()->PrePos.z, m_pPuckInfo[i].PrePos.x - m_pPlayer1->GetPlayer()->PrePos.x);
			float SeVol = vec3.x*cosf(rad) + vec3.y * sinf(rad);
			g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToPuck));
			//g_pPuckSESpeaker->SetVolume(1.0f);

			CircleColProcess(m_pPlayer1->GetPlayer(), &m_pPuckInfo[i]);
		}
		//---------------------------------------------------------------


		// マレット2Pとパックの当たり判定
		//--------------------------------------------------------------
		if (CheckCollisionCircleToCircle(m_pPlayer2->GetPlayer2()->CurPos.x, m_pPlayer2->GetPlayer2()->CurPos.z, m_pPuckInfo[i].CurPos.x, m_pPuckInfo[i].CurPos.z, m_pPlayer1->GetPlayer()->Radius, m_pPuckInfo[i].Radius))
		{
			myVector2 vec1(m_pPlayer2->GetPlayer2()->PrePos.x - m_pPlayer2->GetPlayer2()->CurPos.x, m_pPlayer2->GetPlayer2()->PrePos.z - m_pPlayer2->GetPlayer2()->CurPos.z);
			myVector2 vec2(m_pPuckInfo[i].PrePos.x - m_pPuckInfo[i].CurPos.x, m_pPuckInfo[i].PrePos.z - m_pPuckInfo[i].CurPos.z);
			myVector2 vec3 = vec2 - vec1;
			float rad = atan2f(m_pPuckInfo[i].PrePos.z - m_pPlayer2->GetPlayer2()->PrePos.z, m_pPuckInfo[i].PrePos.x - m_pPlayer2->GetPlayer2()->PrePos.x);
			float SeVol = vec3.x*cosf(rad) + vec3.y * sinf(rad);
			//g_pPuckSESpeaker->SetVolume(SeVol / MALLET_SPEED);
			g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToPuck));

			CircleColProcess(m_pPlayer2->GetPlayer2(), &m_pPuckInfo[i]);
		}
		//---------------------------------------------------------------


		//  パック同士の当たり判定
		//--------------------------------------------------------------
		for (int j = i + 1; j < MAX_PUCK; j++)
		{
			if (m_pPuckInfo[j].Exist == false)
			{
				continue;
			}
			if (CheckCollisionCircleToCircle(m_pPuckInfo[i].CurPos.x,m_pPuckInfo[i].CurPos.z,m_pPuckInfo[j].CurPos.x,m_pPuckInfo[j].CurPos.z,m_pPuckInfo[i].Radius,m_pPuckInfo[j].Radius))
			{
				PuckColProcess(&m_pPuckInfo[i], &m_pPuckInfo[j]);
			}
		}

		//---------------------------------------------------------------

		// パックの位置の最低保証
		//---------------------------------------------------------------
		if (m_pPuckInfo[i].CurPos.y <= -1.0f)
		{
			m_pPuckInfo[i].CurPos = XMFLOAT3(0.25f, 1.25f, 0.0f);
			m_pos = m_pPuckInfo[i].CurPos;
			m_pPuckInfo[i].Move.y = 0.0f;
		}
		//---------------------------------------------------------------
	}
	//---------------------------------------------------------------
}

XMFLOAT3 Puck::GetPuckPos()
{
	return m_pos;
}
void Puck::SetPuckPos(XMFLOAT3 pos)
{
	m_pos = pos;
}

void Puck::SetStage(StageKind kind)
{
	m_StageKind = kind;
}

void Puck::CreatePuck(PUCK_KIND kind, XMFLOAT3 pos,XMFLOAT3 move)
{
	switch (kind)
	{
	case NormalPuck:
		m_pPuckInfo[0].CurPos = pos;
		m_pPuckInfo[0].Move = move;
		m_pPuckInfo[0].PrePos = m_pPuckInfo[0].CurPos;
		m_pPuckInfo[0].Radius = PUCK_RADIUS;
		m_pPuckInfo[0].Mass = PUCK_MASS;
		m_pPuckInfo[0].Kind = NormalPuck;
		m_pPuckInfo[0].Score = 1;
		m_pPuckInfo[0].Exist = true;
		m_pPuckInfo[0].HitStopFrame = 0.0f;
		m_pPuckInfo[0].Size = m_size;

		m_pPuckInfo[1].Exist = false;
		break;
	case DoublePuck:
		m_pPuckInfo[0].CurPos = pos;
		m_pPuckInfo[0].Move = move;
		m_pPuckInfo[0].PrePos = m_pPuckInfo[0].CurPos;
		m_pPuckInfo[0].Radius = PUCK_RADIUS;
		m_pPuckInfo[0].Mass = PUCK_MASS;
		m_pPuckInfo[0].Kind = DoublePuck;
		m_pPuckInfo[0].Score = 1;
		m_pPuckInfo[0].Exist = true;
		m_pPuckInfo[0].HitStopFrame = 0.0f;
		m_pPuckInfo[0].Size = m_size;

		m_pPuckInfo[1].CurPos = pos;
		m_pPuckInfo[1].Move = move;

		m_pPuckInfo[1].CurPos.x = pos.z*-1.0f;
		m_pPuckInfo[1].Move.x = move.z*-1.0f;
		m_pPuckInfo[1].PrePos = m_pPuckInfo[1].CurPos;
		m_pPuckInfo[1].Radius = PUCK_RADIUS;
		m_pPuckInfo[1].Mass = PUCK_MASS;
		m_pPuckInfo[1].Kind = DoublePuck;
		m_pPuckInfo[1].Score = 1;
		m_pPuckInfo[1].Exist = true;
		m_pPuckInfo[1].HitStopFrame = 0.0f;
		m_pPuckInfo[1].Size = m_size;
		break;
	case GhostPuck:
		break;
	case MiniPuck:
		m_pPuckInfo[0].CurPos = pos;
		m_pPuckInfo[0].Move = move;
		m_pPuckInfo[0].PrePos = m_pPuckInfo[0].CurPos;
		m_pPuckInfo[0].Radius = PUCK_RADIUS*0.5f;
		m_pPuckInfo[0].Mass = PUCK_MASS*0.5f;
		m_pPuckInfo[0].Kind = MiniPuck;
		m_pPuckInfo[0].Score = 1;
		m_pPuckInfo[0].Exist = true;
		m_pPuckInfo[0].HitStopFrame = 0.0f;
		m_pPuckInfo[0].Size = XMFLOAT3(0.5f,0.5f,0.5f);

		m_pPuckInfo[1].Exist = false;
		break;
	case BonusPuck:
		m_pPuckInfo[0].CurPos = pos;
		m_pPuckInfo[0].Move = move;
		m_pPuckInfo[0].PrePos = m_pPuckInfo[0].CurPos;
		m_pPuckInfo[0].Radius = PUCK_RADIUS;
		m_pPuckInfo[0].Mass = PUCK_MASS;
		m_pPuckInfo[0].Kind = BonusPuck;
		m_pPuckInfo[0].Score = 3;
		m_pPuckInfo[0].Exist = true;
		m_pPuckInfo[0].HitStopFrame = 0.0f;
		m_pPuckInfo[0].Size = m_size;

		m_pPuckInfo[1].Exist = false;
		break;
	default:
		break;
	}
}

GameObject *Puck::GetPuck(int index)
{

	if (index < MAX_PUCK)
	{
		return m_pPuckData[index];
	}

	return NULL;
}


Puck::PuckInfo *Puck::GetPuckInfo(int index)
{
	for (index; index < MAX_PUCK;index)
	{
		if (m_pPuckInfo[index].Exist)
		{
			return &m_pPuckInfo[index];
		}
	}
	return nullptr;
}


ConveyorInfo *Puck::GetConveyorInfo()
{
	return m_pConveyorInfo;
}

StageKind Puck::GetStage()
{
	return m_StageKind;
}

void CircleColProcess(Player::PlayerInfo *a, Puck::PuckInfo *b)
{
	// 無いパックの当たり判定を取らないようにする処置
	//---------------------------------------------------------------
	if (b->Exist == false)
	{
		return;
	}
	//---------------------------------------------------------------

	float aMoveX;    // aの移動量X
	float aMoveY;    // 

	// 移動した分をマレットの移動ベクトルとする
	//---------------------------------------------------------------
	aMoveX = (a->CurPos.x - a->PrePos.x)*a->Power;
	aMoveY = (a->CurPos.z - a->PrePos.z)*a->Power;
	//---------------------------------------------------------------


	//--- めり込んだ分パックを移動する
	//---------------------------------------------------------------


	// めり込んだ量を調べる
	//---------------------------------------------------------------
	float vx = (a->CurPos.x - b->CurPos.x);
	float vy = (a->CurPos.z - b->CurPos.z);
	float length = sqrtf(vx*vx + vy * vy);
	float distance = (a->Radius + b->Radius) - length;
	//---------------------------------------------------------------


	// aの補正方向を調べる
	//---------------------------------------------------------------
	if (length > 0)
		length = 1.0f / length;
	distance *= 0.5f;
	distance *= 1.001f;
	vx *= length;
	vy *= length;
	//---------------------------------------------------------------


	// aをめり込まない位置まで補正
	//---------------------------------------------------------------
	a->CurPos.x += vx * distance;
	a->CurPos.z += vy * distance;
	//---------------------------------------------------------------


	// bをめり込まない位置まで補正する
	//---------------------------------------------------------------
	b->CurPos.x -= vx * distance;
	b->CurPos.z-= vy * distance;
	//---------------------------------------------------------------


	//--- 衝突後の方向を調べる
	//---------------------------------------------------------------
	float t;    // 方向
	float arX;    // aがbに与える移動運動X方向
	float arY;
	float amX;    // aがbに与える回転運動X方向
	float amY;
	float brX;    // bがaに与える移動運動X方向
	float brY;
	float bmX;    // bがaに与える回転運動
	float bmY;

	vx *= -1;
	vy *= -1;
	//---------------------------------------------------------------


	// 移動運動
	//---------------------------------------------------------------
	t = -(vx * aMoveX + vy * aMoveY) / (vx * vx + vy * vy);
	arX = aMoveX + vx * t;
	arY = aMoveY + vy * t;
	//---------------------------------------------------------------


	// 回転運動
	//---------------------------------------------------------------
	t = -(-vy * aMoveX + vx * aMoveY) / (vy * vy + vx * vx);
	amX = aMoveX - vy * t;
	amY = aMoveY + vx * t;
	//---------------------------------------------------------------


	// 移動運動
	//---------------------------------------------------------------
	t = -(vx * b->Move.x + vy * b->Move.z) / (vx * vx + vy * vy);
	brX = b->Move.x + vx * t;
	brY = b->Move.z + vy * t;
	//---------------------------------------------------------------


	// 回転運動
	//---------------------------------------------------------------
	t = -(-vy * b->Move.x + vx * b->Move.z) / (vy * vy + vx * vx);
	bmX = b->Move.x - vy * t;
	bmY = b->Move.z + vx * t;
	//---------------------------------------------------------------


	//--- 反発させる
	//---------------------------------------------------------------
	float e = 1.0f;        // 反発係数
	float avX;            // 反発後のaの速度X方向
	float avY;
	float bvX;            // 反発後のbの速度X方向
	float bvY;

	avX = (a->Mass * amX + b->Mass * bmX + bmX * e * b->Mass - amX * e * b->Mass) / (a->Mass + b->Mass);
	avY = (a->Mass * amY + b->Mass * bmY + bmY * e * b->Mass - amY * e * b->Mass) / (a->Mass + b->Mass);
	bvX = -e * (bmX - amX) + avX;
	bvY = -e * (bmY - amY) + avY;
	//---------------------------------------------------------------


	// 速度反映
	//---------------------------------------------------------------
	//a->movex = avX + arX;
	//a->movey = avY + arY;
	b->Move.x = bvX + brX;
	b->Move.z = bvY + brY;
	//---------------------------------------------------------------

}

void CircleColProcess(Player2::Player2Info *a, Puck::PuckInfo *b)
{
	// 無いパックの当たり判定を取らないようにする処置
	if (b->Exist == false)
	{
		return;
	}

	float aMoveX;    // aの移動量X
	float aMoveY;    // 
	// 移動した分をマレットの移動ベクトルとする
	aMoveX = (a->CurPos.x - a->PrePos.x)*a->Power;
	aMoveY = (a->CurPos.z - a->PrePos.z)*a->Power;

	//--- めり込んだ分パックを移動する
	// めり込んだ量を調べる
	float vx = (a->CurPos.x - b->CurPos.x);
	float vy = (a->CurPos.z - b->CurPos.z);
	float length = sqrtf(vx*vx + vy * vy);
	float distance = (a->Radius + b->Radius) - length;
	// aの補正方向を調べる
	if (length > 0)
		length = 1.0f / length;
	distance *= 0.5f;
	distance *= 1.001f;
	vx *= length;
	vy *= length;
	// aをめり込まない位置まで補正
	a->CurPos.x += vx * distance;
	a->CurPos.z += vy * distance;
	// bをめり込まない位置まで補正する
	b->CurPos.x -= vx * distance;
	b->CurPos.z -= vy * distance;

	//--- 衝突後の方向を調べる
	float t;    // 方向
	float arX;    // aがbに与える移動運動X方向
	float arY;
	float amX;    // aがbに与える回転運動X方向
	float amY;
	float brX;    // bがaに与える移動運動X方向
	float brY;
	float bmX;    // bがaに与える回転運動
	float bmY;

	vx *= -1;
	vy *= -1;


	// 移動運動
	t = -(vx * aMoveX + vy * aMoveY) / (vx * vx + vy * vy);
	arX = aMoveX + vx * t;
	arY = aMoveY + vy * t;

	// 回転運動
	t = -(-vy * aMoveX + vx * aMoveY) / (vy * vy + vx * vx);
	amX = aMoveX - vy * t;
	amY = aMoveY + vx * t;

	// 移動運動
	t = -(vx * b->Move.x + vy * b->Move.z) / (vx * vx + vy * vy);
	brX = b->Move.x + vx * t;
	brY = b->Move.z + vy * t;

	// 回転運動
	t = -(-vy * b->Move.x + vx * b->Move.z) / (vy * vy + vx * vx);
	bmX = b->Move.x - vy * t;
	bmY = b->Move.z + vx * t;

	//--- 反発させる
	float e = 1.0f;        // 反発係数
	float avX;            // 反発後のaの速度X方向
	float avY;
	float bvX;            // 反発後のbの速度X方向
	float bvY;

	avX = (a->Mass * amX + b->Mass * bmX + bmX * e * b->Mass - amX * e * b->Mass) / (a->Mass + b->Mass);
	avY = (a->Mass * amY + b->Mass * bmY + bmY * e * b->Mass - amY * e * b->Mass) / (a->Mass + b->Mass);
	bvX = -e * (bmX - amX) + avX;
	bvY = -e * (bmY - amY) + avY;

	// 速度反映
	//a->movex = avX + arX;
	//a->movey = avY + arY;
	b->Move.x = bvX + brX;
	b->Move.z = bvY + brY;

}


// 衝突時に呼ぶ
// aをパック bをパックとする
void PuckColProcess(Puck::PuckInfo *a, Puck::PuckInfo *b)
{
	// 無いパックの当たり判定を取らないようにする処置
	if (b->Exist == false || a->Exist == false)
	{
		return;
	}

	//--- めり込んだ分パックを移動する
	// めり込んだ量を調べる
	float vx = (a->CurPos.x - b->CurPos.x);
	float vy = (a->CurPos.z - b->CurPos.z);
	float length = sqrtf(vx*vx + vy * vy);
	float distance = (a->Radius + b->Radius) - length;
	// aの補正方向を調べる
	if (length > 0)
		length = 1.0f / length;
	distance *= 0.5f;
	distance *= 1.0001f;
	vx *= length;
	vy *= length;
	// aをめり込まない位置まで補正
	a->CurPos.x += vx * distance;
	a->CurPos.z += vy * distance;
	// bをめり込まない位置まで補正する
	b->CurPos.x -= vx * distance;
	b->CurPos.z -= vy * distance;

	//--- 衝突後の方向を調べる
	float t;	// 方向
	float arX;	// aがbに与える移動運動X方向
	float arY;
	float amX;	// aがbに与える回転運動X方向
	float amY;
	float brX;	// bがaに与える移動運動X方向
	float brY;
	float bmX;	// bがaに与える回転運動
	float bmY;

	vx *= -1;
	vy *= -1;


	// 移動運動
	t = -(vx * a->Move.x + vy * a->Move.z) / (vx * vx + vy * vy);
	arX = a->Move.x + vx * t;
	arY = a->Move.z + vy * t;

	// 回転運動
	t = -(-vy * a->Move.x + vx * a->Move.z) / (vy * vy + vx * vx);
	amX = a->Move.x - vy * t;
	amY = a->Move.z + vx * t;

	// 移動運動
	t = -(vx * b->Move.x + vy * b->Move.z) / (vx * vx + vy * vy);
	brX = b->Move.x + vx * t;
	brY = b->Move.z + vy * t;

	// 回転運動
	t = -(-vy * b->Move.x + vx * b->Move.z) / (vy * vy + vx * vx);
	bmX = b->Move.x - vy * t;
	bmY = b->Move.z + vx * t;

	//--- 反発させる
	float e = 1.0f;		// 反発係数
	float avX;			// 反発後のaの速度X方向
	float avY;
	float bvX;			// 反発後のbの速度X方向
	float bvY;

	avX = (a->Mass * amX + b->Mass * bmX + bmX * e * b->Mass - amX * e * b->Mass) / (a->Mass + b->Mass);
	avY = (a->Mass * amY + b->Mass * bmY + bmY * e * b->Mass - amY * e * b->Mass) / (a->Mass + b->Mass);
	bvX = -e * (bmX - amX) + avX;
	bvY = -e * (bmY - amY) + avY;

	// 速度反映
	a->Move.x = avX + arX;
	a->Move.z = avY + arY;
	b->Move.x = bvX + brX;
	b->Move.z = bvY + brY;
}


// チュートリアルステージ
void Puck::UpdateTutorial(int i)
{
	// 壁に当たった時の挙動
	//---------------------------------------------------------------
	// 上の壁
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
		g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToWall));
	}

	// 下の壁
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
		g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToWall));
	}

	// 右の壁
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
		g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToWall));
	}
	// 左の壁
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
		g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToWall));
	}

	//---------------------------------------------------------------
}

// ノーマルステージ
void Puck::UpdateNormalStage(int i)
{
	// 壁に当たった時の挙動
	//---------------------------------------------------------------
	// 上の壁
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// 下の壁
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// 右の壁上
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 右の壁下
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 左の壁上
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 左の壁下
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}

	//---------------------------------------------------------------

	// ゴールに入った時の挙動
	//---------------------------------------------------------------
		// 右のゴール
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// 左のゴール
	if (m_pPuckInfo[i].CurPos.x + PUCK_RADIUS <= STAGE_LEFT + 0.25f)
	{
		m_pScore->AddScore_2P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(-1.0f, 1.25f, 5.0f), XMFLOAT3(-0.03f, 0.0f, -0.05f));
	}
	//---------------------------------------------------------------

}

// 上下貫通ステージ
void Puck::UpdateTopUnderPairStage(int i)
{
	// 壁に当たった時の挙動
	//---------------------------------------------------------------
	// 上の壁
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS*2;
		m_pPuckInfo[i].Move.z *= 0.8f;
	}

	// 下の壁
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS*2;
		m_pPuckInfo[i].Move.z *= 0.8f;
	}

	// 右の壁上
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 右の壁下
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}

	// 左の壁上
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 左の壁下
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	//---------------------------------------------------------------


	// ゴールに入った時の挙動
	//---------------------------------------------------------------
		// 右のゴール
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// 左のゴール
	if (m_pPuckInfo[i].CurPos.x + PUCK_RADIUS <= STAGE_LEFT + 0.25f)
	{
		m_pScore->AddScore_2P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(-1.0f, 1.25f, 5.0f), XMFLOAT3(-0.03f, 0.0f, -0.05f));
	}
	//---------------------------------------------------------------
}

// 
void Puck::UpdateConveyorStage(int i)
{
	// 壁に当たった時の挙動
	//---------------------------------------------------------------
	// 上の壁
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// 下の壁
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// 右の壁上
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 右の壁下
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 左の壁上
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 左の壁下
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	//---------------------------------------------------------------


	// 反射板の移動
	//---------------------------------------------------------------
	m_pConveyorInfo->Pos.z += m_pConveyorInfo->Move;

	if (m_pConveyorInfo->Pos.z >= STAGE_TOP - m_pConveyorInfo->Radius)
	{
		m_pConveyorInfo->Pos.z = STAGE_TOP - m_pConveyorInfo->Radius;
		m_pConveyorInfo->Move *= -1.0f;
	}

	if (m_pConveyorInfo->Pos.z <= STAGE_BOTTOM + m_pConveyorInfo->Radius)
	{
		m_pConveyorInfo->Pos.z = STAGE_BOTTOM + m_pConveyorInfo->Radius;
		m_pConveyorInfo->Move *= -1.0f;
	}
	//---------------------------------------------------------------

	// 反射板との当たり判定
	//---------------------------------------------------------------
	for (int i=0; i < MAX_PUCK; i++)
	{
		if (m_pPuckInfo[i].Exist == false)
		{
			continue;
		}
		if (m_pConveyorInfo->HitConveyorToPuck == true)
		{
			// パックとの当たり判定
			if (CheckCollisionCircleToCircle(m_pPuckInfo[i].CurPos.x, m_pPuckInfo[i].CurPos.z, m_pConveyorInfo->Pos.x, m_pConveyorInfo->Pos.z, m_pPuckInfo[i].Radius, m_pConveyorInfo->Radius))
			{
				// 当たったかの判定を入れる
				m_pConveyorInfo->HitConveyorToPuck = true;

				// 強い反射
				m_pPuckInfo[i].Move.x *= -3.0f;
				m_pPuckInfo[i].Move.z *= -3.0f;

			}

		}
		
	}
	//---------------------------------------------------------------


	// ゴールに入った時の挙動
	//---------------------------------------------------------------
		// 右のゴール
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// 左のゴール
	if (m_pPuckInfo[i].CurPos.x + PUCK_RADIUS <= STAGE_LEFT + 0.25f)
	{
		m_pScore->AddScore_2P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(-1.0f, 1.25f, 5.0f), XMFLOAT3(-0.03f, 0.0f, -0.05f));
	}
	//---------------------------------------------------------------
}

// ノーマルパックのみのステージ
void Puck::UpdateNormalOnlyStage(int i)
{
	// 壁に当たった時の挙動
	//---------------------------------------------------------------
	// 上の壁
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// 下の壁
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// 右の壁上
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 右の壁下
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 左の壁上
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// 左の壁下
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}

	//---------------------------------------------------------------

	// ゴールに入った時の挙動
	//---------------------------------------------------------------
		// 右のゴール
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// 左のゴール
	if (m_pPuckInfo[i].CurPos.x + PUCK_RADIUS <= STAGE_LEFT + 0.25f)
	{
		m_pScore->AddScore_2P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(-1.0f, 1.25f, 5.0f), XMFLOAT3(-0.03f, 0.0f, -0.05f));
	}
	//---------------------------------------------------------------
}


void Puck::UpdateAccelerationStage(int i)
{
	// 壁に当たった時の挙動
//---------------------------------------------------------------
// 上の壁
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.1f;
	}

	// 下の壁
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.1f;
	}

	// 右の壁上
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.1f;
	}
	// 右の壁下
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.1f;
	}
	// 左の壁上
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.1f;
	}
	// 左の壁下
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.1f;
	}

	//---------------------------------------------------------------

	// ゴールに入った時の挙動
	//---------------------------------------------------------------
		// 右のゴール
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// 左のゴール
	if (m_pPuckInfo[i].CurPos.x + PUCK_RADIUS <= STAGE_LEFT + 0.25f)
	{
		m_pScore->AddScore_2P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(-1.0f, 1.25f, 5.0f), XMFLOAT3(-0.03f, 0.0f, -0.05f));
	}
	//---------------------------------------------------------------
}