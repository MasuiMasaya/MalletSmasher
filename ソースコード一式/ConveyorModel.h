#pragma once
#include "Model.h"
#include "Puck.h"

class ConveyorModel : public Model
{
public:
	ConveyorModel(Puck*);
	~ConveyorModel();

	bool Init();
	void Uninit();
	void Update();


private:
	Model* m_pConveyorModel;
	Puck* m_pConveyor;
};