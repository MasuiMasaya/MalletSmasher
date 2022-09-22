#include "Meteor.h"

#define MateorNum (10)


Meteor *pmeteor;

Meteor::Meteor()
{
	m_NewPos = XMFLOAT3(rand() % 6 + 1, 10, rand() % 6 + 1);
	m_pos = m_NewPos;
	
}

Meteor::~Meteor()
{
}


void Meteor::Update()
{
	// à⁄ìÆ
	//m_move = XMFLOAT3(0, m_move.y, 0);
	//m_NewPos = XMFLOAT3(rand() % 10 + 1, 10, rand() % 6 + 1);
	
	//// èdóÕ
	m_pos.y -= 0.1f;

	if (m_pos.y < 0)
	{
		
		m_pos =XMFLOAT3(rand() % 10 + 1, 10, rand() % 6 + 1);
	}
	

}



