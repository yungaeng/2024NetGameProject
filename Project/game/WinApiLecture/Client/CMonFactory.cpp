#include "pch.h"
#include "CMonFactory.h"
#include "CRigidBody.h"

#include "CMonster.h"
#include "AI.h"


#include "CIdleState.h"
#include "CTraceState.h"
CMonster* CMonFactory::CreateMonster(MON_TYPE _eType, Vec2 _vPos)
{
	CMonster* pMon = nullptr;
	switch (_eType)
	{
	case MON_TYPE::NORMAL:
	{
		pMon = new CMonster;
		pMon->SetPos(_vPos);
		pMon->m_vCenterPos = _vPos;
		pMon->SetName(L"Monster");
		pMon->SetScale(Vec2(200.f, 200.f));

		tMonInfo info = {};
		info.fAtt = 10.f;
		info.fAttRange = 50.f;
		info.fRecgRange = 300.f;
		info.fHP = 100.f;
		info.fSpeed = (float)MakeRandomInt(75, 125);

		pMon->SetMonInfo(info);

		pMon->CreateRigidBody();
		pMon->GetRigidBody()->SetMass(1.f);


	//	AI* pAI = new AI;
	//	pAI->AddState(new CIdleState);
	////	pAI->AddState(new CTraceState);
	//	pAI->SetCurState(MON_STATE::IDLE);

	//	pMon->SetAI(pAI);
	}
		break;

	case MON_TYPE::RANGE:
	{

	}
		break;
	}
	assert(pMon);


	return pMon;
}
