#pragma once
#include "CharacterBase.h"
#include "PolyLine.h"



class Meteor:public CharacterBase
{
public:
	Meteor();
	~Meteor();

	void Update();

	XMFLOAT3 SetMeteorPos() { return m_NewPos; };

	

	

private:
	Cube* m_pCube;
	PolyLine* m_pPolyLine;
	XMFLOAT3 m_NewPos;

};

