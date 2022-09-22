#include "TitelStageModel.h"

TitelStageModel::TitelStageModel()
{
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_size = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

TitelStageModel::~TitelStageModel()
{

}

bool TitelStageModel::Init()
{
	m_pos = XMFLOAT3(0.0f, 0.15f, 0.0f);
	m_size = XMFLOAT3(1.0f, 2.0f, 1.0f);


	return true;
}

void TitelStageModel::Uninit()
{

}

void TitelStageModel::Update()
{

}
