#include "StageModel.h"

StageModel::StageModel()
{
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_size = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

StageModel::~StageModel()
{

}

bool StageModel::Init()
{
	m_pos = XMFLOAT3(0.0f, 0.15f, 0.0f);
	m_size = XMFLOAT3(1.0f, 2.0f, 1.0f);


	return true;
}

void StageModel::Uninit()
{

}

void StageModel::Update()
{

}
