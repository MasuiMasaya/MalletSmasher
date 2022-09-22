#ifndef __CHARACTER_BASE_H__
#define __CHARACTER_BASE_H__

//#include "GameObject.h"
#include "model.h"

using namespace DirectX;

class CharacterBase :public Model
{
public:
	CharacterBase();
	virtual ~CharacterBase();
	
	// キャラクタの座標データが足元になるように描画で補正を行う
	ID3D11ShaderResourceView *m_pShadowTex;

	virtual void Draw();


private:

protected:
	// XMFLOAT3 m_move;
	DrawBuffer m_shadowBuf;
};






#endif // !__CHARACTERBASE_H__

