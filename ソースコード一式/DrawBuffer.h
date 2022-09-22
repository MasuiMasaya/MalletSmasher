#ifndef __DRAW_BUFFER_H__
#define __DRAW_BUFFER_H__

#include "DirectX.h"

class DrawBuffer
{
public:

	DrawBuffer();
	~DrawBuffer();

	// DirectX��Buffer�ɒ��߂����_�������ɕ`����s��
	// ���_�̏��͑��₵���茸�炵���肷�邱�Ƃ��ł���
	// ���̂��߈����Œ��_�ׂ̍�������������K�v������
	HRESULT CreateVertexBuffer(

		// �`��ɕK�v�Ȓ��_�̃f�[�^
		void* pVtx,

		// �������̒��_�̃f�[�^�T�C�Y
		UINT size,

		// �`�悷�钸�_�̐�
		UINT count,

		// ���_�o�b�t�@�̏����������\��
		bool isWrite = false
	);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	// ���ۂ̃��f���͓������_�����L���ĎO�p�`��`�悷�邱�Ƃ�����
	// �����ŎO�p�`��`�悷��ۂɁA���ڒ��_���w�肷��̂ł͂Ȃ�
	// �C���f�b�N�X(�Q�Ɣԍ�)��p���ĕ`�悷��
	// ���_�o�b�t�@�݂̂ŕ`�悷������f�[�^�ʂ��y���Ȃ�
	HRESULT CreateIndexBuffer(
		const long* pidx,
		UINT count);


	// �o�b�t�@�̏�������
	void Write(void* pData);


	// �o�b�t�@�̕`��
	void Draw(
		// �|���S���̕`����@
		D3D11_PRIMITIVE_TOPOLOGY primitive);

private:

	// ���_�o�b�t�@
	ID3D11Buffer* m_pVertexBuffer;

	// ���_�̃f�[�^�T�C�Y
	UINT m_vtxSize;

	// ���_�̐�
	UINT m_vtxCount;

	// �C���f�b�N�X�o�b�t�@
	ID3D11Buffer* m_pIndexBuffer;

	// �C���f�b�N�X�̐�
	UINT m_idxCount;

};



#endif // !__DRAW_BUFFER_H__

