// �C���N��
#include "GameObject.h"
#include "Shader.h"

// �R���X�g���N�^
GameObject::GameObject()
	// �C�j�V�����C�U
	:m_pCube(nullptr)
	,m_pos(0,0,0)
	,m_size(1,1,1)
	,m_collisionType(COLLISION_STATIC)
	,m_move(0,0,0)
{

}

// �f�X�g���N�^
GameObject::~GameObject()
{
	Uninit();
}




// �l�又��
// ����������
bool GameObject::Init()
{
	// �L���[�u�̐���
	m_pCube = new Cube();
	m_pCube->SetSize(m_size.x,m_size.y,m_size.z);
	m_pCube->SetPos(m_pos.x, m_pos.y, m_pos.z);

	return true;
}



// �I������
void GameObject::Uninit()
{
	if (m_pCube != nullptr)		// nullptr... NULL�|�C���^
	{
		delete m_pCube;
		m_pCube = NULL;
	}
}



// �X�V����
void GameObject::Update()
{

}



// �`�揈��
void GameObject::Draw()
{
	//======================================================================
	// 3D���W�ϊ��̊�{�͍s��Ōv�Z���s��
	// �Ⴆ�΃��f�����������������A��]���������A�ړ����������ꍇ
	// ���ꂼ��̕ϊ��s����쐬����K�v������
	// (�����S�Ă���x�ɍs���ϊ��͎����ō�邵���Ȃ�)
	// ���ꂼ��̕ϊ��s����|�����킹���s�񂪍ŏI�I�ȃ��[���h�ϊ��s��ƂȂ�
	// ���̎��A�|���Z�̏��Ԃ��ӎ����Ȃ��ƈӐ}���Ȃ����ʂƂȂ�̂Œ��ӂ��K�v
	// ��{�I�ɂ͊g�k�~��]�~�ړ�
	// �̏��ԂŌv�Z����
	//====================================================================== 
	// ���[���h���W�ւ̕ϊ�
	SHADER->SetWorld(XMMatrixScaling(m_size.x, m_size.y, m_size.z)*XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z));

	// �`��
	m_pCube->Draw();
}




// �Q�b�^�[
XMFLOAT3 GameObject::GetPos()
{
	return m_pos;
}



// �Q�b�^�[
XMFLOAT3 GameObject::GetSize()
{
	return m_size;
}


// �Q�b�^�[
ObjectCollisionType GameObject::GetCollisionType()
{
	return m_collisionType;
}


// �Q�b�^�[
XMFLOAT3 GameObject::GetMove()
{
	return m_move;
}


// �Z�b�^�[
void GameObject::SetPos(XMFLOAT3 pos)
{
	m_pos = pos;
}

void GameObject::SetSize(XMFLOAT3 size)
{
	m_size = size;
}

// �Z�b�^�[
void GameObject::SetMove(XMFLOAT3 move)
{
	m_move = move;
}