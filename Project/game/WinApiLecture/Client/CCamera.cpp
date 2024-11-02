#include "pch.h"
#include "CCamera.h"

#include"CObject.h"
#include"CCore.h"
#include"CKeyMgr.h"
#include"CTimeMgr.h"


CCamera::CCamera()
	:m_pTargerObj(nullptr)
	, m_fTime(0.5f)
	, m_fSpeed(0.f)
	, m_fAccTime(0.5f)
{

}

CCamera::~CCamera()
{

}


void CCamera::update()
{
	// 현재 지정한 타겟 obj가 있다면
	if (m_pTargerObj)
	{
		// 그 obj가 죽을 예정이라면 타겟 해제
		if (m_pTargerObj->IsDead())
			m_pTargerObj = nullptr;
		else
		{
			// 가로만 따라다니게 변경
			m_vLookAt.x = m_pTargerObj->GetPos().x;
		}
	}

	if (KEY_HOLD(KEY::UP))
		m_vLookAt.y -= 500.f * fDT;
	if (KEY_HOLD(KEY::DOWN))
		m_vLookAt.y += 500.f * fDT;
	if (KEY_HOLD(KEY::LEFT))
		m_vLookAt.x -= 500.f * fDT;
	if (KEY_HOLD(KEY::RIGHT))
		m_vLookAt.x += 500.f * fDT;
	// 화면 중앙좌표와 카메라 중심 차이값 계산


	CalDiff();
}

void CCamera::CalDiff()
{
	// 이전 LookAt과 현재 Look의 차이값을 보정해서 현재의 LookAt값을 구한다.
	m_fAccTime += fDT;

	if (m_fAccTime >= m_fTime)
	{
		m_vCurLookAt = m_vLookAt;
	}
	else
	{
		Vec2 vLookDir = m_vLookAt - m_vPrevLookAt;		// 차이이자 방향 벡터
		if (!vLookDir.IsZero())
		{
			m_vCurLookAt = m_vPrevLookAt + vLookDir.Normalize() * m_fSpeed * fDT;

		}
	
	}
	

	// 이전 prevLooAt과 현재 Look의 차이값을 보정해서 현재의 CurLookAt을 구한다.
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vCenter = vResolution / 2.f;

	// 기존중심과 카메라 중심의 차이
	m_vDiff = m_vCurLookAt - vCenter;

	m_vPrevLookAt = m_vCurLookAt;
}
