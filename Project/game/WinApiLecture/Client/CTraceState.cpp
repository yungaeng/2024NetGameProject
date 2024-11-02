#include "pch.h"
#include "CTraceState.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CTimeMgr.h"


CTraceState::CTraceState()
	:CState(MON_STATE::TRACE)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::update()
{
	// 타겟팅 된 Player 쫓아가기
	CPlayer* pPlayer = (CPlayer*)CSceneMgr::GetInst()->GetCurScene()->GetPlayer();
	Vec2 vPlayerPos = pPlayer->GetPos();

	Vec2 vMonPos = GetMonster()->GetPos();

	// 플레이어의 벡터에서 몬스터의 벡터를 빼면 몬스터에서 플레이어로의 방향벡터가 나온다
	Vec2 vMonDir = vPlayerPos - vMonPos;
	if (vMonDir.IsZero())
		return;
	vMonDir.Normalize();

	vMonPos += vMonDir *  GetMonster()->GetMonInfo().fSpeed * fDT;

	GetMonster()->SetPos(vMonPos);

}

void CTraceState::Enter()
{
}

void CTraceState::Exit()
{
}
