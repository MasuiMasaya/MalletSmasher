#ifndef __FIELD_H__
#define __FIELD_H__

#include "GameObject.h"

using namespace DirectX;

class Field:public GameObject
{
public:
	Field(XMFLOAT3 size);
	virtual ~Field();

private:

};

#endif // ! __FIELD_H__
