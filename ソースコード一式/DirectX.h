#ifndef __DIRECT_X_H__
#define __DIRECT_X_H__

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

#define SAFE_RELEASE(p) if(p){p->Release(); p = NULL;}

enum BlendMode
{
	BLEND_ALPHA,
	BLEND_ADD,
	BLEND_MAX
};

HRESULT InitDX(HWND hWnd, UINT width, UINT height, bool fullscreen);
void UninitDX();
void BeginDrawDX();
void EndDrawDX();

ID3D11Device *GetDevice();
ID3D11DeviceContext *GetContext();

void SetBlendMode(BlendMode blend);

// 特定の面を映らないようにする処理を
// カリング(Culling)という
void EnableCulling(bool enable);

// Zバッファの有効/無効を切り替える
void EnableZBuffer(bool enable);

#endif // __DIRECT_X_H__