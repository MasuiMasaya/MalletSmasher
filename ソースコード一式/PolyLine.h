#ifndef __POLYLINE_H__
#define __POLYLINE_H__

#include "DrawBuffer.h"
#include <DirectXMath.h>
#include "Shader.h"

// �|�����C���p���_���
struct PolyLineVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uv;
};

class PolyLine
{
public:
	PolyLine();
	~PolyLine();

	// vtxNum : �쐬����|�����C���̒��_��
	// �߂�l : ���������|�����C���̲��ޯ��

	// �ǂ񂾂����_�g�����������ɓ����
	int MakePolyLine(int vtxNum);
	void Draw();

	ID3D11ShaderResourceView *m_pPolyLineTex;

	// ���_���ݒ�
	void SetVertex(int polyIndex, int vtxIndex, DirectX::XMFLOAT3 pos);
	void SetUV(int polyIndex, int vtxIndex, DirectX::XMFLOAT2 uv);

	// �擾�֐�
	int GetPolyLineNum();
	int GetVertexNum(int polyIndex);

private:
	DrawBuffer m_buffer;	// �`��p�o�b�t�@
	PolyLineVertex* m_pVtx; // ���_���
	int* m_pVertexNum;		// �|�����C�����Ƃ̒��_��
};


#endif // __POLYLINE_H__