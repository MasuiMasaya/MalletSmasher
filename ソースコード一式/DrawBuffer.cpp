#include "DrawBuffer.h"

// コンストラクタ
DrawBuffer::DrawBuffer()
	:m_pVertexBuffer(NULL)
	,m_vtxSize(0)
	,m_vtxCount(0)
	,m_pIndexBuffer(NULL)
	,m_idxCount(0)
{

}
// デストラクタ
DrawBuffer::~DrawBuffer()
{
	// 開放処理
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
}



// バッファの作成
HRESULT DrawBuffer::CreateVertexBuffer(void* pVtx, UINT size, UINT count, bool isWrite)
{
	// 作成するバッファの情報
	// D3D11_BUFFER_DESC 構造体で作られてる すべて0で初期化
	D3D11_BUFFER_DESC bufDesc = {};

	// バッファの大きさ(記憶領域)
	bufDesc.ByteWidth = size * count;

	// バッファの使用方法(普通に使う)
	if (isWrite)
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
	}

	// バッファの利用先
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// バッファの初期値を設定
	D3D11_SUBRESOURCE_DATA subResource = {};
	// 初期化に使うアドレス(データ)の置き場の指定
	subResource.pSysMem = pVtx;

	// 頂点バッファの作成
	HRESULT hr;
	// ゲッター(Dxで必要なものを管理する)
	ID3D11Device* pDevice = GetDevice();
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, &m_pVertexBuffer);

	// そのほかの情報を設定
	if (SUCCEEDED(hr))
	{
		m_vtxSize = size;
		m_vtxCount = count;
	}
	return hr;

}



// バッファの書き込み
void DrawBuffer::Write(void* pData)
{

	HRESULT hr;
	ID3D11DeviceContext* pContext = GetContext();
	D3D11_MAPPED_SUBRESOURCE mapResource;
	hr = pContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapResource);
	
	// Map関数で書き込みokであれば、実行
	// 
	if (SUCCEEDED(hr))
	{
		memcpy_s(mapResource.pData, m_vtxSize*m_vtxCount, pData, m_vtxSize*m_vtxCount);
	}

	pContext->Unmap(m_pVertexBuffer, 0);
}




// 描画関数
void DrawBuffer:: Draw(D3D11_PRIMITIVE_TOPOLOGY primitive)
{
	ID3D11DeviceContext* pContText = GetContext();

	// 頂点一つ当たりの大きさ
	UINT stride = m_vtxSize;

	// 頂点データの読み取り開始位置
	UINT offset = 0;

	// プリミティブの設定
	// TriangleList...頂点を3つずつに区切って描画
	// 
	// 例 [0,1,2] [3,4,5]
	// TriangleStrip...参照場所を一つずつずらしながら描画
	// 例 [0,1,2][1,2,3][2,3,4]
	pContText->IASetPrimitiveTopology(primitive);

	// 描画に使用するバッファの設定
	pContText->IASetVertexBuffers(
		// GPUへの割り当て先
		0, 

		// 頂点バッファの数
		1, 

		// 頂点バッファ
		&m_pVertexBuffer,

		// 頂点のサイズ
		&stride, 

		// 読み取り開始位置
		&offset);

	// 描画
	if (m_idxCount > 0)
	{
		// 描画に使用するインデックスバッファ
		// バッファを設定
		pContText->IASetIndexBuffer(
			m_pIndexBuffer, 
			// インデックス一つ当たりの大きさ
			// 4byte=32bit
			// 2byte=16bit
			DXGI_FORMAT_R32_UINT, 
			0);

		// インデックスバッファを使用して描画
		pContText->DrawIndexed(m_idxCount, 0, 0);



	}
	else
	{
		pContText->Draw(m_vtxCount, 0);
	}
}



// インデックスバッファ
HRESULT DrawBuffer::CreateIndexBuffer(const long* pIdx, UINT count)
{
	// バッファの情報を設定
	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.ByteWidth = sizeof(long)*count;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// バッファの初期データ
	D3D11_SUBRESOURCE_DATA subResource = {};
	subResource.pSysMem = pIdx;

	// インデックスバッファの作成
	ID3D11Device* pDevice = GetDevice();
	HRESULT hr;
	hr = pDevice->CreateBuffer(&bufDesc, &subResource, &m_pIndexBuffer);

	if (SUCCEEDED(hr))
	{
		m_idxCount = count;
	}

	return hr;
}