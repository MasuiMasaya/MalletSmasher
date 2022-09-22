#include "FigureBase.h"

// コンストラクタ
Figure::Figure()
	:m_posX(0),m_posY(0),m_posZ(0)
	,m_width(0),m_height(0),m_depth(0)
{
	
}
// デストラクタ
Figure::~Figure()
{

}



void Figure::SetPos(float x, float y, float z)
{

}



void Figure::SetSize(float w, float h, float d)
{

}



void Figure::Draw()
{
	m_buffer.Draw(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}