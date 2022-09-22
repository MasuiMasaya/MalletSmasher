#include "ConveyorModel.h"

ConveyorModel::ConveyorModel(Puck* p1)
{
	m_pConveyor = p1;
}

ConveyorModel::~ConveyorModel()
{

}

bool ConveyorModel::Init()
{
	m_pConveyor->GetConveyorInfo();

	m_pos = m_pConveyor->GetConveyorInfo()->Pos;
	m_size = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_move.z = m_pConveyor->GetConveyorInfo()->Move;

	

	return true;
}

void ConveyorModel::Uninit()
{

}

void ConveyorModel::Update()
{
	m_pos = m_pConveyor->GetConveyorInfo()->Pos;
	m_move.z = m_pConveyor->GetConveyorInfo()->Move;
}