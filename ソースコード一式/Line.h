#pragma once

#include "FigureBase.h"

// ’è”’è‹`
#define MAX_LINES (100)
#define MAX_LINE_VTX (MAX_LINES * 2)

class Line :public Figure
{
public:
	Line();
	~Line();


	void SetLine(XMFLOAT3 start, XMFLOAT3 end);

	void Draw();
private:
	Vertex m_vtx[MAX_LINE_VTX];
	int m_drawNum;

};
