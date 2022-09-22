#include "Collision.h"


using namespace DirectX;


Collision::Collision()
	:m_registerNum(0)
{
}

Collision::~Collision()
{
	Uninit();
}



bool Collision::Init()
{
	

	for (int i = 0; i < MAX_COLLISION_REGISTER; ++i)
	{
		m_pList[i][0] = NULL;
		m_pList[i][1] = NULL;
	}

	return true;
}



void Collision::Uninit()
{

	for (int i = 0; i < m_registerNum; ++i)
	{
		m_pList[i][0] = NULL;
		m_pList[i][1] = NULL;
	}
}



bool Collision::Update()
{
	// m_pList�̒��ɂ��铖���蔻��𒲂ׂ�
	// �����蔻�菈��
	for (int i = 0; i < MAX_COLLISION_REGISTER; i++)
	{
		GameObject *pA = m_pList[i][0];
		GameObject *pB = m_pList[i][1];

		// �o�^����ĂȂ��f�[�^�͎��{���Ȃ�
		if (pA == NULL || pB == NULL)
		{
			continue;
		}

		// �l�p�`���m�̓����蔻��
		// 2D�ł�3D�ł�
		// ���S�܂ł̋��� < �����̑傫���𑫂����킹����
		// �͕ς��Ȃ��A�v�Z���鎲������������
		XMFLOAT3 aPos = pA->GetPos();
  		XMFLOAT3 bPos = pB->GetPos();
		XMFLOAT3 aSize = pA->GetSize();
		XMFLOAT3 bSize = pB->GetSize();

		XMFLOAT3 distance(fabsf(bPos.x - aPos.x), fabsf(bPos.y - aPos.y), fabsf(bPos.z - aPos.z));

		bool hitX = distance.x < (aSize.x + bSize.x)*0.5f;
		bool hitY = distance.y < (aSize.y + bSize.y)*0.5f;
		bool hitZ = distance.z < (aSize.z + bSize.z)*0.5f;

		if (hitX && hitY && hitZ)
		{
			// dynamic - static
			if (pA->GetCollisionType() == COLLISION_DYNAMIC && pB->GetCollisionType() == COLLISION_STATIC)
			{
				Push(pA, pB);
			}

			// �t�̏ꍇ
			if (pA->GetCollisionType() == COLLISION_STATIC && pB->GetCollisionType() == COLLISION_DYNAMIC)
			{
				Push(pB, pA);
			}

			// dynamic - dynamic
			if (pA->GetCollisionType() == COLLISION_DYNAMIC && pB->GetCollisionType() == COLLISION_DYNAMIC)
			{
				Push(pA, pB);
			}

			// dynamic - overrlap

		}
	}



	// ���t���[����������̂��ς��̂Ń��X�g�̒��̔�������Ȃ���
	m_registerNum = 0;
	return false;
}



void Collision::Register(GameObject *pA, GameObject *pB)
{
	if (m_registerNum < MAX_COLLISION_REGISTER)
	{
		m_pList[m_registerNum][0] = pA;
		m_pList[m_registerNum][1] = pB;
		++m_registerNum;
	}
}


//  �����o������
void Collision::Push(GameObject *pDynamic, GameObject *pStatic)
{

	// ���O�Ɋe�I�u�W�F�N�g�̏��𔲂��o��
	XMFLOAT3 dPos = pDynamic->GetPos();
	XMFLOAT3 sPos = pStatic->GetPos();
	XMFLOAT3 dSize = pDynamic->GetSize();
	XMFLOAT3 sSize = pStatic->GetSize();
	XMFLOAT3 dMove = pDynamic->GetMove();

	// �v�Z�ɕK�v�ȏ��𔲂��o��
	// �ړ��ʂ���ړ�������������߂�(�ړ����������1��-1�ŕ\��)

	// �O�����Z�q (A ? B : C)
	// A�̏������������Ă�����B,�s�����Ȃ�C
	XMFLOAT3 dir(dMove.x > 0 ? 1 : -1, dMove.y > 0 ? 1 : -1, dMove.z > 0 ? 1 : -1);

	// ���ۂɓ����蔻��ł߂荞�񂾂̂̓I�u�W�F�N�g�̍��W�ł͂Ȃ��A�p�������߂荞��ł��邱�Ƃ�����̂�
	// �Ώۂ̊p���v�Z�ŋ��߂�

	// �ړ���̊p�̍��W
	XMFLOAT3 dPoint(dPos.x + dir.x*dSize.x*0.5f, dPos.y + dir.y*dSize.y*0.5f, dPos.z + dir.z*dSize.z*0.5f);

	// �ړ��O�̊p�̍��W
	// ���̂܂܂̈ړ��ʂňړ��O�̍��W��\�����Ƃ���ƕ��������_�̌v�Z�덷�ɂ��A�ړ��x�N�g���̎n�_���ʂ̓����ɂ��邱�ƂɂȂ�
	// �ђʂ��ĂȂ��Ɣ��肳��邽�߂ɁA�ړ��O�̈ʒu�������Ȃ�悤�Ɍv�Z�̎������ړ��ʂ�傫������
	XMFLOAT3 prePoint(dPoint.x - dMove.x*1.1f, dPoint.y - dMove.y*1.1f, dPoint.z - dMove.z*1.1f);

	// �Փ˂��ꂽ���̊p���v�Z
	// �Փ˂��ꂽ���̊p�͈ړ������𔽓]���������Ɍ�������
	// �傫���̔����𑫂��Ƌ��߂邱�Ƃ��ł���
	XMFLOAT3 sPoint(sPos.x - dir.x*sSize.x*0.5f, sPos.y - dir.y*sSize.y*0.5f, sPos.z - dir.z*sSize.z*0.5f);

	// �ʏ�̈�_����ړ��x�N�g���̎n�_�ƏI�_�֌������x�N�g�����v�Z����
	// �n�_
	XMFLOAT3 vStart(prePoint.x - sPoint.x, prePoint.y - sPoint.y, prePoint.z - sPoint.z);
	// �I�_
	XMFLOAT3 vEnd(dPoint.x - sPoint.x, dPoint.y - sPoint.y, dPoint.z - sPoint.z);

	// ���ꂼ��̖ʂƌv�Z
	// �e�ʂ̖@���x�N�g��
	XMFLOAT3 Normal[] =
	{
		XMFLOAT3(-dir.x,     0,     0),
		XMFLOAT3(     0,-dir.y,     0),
		XMFLOAT3(     0,     0,-dir.z),
	};

	for (int i = 0; i < _countof(Normal); i++)
	{
		// �ʂ̖@���Ƃ̓��ς��v�Z (dot�����ς̌���)
		float dotS = vStart.x*Normal[i].x + vStart.y*Normal[i].y + vStart.z*Normal[i].z;

		float dotE = vEnd.x*Normal[i].x + vEnd.y*Normal[i].y + vEnd.z*Normal[i].z;

		// ���ꂼ��̓��ς̌��ʂ����ƕ��̒l�ł���Έړ��x�N�g�����ʂ��ђʂ��Ă���
		// ���ƕ��̑g�ݍ��킹���ǂ����͊|���Z�Ŕ���ł���
		// (���~������/���~������/���~������)
		if (dotS*dotE < 0)
		{
			// �ړ��x�N�g���̕����ɂ߂荞�񂾗ʂ�߂��̂ł͂Ȃ��A�ʂ̕����Ɍ������Ė߂�
			// (�߂������͓��ς̒l�Ɠ�����)

			// float�^�̐�Βl�����
			dotE = fabsf(dotE);
			dPos.x += Normal[i].x*dotE;
			dPos.y += Normal[i].y*dotE;
			dPos.z += Normal[i].z*dotE;

			pDynamic->SetPos(dPos);

			// �S�Ă̈ړ��ʂ�0�ɂ��Ă��܂��Ƒ��̃I�u�W�F�N�g�Ɖ����o���̌v�Z������ۂɈړ��ʂ��Ȃ����Ƃ�
			// �Ȃ�̂ŁA�ړ��ʂ�0�̂��Ȃ���΂Ȃ�Ȃ���������0�ɂ���
			dMove.x *= 1 - fabsf(Normal[i].x);
			dMove.y *= 1 - fabsf(Normal[i].y);
			dMove.z *= 1 - fabsf(Normal[i].z);
			pDynamic->SetMove(dMove);
			break;
		}
	}
}