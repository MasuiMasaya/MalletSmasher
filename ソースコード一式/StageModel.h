#pragma once
#include "Model.h"

class StageModel: public Model
{
public:
	StageModel();
	~StageModel();

	bool Init();
	void Uninit();
	void Update();
	

private:
	Model* m_pStageModel;
};


