#include "pch.h"
#include "CState.h"
#include "CMonster.h"
#include "AI.h"


CState::CState(MON_STATE _eState)
	:m_eState(_eState)
	,m_pAI(nullptr)
{
}

CState::~CState()
{
}



CMonster* CState::GetMonster()
{
	return 	GetAI()->GetOwner();
}
