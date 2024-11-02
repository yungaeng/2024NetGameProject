#include "pch.h"
#include "CIdleState.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"


CIdleState::CIdleState()
	:CState(MON_STATE::IDLE)
{
}

CIdleState::~CIdleState()
{
}


void CIdleState::update()
{
	// 가만히 있는다,
	// Player 위치 체크
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();

	Vec2 vPlayerPos = pPlayer->GetPos();

	// 몬스터의 범위 안에 들어오면 추적 상태로 전환
	CMonster* pMonster = GetMonster();

	Vec2 vMonPos = pMonster->GetPos();

	Vec2 vDiff = vPlayerPos - vMonPos;
	float fLen = vDiff.Length();

	//// 인지범위 내로 들어왔다면, 상태를 전환한다.
	//if (fLen < pMonster->GetMonInfo().fRecgRange)
	//{
	//	// 상태 전환 시 일관성 있는 상태에 따른 업데이트, 렌더가 필요하므로 이벤트에 등록하여 준다.
	//	ChangeAIState(GetAI(), MON_STATE::TRACE);
	//}
}

void CIdleState::Enter()
{
}

void CIdleState::Exit()
{
}