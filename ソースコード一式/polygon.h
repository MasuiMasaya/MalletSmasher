#ifndef __POLYGON_H__
#define __POLYGON_H__


#include "DrawBuffer.h"
#include "Shader.h"
#include "DirectX.h"
#include "Defines.h"
#include "Texture.h"

using namespace DirectX;

class CPolygon
{
public:
	CPolygon();
	~CPolygon();

	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPolygon(XMFLOAT3 polygonsize,XMFLOAT3 polygonpos);


private:
	DrawBuffer m_polygon;

};






#endif // !__POLYGON_H__

