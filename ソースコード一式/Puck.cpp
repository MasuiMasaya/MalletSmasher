 #include "Puck.h"
#include "SoundEffect.h"
#include "myVector.h"

using namespace DirectX;

GoalInfo g_StageData;
IXAudio2SourceVoice *g_pPuckSESpeaker;

// �֐�
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
	// ���I�m��
	//---------------------------------------------------------------
	m_pPuckInfo = new PuckInfo [MAX_PUCK];

	for (int i = 0; i < MAX_PUCK; i++)
	{
		m_pPuckData[i] = GetPuck(i);
	}

	m_pConveyorInfo = new ConveyorInfo();

	//--------------------------------------------------------------


	//������
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

	// ���֌W�̏�����
	//�`�`�`�`�`�`�`�`�`�`�`�`
	InitSoundEffect();
	//�`�`�`�`�`�`�`�`�`�`�`�`
	//---------------------------------------------------------------


	// �p�b�N�̏�����
	//---------------------------------------------------------------
	m_pos = XMFLOAT3(0.0f, 1.25f, 0.0f);
	m_size = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_move.x = 0;
	m_move.y = 0;
	m_move.z = 0;
	//---------------------------------------------------------------


	// �p�b�N�̏��̏�����
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


	// �X�e�[�W�f�[�^�̏�����
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
	// �v���C���[�̏������炤
	//---------------------------------------------------------------
	m_pPlayer1->GetPlayer();
	m_pPlayer2->GetPlayer2();
	//---------------------------------------------------------------


	// �X�V����
	//---------------------------------------------------------------
	for (int i = 0; i < MAX_PUCK; i++)
	{
		// �g���Ă��Ȃ��̂͏������Ȃ�
		//---------------------------------------------------------------
		if (m_pPuckInfo[i].Exist == false)
		{
			continue;
		}
		//---------------------------------------------------------------


		// ���W�̍X�V
		//---------------------------------------------------------------
		m_pPuckInfo[i].PrePos = m_pPuckInfo[i].CurPos;
		m_pPuckInfo[i].CurPos.x += m_pPuckInfo[i].Move.x;
		m_pPuckInfo[i].CurPos.z += m_pPuckInfo[i].Move.z;
		//---------------------------------------------------------------


		// ��������
		//---------------------------------------------------------------
		if (m_pPuckInfo[i].Move.z > MAX_PUCK_SPEED)
		{
			m_pPuckInfo[i].Move.z = MAX_PUCK_SPEED;
		}
		//---------------------------------------------------------------
		

		// �X�e�[�W�ɍ��킹�ĕǂƂ̓����蔻���؂�ւ���
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

		// ���݂̈ʒu�̍X�V
		//---------------------------------------------------------------
		m_pos = m_pPuckInfo[i].CurPos;
		m_move = m_pPuckInfo[i].Move;
		m_size = m_pPuckInfo[i].Size;
		//---------------------------------------------------------------


		// �}���b�g1P�ƃp�b�N�̓����蔻��
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


		// �}���b�g2P�ƃp�b�N�̓����蔻��
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


		//  �p�b�N���m�̓����蔻��
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

		// �p�b�N�̈ʒu�̍Œ�ۏ�
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
	// �����p�b�N�̓����蔻������Ȃ��悤�ɂ��鏈�u
	//---------------------------------------------------------------
	if (b->Exist == false)
	{
		return;
	}
	//---------------------------------------------------------------

	float aMoveX;    // a�̈ړ���X
	float aMoveY;    // 

	// �ړ����������}���b�g�̈ړ��x�N�g���Ƃ���
	//---------------------------------------------------------------
	aMoveX = (a->CurPos.x - a->PrePos.x)*a->Power;
	aMoveY = (a->CurPos.z - a->PrePos.z)*a->Power;
	//---------------------------------------------------------------


	//--- �߂荞�񂾕��p�b�N���ړ�����
	//---------------------------------------------------------------


	// �߂荞�񂾗ʂ𒲂ׂ�
	//---------------------------------------------------------------
	float vx = (a->CurPos.x - b->CurPos.x);
	float vy = (a->CurPos.z - b->CurPos.z);
	float length = sqrtf(vx*vx + vy * vy);
	float distance = (a->Radius + b->Radius) - length;
	//---------------------------------------------------------------


	// a�̕␳�����𒲂ׂ�
	//---------------------------------------------------------------
	if (length > 0)
		length = 1.0f / length;
	distance *= 0.5f;
	distance *= 1.001f;
	vx *= length;
	vy *= length;
	//---------------------------------------------------------------


	// a���߂荞�܂Ȃ��ʒu�܂ŕ␳
	//---------------------------------------------------------------
	a->CurPos.x += vx * distance;
	a->CurPos.z += vy * distance;
	//---------------------------------------------------------------


	// b���߂荞�܂Ȃ��ʒu�܂ŕ␳����
	//---------------------------------------------------------------
	b->CurPos.x -= vx * distance;
	b->CurPos.z-= vy * distance;
	//---------------------------------------------------------------


	//--- �Փˌ�̕����𒲂ׂ�
	//---------------------------------------------------------------
	float t;    // ����
	float arX;    // a��b�ɗ^����ړ��^��X����
	float arY;
	float amX;    // a��b�ɗ^�����]�^��X����
	float amY;
	float brX;    // b��a�ɗ^����ړ��^��X����
	float brY;
	float bmX;    // b��a�ɗ^�����]�^��
	float bmY;

	vx *= -1;
	vy *= -1;
	//---------------------------------------------------------------


	// �ړ��^��
	//---------------------------------------------------------------
	t = -(vx * aMoveX + vy * aMoveY) / (vx * vx + vy * vy);
	arX = aMoveX + vx * t;
	arY = aMoveY + vy * t;
	//---------------------------------------------------------------


	// ��]�^��
	//---------------------------------------------------------------
	t = -(-vy * aMoveX + vx * aMoveY) / (vy * vy + vx * vx);
	amX = aMoveX - vy * t;
	amY = aMoveY + vx * t;
	//---------------------------------------------------------------


	// �ړ��^��
	//---------------------------------------------------------------
	t = -(vx * b->Move.x + vy * b->Move.z) / (vx * vx + vy * vy);
	brX = b->Move.x + vx * t;
	brY = b->Move.z + vy * t;
	//---------------------------------------------------------------


	// ��]�^��
	//---------------------------------------------------------------
	t = -(-vy * b->Move.x + vx * b->Move.z) / (vy * vy + vx * vx);
	bmX = b->Move.x - vy * t;
	bmY = b->Move.z + vx * t;
	//---------------------------------------------------------------


	//--- ����������
	//---------------------------------------------------------------
	float e = 1.0f;        // �����W��
	float avX;            // �������a�̑��xX����
	float avY;
	float bvX;            // �������b�̑��xX����
	float bvY;

	avX = (a->Mass * amX + b->Mass * bmX + bmX * e * b->Mass - amX * e * b->Mass) / (a->Mass + b->Mass);
	avY = (a->Mass * amY + b->Mass * bmY + bmY * e * b->Mass - amY * e * b->Mass) / (a->Mass + b->Mass);
	bvX = -e * (bmX - amX) + avX;
	bvY = -e * (bmY - amY) + avY;
	//---------------------------------------------------------------


	// ���x���f
	//---------------------------------------------------------------
	//a->movex = avX + arX;
	//a->movey = avY + arY;
	b->Move.x = bvX + brX;
	b->Move.z = bvY + brY;
	//---------------------------------------------------------------

}

void CircleColProcess(Player2::Player2Info *a, Puck::PuckInfo *b)
{
	// �����p�b�N�̓����蔻������Ȃ��悤�ɂ��鏈�u
	if (b->Exist == false)
	{
		return;
	}

	float aMoveX;    // a�̈ړ���X
	float aMoveY;    // 
	// �ړ����������}���b�g�̈ړ��x�N�g���Ƃ���
	aMoveX = (a->CurPos.x - a->PrePos.x)*a->Power;
	aMoveY = (a->CurPos.z - a->PrePos.z)*a->Power;

	//--- �߂荞�񂾕��p�b�N���ړ�����
	// �߂荞�񂾗ʂ𒲂ׂ�
	float vx = (a->CurPos.x - b->CurPos.x);
	float vy = (a->CurPos.z - b->CurPos.z);
	float length = sqrtf(vx*vx + vy * vy);
	float distance = (a->Radius + b->Radius) - length;
	// a�̕␳�����𒲂ׂ�
	if (length > 0)
		length = 1.0f / length;
	distance *= 0.5f;
	distance *= 1.001f;
	vx *= length;
	vy *= length;
	// a���߂荞�܂Ȃ��ʒu�܂ŕ␳
	a->CurPos.x += vx * distance;
	a->CurPos.z += vy * distance;
	// b���߂荞�܂Ȃ��ʒu�܂ŕ␳����
	b->CurPos.x -= vx * distance;
	b->CurPos.z -= vy * distance;

	//--- �Փˌ�̕����𒲂ׂ�
	float t;    // ����
	float arX;    // a��b�ɗ^����ړ��^��X����
	float arY;
	float amX;    // a��b�ɗ^�����]�^��X����
	float amY;
	float brX;    // b��a�ɗ^����ړ��^��X����
	float brY;
	float bmX;    // b��a�ɗ^�����]�^��
	float bmY;

	vx *= -1;
	vy *= -1;


	// �ړ��^��
	t = -(vx * aMoveX + vy * aMoveY) / (vx * vx + vy * vy);
	arX = aMoveX + vx * t;
	arY = aMoveY + vy * t;

	// ��]�^��
	t = -(-vy * aMoveX + vx * aMoveY) / (vy * vy + vx * vx);
	amX = aMoveX - vy * t;
	amY = aMoveY + vx * t;

	// �ړ��^��
	t = -(vx * b->Move.x + vy * b->Move.z) / (vx * vx + vy * vy);
	brX = b->Move.x + vx * t;
	brY = b->Move.z + vy * t;

	// ��]�^��
	t = -(-vy * b->Move.x + vx * b->Move.z) / (vy * vy + vx * vx);
	bmX = b->Move.x - vy * t;
	bmY = b->Move.z + vx * t;

	//--- ����������
	float e = 1.0f;        // �����W��
	float avX;            // �������a�̑��xX����
	float avY;
	float bvX;            // �������b�̑��xX����
	float bvY;

	avX = (a->Mass * amX + b->Mass * bmX + bmX * e * b->Mass - amX * e * b->Mass) / (a->Mass + b->Mass);
	avY = (a->Mass * amY + b->Mass * bmY + bmY * e * b->Mass - amY * e * b->Mass) / (a->Mass + b->Mass);
	bvX = -e * (bmX - amX) + avX;
	bvY = -e * (bmY - amY) + avY;

	// ���x���f
	//a->movex = avX + arX;
	//a->movey = avY + arY;
	b->Move.x = bvX + brX;
	b->Move.z = bvY + brY;

}


// �Փˎ��ɌĂ�
// a���p�b�N b���p�b�N�Ƃ���
void PuckColProcess(Puck::PuckInfo *a, Puck::PuckInfo *b)
{
	// �����p�b�N�̓����蔻������Ȃ��悤�ɂ��鏈�u
	if (b->Exist == false || a->Exist == false)
	{
		return;
	}

	//--- �߂荞�񂾕��p�b�N���ړ�����
	// �߂荞�񂾗ʂ𒲂ׂ�
	float vx = (a->CurPos.x - b->CurPos.x);
	float vy = (a->CurPos.z - b->CurPos.z);
	float length = sqrtf(vx*vx + vy * vy);
	float distance = (a->Radius + b->Radius) - length;
	// a�̕␳�����𒲂ׂ�
	if (length > 0)
		length = 1.0f / length;
	distance *= 0.5f;
	distance *= 1.0001f;
	vx *= length;
	vy *= length;
	// a���߂荞�܂Ȃ��ʒu�܂ŕ␳
	a->CurPos.x += vx * distance;
	a->CurPos.z += vy * distance;
	// b���߂荞�܂Ȃ��ʒu�܂ŕ␳����
	b->CurPos.x -= vx * distance;
	b->CurPos.z -= vy * distance;

	//--- �Փˌ�̕����𒲂ׂ�
	float t;	// ����
	float arX;	// a��b�ɗ^����ړ��^��X����
	float arY;
	float amX;	// a��b�ɗ^�����]�^��X����
	float amY;
	float brX;	// b��a�ɗ^����ړ��^��X����
	float brY;
	float bmX;	// b��a�ɗ^�����]�^��
	float bmY;

	vx *= -1;
	vy *= -1;


	// �ړ��^��
	t = -(vx * a->Move.x + vy * a->Move.z) / (vx * vx + vy * vy);
	arX = a->Move.x + vx * t;
	arY = a->Move.z + vy * t;

	// ��]�^��
	t = -(-vy * a->Move.x + vx * a->Move.z) / (vy * vy + vx * vx);
	amX = a->Move.x - vy * t;
	amY = a->Move.z + vx * t;

	// �ړ��^��
	t = -(vx * b->Move.x + vy * b->Move.z) / (vx * vx + vy * vy);
	brX = b->Move.x + vx * t;
	brY = b->Move.z + vy * t;

	// ��]�^��
	t = -(-vy * b->Move.x + vx * b->Move.z) / (vy * vy + vx * vx);
	bmX = b->Move.x - vy * t;
	bmY = b->Move.z + vx * t;

	//--- ����������
	float e = 1.0f;		// �����W��
	float avX;			// �������a�̑��xX����
	float avY;
	float bvX;			// �������b�̑��xX����
	float bvY;

	avX = (a->Mass * amX + b->Mass * bmX + bmX * e * b->Mass - amX * e * b->Mass) / (a->Mass + b->Mass);
	avY = (a->Mass * amY + b->Mass * bmY + bmY * e * b->Mass - amY * e * b->Mass) / (a->Mass + b->Mass);
	bvX = -e * (bmX - amX) + avX;
	bvY = -e * (bmY - amY) + avY;

	// ���x���f
	a->Move.x = avX + arX;
	a->Move.z = avY + arY;
	b->Move.x = bvX + brX;
	b->Move.z = bvY + brY;
}


// �`���[�g���A���X�e�[�W
void Puck::UpdateTutorial(int i)
{
	// �ǂɓ����������̋���
	//---------------------------------------------------------------
	// ��̕�
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
		g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToWall));
	}

	// ���̕�
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
		g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToWall));
	}

	// �E�̕�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
		g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToWall));
	}
	// ���̕�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
		g_pPuckSESpeaker = PlaySound(GetSoundEffect(SE_PuckToWall));
	}

	//---------------------------------------------------------------
}

// �m�[�}���X�e�[�W
void Puck::UpdateNormalStage(int i)
{
	// �ǂɓ����������̋���
	//---------------------------------------------------------------
	// ��̕�
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// ���̕�
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// �E�̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// �E�̕ǉ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// ���̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// ���̕ǉ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}

	//---------------------------------------------------------------

	// �S�[���ɓ��������̋���
	//---------------------------------------------------------------
		// �E�̃S�[��
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// ���̃S�[��
	if (m_pPuckInfo[i].CurPos.x + PUCK_RADIUS <= STAGE_LEFT + 0.25f)
	{
		m_pScore->AddScore_2P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(-1.0f, 1.25f, 5.0f), XMFLOAT3(-0.03f, 0.0f, -0.05f));
	}
	//---------------------------------------------------------------

}

// �㉺�ђʃX�e�[�W
void Puck::UpdateTopUnderPairStage(int i)
{
	// �ǂɓ����������̋���
	//---------------------------------------------------------------
	// ��̕�
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS*2;
		m_pPuckInfo[i].Move.z *= 0.8f;
	}

	// ���̕�
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS*2;
		m_pPuckInfo[i].Move.z *= 0.8f;
	}

	// �E�̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// �E�̕ǉ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}

	// ���̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// ���̕ǉ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	//---------------------------------------------------------------


	// �S�[���ɓ��������̋���
	//---------------------------------------------------------------
		// �E�̃S�[��
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// ���̃S�[��
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
	// �ǂɓ����������̋���
	//---------------------------------------------------------------
	// ��̕�
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// ���̕�
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// �E�̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// �E�̕ǉ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// ���̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// ���̕ǉ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	//---------------------------------------------------------------


	// ���˔̈ړ�
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

	// ���˔Ƃ̓����蔻��
	//---------------------------------------------------------------
	for (int i=0; i < MAX_PUCK; i++)
	{
		if (m_pPuckInfo[i].Exist == false)
		{
			continue;
		}
		if (m_pConveyorInfo->HitConveyorToPuck == true)
		{
			// �p�b�N�Ƃ̓����蔻��
			if (CheckCollisionCircleToCircle(m_pPuckInfo[i].CurPos.x, m_pPuckInfo[i].CurPos.z, m_pConveyorInfo->Pos.x, m_pConveyorInfo->Pos.z, m_pPuckInfo[i].Radius, m_pConveyorInfo->Radius))
			{
				// �����������̔��������
				m_pConveyorInfo->HitConveyorToPuck = true;

				// ��������
				m_pPuckInfo[i].Move.x *= -3.0f;
				m_pPuckInfo[i].Move.z *= -3.0f;

			}

		}
		
	}
	//---------------------------------------------------------------


	// �S�[���ɓ��������̋���
	//---------------------------------------------------------------
		// �E�̃S�[��
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// ���̃S�[��
	if (m_pPuckInfo[i].CurPos.x + PUCK_RADIUS <= STAGE_LEFT + 0.25f)
	{
		m_pScore->AddScore_2P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(-1.0f, 1.25f, 5.0f), XMFLOAT3(-0.03f, 0.0f, -0.05f));
	}
	//---------------------------------------------------------------
}

// �m�[�}���p�b�N�݂̂̃X�e�[�W
void Puck::UpdateNormalOnlyStage(int i)
{
	// �ǂɓ����������̋���
	//---------------------------------------------------------------
	// ��̕�
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// ���̕�
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.0f;
	}

	// �E�̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// �E�̕ǉ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// ���̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}
	// ���̕ǉ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.0f;
	}

	//---------------------------------------------------------------

	// �S�[���ɓ��������̋���
	//---------------------------------------------------------------
		// �E�̃S�[��
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// ���̃S�[��
	if (m_pPuckInfo[i].CurPos.x + PUCK_RADIUS <= STAGE_LEFT + 0.25f)
	{
		m_pScore->AddScore_2P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(-1.0f, 1.25f, 5.0f), XMFLOAT3(-0.03f, 0.0f, -0.05f));
	}
	//---------------------------------------------------------------
}


void Puck::UpdateAccelerationStage(int i)
{
	// �ǂɓ����������̋���
//---------------------------------------------------------------
// ��̕�
	if (m_pPuckInfo[i].CurPos.z >= STAGE_TOP - PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_TOP - PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.1f;
	}

	// ���̕�
	if (m_pPuckInfo[i].CurPos.z <= STAGE_BOTTOM + PUCK_RADIUS)
	{
		m_pPuckInfo[i].CurPos.z = STAGE_BOTTOM + PUCK_RADIUS;
		m_pPuckInfo[i].Move.z *= -1.1f;
	}

	// �E�̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.1f;
	}
	// �E�̕ǉ�
	if (m_pPuckInfo[i].CurPos.x >= STAGE_RIGHT - PUCK_RADIUS && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_RIGHT - PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.1f;
	}
	// ���̕Ǐ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z + PUCK_RADIUS >= g_StageData.Top)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.1f;
	}
	// ���̕ǉ�
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS <= STAGE_LEFT && m_pPuckInfo[i].CurPos.z - PUCK_RADIUS <= g_StageData.Bottom)
	{
		m_pPuckInfo[i].CurPos.x = STAGE_LEFT + PUCK_RADIUS;
		m_pPuckInfo[i].Move.x *= -1.1f;
	}

	//---------------------------------------------------------------

	// �S�[���ɓ��������̋���
	//---------------------------------------------------------------
		// �E�̃S�[��
	if (m_pPuckInfo[i].CurPos.x - PUCK_RADIUS >= STAGE_RIGHT - 0.25f)
	{

		m_pScore->AddScore_1P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(1.0f, 1.25f, 5.0f), XMFLOAT3(0.03f, 0.0f, -0.05f));

	}

	// ���̃S�[��
	if (m_pPuckInfo[i].CurPos.x + PUCK_RADIUS <= STAGE_LEFT + 0.25f)
	{
		m_pScore->AddScore_2P(m_pPuckInfo[i].Score);
		CreatePuck(NormalPuck, XMFLOAT3(-1.0f, 1.25f, 5.0f), XMFLOAT3(-0.03f, 0.0f, -0.05f));
	}
	//---------------------------------------------------------------
}