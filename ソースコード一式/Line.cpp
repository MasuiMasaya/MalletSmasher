#include "Line.h"


//	�R���X�g���N�^
Line::Line()
	:m_drawNum(0)
{
	// �`�悷�钼���̒��_���
	//Vertex vtx[] =
	//{
	//	{{0,0,0},{1,1,1,1},{0,0}}, // ���_
	//	{{5,5,5},{1,1,1,1},{0,0}}, // �I�_
	//};

	for (int i = 0; i < MAX_LINE_VTX; i++)
	{
		m_vtx[i].color.w = 1.0f;
		m_vtx[i].color.x = 1.0f;
		m_vtx[i].color.y = 1.0f;
		m_vtx[i].color.z = 1.0f;
	}
	// �o�b�t�@�쐬
	m_buffer.CreateVertexBuffer(m_vtx, sizeof(Vertex), MAX_LINE_VTX, true);

}

// �f�X�g���N�^
Line::~Line()
{
}



void Line::Draw()
{
#if 0 // �������݃e�X�g
	static float f = 0.0f;
	// �`�悷�钼���̒��_���
	Vertex vtx[] =
	{
		{{0,0,0},{1,1,1,1},{0,0}},         // �n�_
		{{sinf(f),5,0},{1,1,1,1},{0,0}}, // �I�_
	};
	f += 0.01f;
	m_buffer.Write(vtx);
#endif

	// �g�p���Ă��Ȃ����_�f�[�^�͏�����
	for (int i = m_drawNum; i < MAX_LINE_VTX; i++)
	{
		m_vtx[i].pos.x = m_vtx[i].pos.y = m_vtx[i].pos.z = 0.0f;
	}

	// �`��܂łɗ��߂������̏��𒸓_�o�b�t�@�֏�������
	m_buffer.Write(m_vtx);

	// ���_��`�悷�鎞�̃v���~�e�B�u���_�o�b�t�@���̃f�[�^�����ǂݎ���Ē�����`��
	m_buffer.Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	m_drawNum = 0;
}

void Line::SetLine(XMFLOAT3 start, XMFLOAT3 end)
{
	// ���_�z��̐擪���璼���̃f�[�^���i�[
	if (m_drawNum < MAX_LINE_VTX)
	{
		m_vtx[m_drawNum].pos = start;
		++m_drawNum;
		m_vtx[m_drawNum].pos = end;
		++m_drawNum;
	}
}