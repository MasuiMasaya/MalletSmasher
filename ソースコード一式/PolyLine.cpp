#include "PolyLine.h"
#include "Texture.h"

//--- �萔��`
#define MAX_POLYLINE_NUM (10)			// �쐬�ł���|�����C�����
#define MAX_POLYLINE_VERTEX_NUM (1000)	// �S�̂Ŏg�p�ł��钸�_��

#define POLYLINEINTERVAL (4)

// �k�ޗp�̒��_��
#define DEGENERATE_VTX (2)   

PolyLine::PolyLine()
	: m_pVtx(NULL)
	, m_pVertexNum(NULL)
{

	LoadTextureFromFile("siro.png", &m_pPolyLineTex);
	// ���_����
	m_pVtx = new PolyLineVertex[MAX_POLYLINE_VERTEX_NUM];
	for (int i = 0; i < MAX_POLYLINE_VERTEX_NUM; ++i)
	{
		m_pVtx[i].pos = DirectX::XMFLOAT3(0, 0, 0);
		m_pVtx[i].color = DirectX::XMFLOAT4(0.3, 1, 0.3, 1);
		if (i % POLYLINEINTERVAL == 0)
		{
			m_pVtx[i].uv = DirectX::XMFLOAT2(0, 0);
		}
		else if (i % POLYLINEINTERVAL == 1)
		{
			m_pVtx[i].uv = DirectX::XMFLOAT2(1, 0);
		}
		else if (i % POLYLINEINTERVAL == 2)
		{
			m_pVtx[i].uv = DirectX::XMFLOAT2(0, 1);
		}
		else if (i % POLYLINEINTERVAL == 3)
		{
			m_pVtx[i].uv = DirectX::XMFLOAT2(1, 1);
		}
	}
	m_buffer.CreateVertexBuffer(m_pVtx, sizeof(PolyLineVertex),
		MAX_POLYLINE_VERTEX_NUM, true);

	// �|�����C�����_��
	m_pVertexNum = new int[MAX_POLYLINE_NUM];
	for (int i = 0; i < MAX_POLYLINE_NUM; ++i)
	{
		m_pVertexNum[i] = 0;
	}
}
PolyLine::~PolyLine()
{
	delete[] m_pVertexNum;
	delete[] m_pVtx;
}

void PolyLine::Draw()
{
	m_buffer.Write(m_pVtx);
	SHADER->SetTexture(m_pPolyLineTex);
	m_buffer.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	SHADER->SetTexture(NULL);
}

int PolyLine::MakePolyLine(int vtxNum)
{

	// �S�̂Ŏg�p���Ă��钸�_�����J�E���g
	int useVtxNum = 0;

	// �擪����g�p���Ă��Ȃ��|�����C��������
	for (int i = 0; i < MAX_POLYLINE_NUM; ++i)
	{
		useVtxNum += m_pVertexNum[i];
		// �g�p�̈�̓X�L�b�v
		if (m_pVertexNum[i] > 0)
		{
			continue;
		}
		// ���g�p�̈�܂łɎg�p���Ă��钸�_���ƁA������g�p���钸�_���̑��a���A�|�����C���ň����钸�_�̑����𒴂��Ă��Ȃ���
		if (useVtxNum + vtxNum >= MAX_POLYLINE_VERTEX_NUM)
		{
			break;
		}

		// ���g�p�̈����������g�p���钸�_����ݒ�
		// �N���X���ŏk�ޏ������s�����߈����̒��_���ɏk�ސ��𑫂����킹�����Ŋm��
		m_pVertexNum[i] = vtxNum+DEGENERATE_VTX;
		return i;
	}

	// �|�����C�����쐬�ł��Ȃ�����
	return -1;
}



// �|�����C���̒��_��ݒ�
void PolyLine::SetVertex(int polyIndex, int vtxIndex, DirectX::XMFLOAT3 pos)
{
	// �w�肳�ꂽ�|�����C���̍ŏ��̒��_���A�S�̂̒��_�̂����A���Ԗڂ̒��_�����߂�
	int startPolyIdx = 0;
	for (int i = 0; i < polyIndex; i++)
	{
		startPolyIdx += m_pVertexNum[i];
	}
	m_pVtx[startPolyIdx + vtxIndex].pos = pos;


	// �ݒ肷�钸�_���|�����C���̃X�^�[�g�ʒu�̒��_�Ȃ�k�ޏ����������Ȃ�
	if (vtxIndex == 0 && polyIndex != 0)
	{
		m_pVtx[startPolyIdx - 1].pos = pos;
	}

	// �|�����C�����\������ƍŌ�̒��_�Ȃ�k�ޏ������s��
	if (vtxIndex == m_pVertexNum[polyIndex] - DEGENERATE_VTX - 1)
	{
		m_pVtx[startPolyIdx + vtxIndex + 1].pos = pos;
	}
}




void PolyLine::SetUV(int polyIndex, int vtxIndex, DirectX::XMFLOAT2 uv)
{
	// �w�肳�ꂽ�|�����C���̍ŏ��̒��_���A�S�̂̒��_�̂����A���Ԗڂ̒��_�����߂�
	int startPolyIdx = 0;
	for (int i = 0; i < polyIndex; i++)
	{
		startPolyIdx += m_pVertexNum[i];
	}
	m_pVtx[startPolyIdx + vtxIndex].uv = uv;


	// �ݒ肷�钸�_���|�����C���̃X�^�[�g�ʒu�̒��_�Ȃ�k�ޏ����������Ȃ�
	if (vtxIndex == 0 && polyIndex != 0)
	{
		m_pVtx[startPolyIdx - 1].uv = uv;
	}

	// �|�����C�����\������ƍŌ�̒��_�Ȃ�k�ޏ������s��
	if (vtxIndex == m_pVertexNum[polyIndex] - DEGENERATE_VTX - 1)
	{
		m_pVtx[startPolyIdx + vtxIndex + 1].uv = uv;
	}
}





int PolyLine::GetPolyLineNum()
{
	int cnt = 0;
	for (int i = 0; i < MAX_POLYLINE_NUM; ++i)
	{
		if (m_pVertexNum[i] > 0)
		{
			cnt++;
		}
		else
		{
			break;
		}
	}
	return cnt;
}



int PolyLine::GetVertexNum(int polyIndex)
{
	if (polyIndex < MAX_POLYLINE_NUM)
	{
		return m_pVertexNum[polyIndex] - 2;
	}
	return 0;
}
