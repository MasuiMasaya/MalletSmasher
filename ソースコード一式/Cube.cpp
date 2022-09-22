#include "Cube.h"


Cube::Cube()
{
	
	// Vtx�o�b�t�@(���_���)
	const float L = 0.5f;

	Vertex vtx[] = {
		{ {-L, L,-L},{1,1,1,1},{0,0} }, // 0
		{ { L, L,-L},{1,1,1,1},{1,0} },	// 1
		{ {-L,-L,-L},{1,1,1,1},{0,1} },	// 2
		{ { L,-L,-L},{1,1,1,1},{1,1} },	// 3
		{ { L, L, L},{1,1,1,1},{0,0} },	// 4
		{ {-L, L, L},{1,1,1,1},{1,0} },	// 5
		{ { L,-L, L},{1,1,1,1},{0,1} },	// 6
		{ {-L,-L, L},{1,1,1,1},{1,1} },	// 7
	};

	 // �C���f�b�N�X�f�[�^
	long idx[] = {
		0,1,2, 1,3,2, // ��O
		1,4,3, 4,6,3, // �E
		4,5,6, 5,7,6, // ��
		5,0,7, 0,2,7, // ��
		1,0,4, 0,5,4, // ��
		2,3,7, 3,6,7, // ��
	};

	// �o�b�t�@�쐬
	m_buffer.CreateVertexBuffer(vtx, sizeof(Vertex), sizeof(vtx) / sizeof(Vertex));

	// �C���f�b�N�X�o�b�t�@
	m_buffer.CreateIndexBuffer(idx, sizeof(idx) / sizeof(long));
}
Cube::~Cube()
{

}


void Cube::UpdateCube()
{

}

//void Cube::SetPos(float x, float y, float z)
//{
//
//}
//
//
//
//void Cube::SetSize(float w, float h, float d)
//{
//
//}
