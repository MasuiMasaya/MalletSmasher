// �C���N��
#include "CharacterBase.h"
#include "Texture.h"



// �R���X�g���N�^
CharacterBase::CharacterBase()
{
	// �L�����N�^�[�͓����I�u�W�F�N�g���낤
	m_collisionType = COLLISION_DYNAMIC;

	// �e�Ŏg�p����e�N�X�`���̓ǂݍ���
	LoadTextureFromFile("shadow.png", &m_pShadowTex);

	// �e�`��p�|���S��
	float s = 0.5;
	XMFLOAT4 w(1, 1, 1, 1);
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
		XMFLOAT2 uv;
	};
	Vertex vtx[] =
	{
	{ XMFLOAT3(-s,0,s),w,XMFLOAT2(0,0) },
	{ XMFLOAT3(s,0,s),w,XMFLOAT2(1,0) },
	{ XMFLOAT3(-s,0,-s),w,XMFLOAT2(0,1) },
	{ XMFLOAT3(s,0,-s),w,XMFLOAT2(1,1) },
	};
	m_shadowBuf.CreateVertexBuffer(vtx, sizeof(Vertex), _countof(vtx));

}

// �f�X�g���N�^
CharacterBase::~CharacterBase()
{

}

void CharacterBase::Draw()
{

	SHADER->SetWorld(XMMatrixScaling(m_size.x, m_size.y, m_size.z)*XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));
	m_pCube->Draw();

	// �e�̕`��
	// �e�̓v���C���[�ɂ��Ĉړ����邪y���W�܂ňړ�����ƁA�󒆂ɉe���ł��Ă��܂��̂ŁAY���W�͂O�ɂ���
	// Y����0�̂܂܂��ƒn�ʂƏd�Ȃ��Ă��܂��ǂ��炪��ɕ`�悳��邩�s����ɂȂ��Ă��܂��̂�
	// (Z�t�@�C�e�B���O)��������Y���W���グ�ďd�Ȃ���������
	float scale = (5.0f - m_pos.y) / 5.0f;
	scale = min(1, max(0, scale));

	SHADER->SetWorld(XMMatrixScaling(scale, scale, scale)*XMMatrixTranslation(m_pos.x, 0.001f, m_pos.z));
	//SHADER->SetWorld(XMMatrixTranslation(m_pos.x, 0.001f, m_pos.z));
	SHADER->SetTexture(m_pShadowTex);
	m_shadowBuf.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	SHADER->SetTexture(NULL);

}

