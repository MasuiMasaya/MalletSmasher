#pragma once


#pragma once
#include "Model.h"

class TitelStageModel : public Model
{
public:
	TitelStageModel();
	~TitelStageModel();

	bool Init();
	void Uninit();
	void Update();


private:
	Model* m_pStageModel;
};

