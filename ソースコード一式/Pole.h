#ifndef __POLE_H__
#define __POLE_H__

// インクルード
#include "FigureBase.h"
#include "Field.h"

class Pole:public Figure
{
public:
	Pole();
	~Pole();

	void Draw();

private:
	float m_PosX, m_PosY, m_PosZ;
	float m_width, m_height, m_depth;
	

};



#endif // !__POLE__H__
