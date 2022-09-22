#ifndef __POLYLINE_H__
#define __POLYLINE_H__

#include "DrawBuffer.h"
#include <DirectXMath.h>
#include "Shader.h"

// ポリライン用頂点情報
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

	// vtxNum : 作成するポリラインの頂点数
	// 戻り値 : 生成したポリラインのｲﾝﾃﾞｯｸｽ

	// どんだけ頂点使うかを引数に入れる
	int MakePolyLine(int vtxNum);
	void Draw();

	ID3D11ShaderResourceView *m_pPolyLineTex;

	// 頂点情報設定
	void SetVertex(int polyIndex, int vtxIndex, DirectX::XMFLOAT3 pos);
	void SetUV(int polyIndex, int vtxIndex, DirectX::XMFLOAT2 uv);

	// 取得関数
	int GetPolyLineNum();
	int GetVertexNum(int polyIndex);

private:
	DrawBuffer m_buffer;	// 描画用バッファ
	PolyLineVertex* m_pVtx; // 頂点情報
	int* m_pVertexNum;		// ポリラインごとの頂点数
};


#endif // __POLYLINE_H__