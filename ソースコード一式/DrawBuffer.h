#ifndef __DRAW_BUFFER_H__
#define __DRAW_BUFFER_H__

#include "DirectX.h"

class DrawBuffer
{
public:

	DrawBuffer();
	~DrawBuffer();

	// DirectXはBufferに貯めた頂点情報を元に描画を行う
	// 頂点の情報は増やしたり減らしたりすることができる
	// そのため引数で頂点の細かい情報を教える必要がある
	HRESULT CreateVertexBuffer(

		// 描画に必要な頂点のデータ
		void* pVtx,

		// 一つ当たりの頂点のデータサイズ
		UINT size,

		// 描画する頂点の数
		UINT count,

		// 頂点バッファの書き換えが可能か
		bool isWrite = false
	);

	// インデックスバッファの作成
	// 実際のモデルは同じ頂点を共有して三角形を描画することが多い
	// そこで三角形を描画する際に、直接頂点を指定するのではなく
	// インデックス(参照番号)を用いて描画する
	// 頂点バッファのみで描画するよりもデータ量が軽くなる
	HRESULT CreateIndexBuffer(
		const long* pidx,
		UINT count);


	// バッファの書き込み
	void Write(void* pData);


	// バッファの描画
	void Draw(
		// ポリゴンの描画方法
		D3D11_PRIMITIVE_TOPOLOGY primitive);

private:

	// 頂点バッファ
	ID3D11Buffer* m_pVertexBuffer;

	// 頂点のデータサイズ
	UINT m_vtxSize;

	// 頂点の数
	UINT m_vtxCount;

	// インデックスバッファ
	ID3D11Buffer* m_pIndexBuffer;

	// インデックスの数
	UINT m_idxCount;

};



#endif // !__DRAW_BUFFER_H__

