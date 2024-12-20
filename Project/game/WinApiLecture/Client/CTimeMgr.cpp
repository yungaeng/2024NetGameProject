#include "pch.h"
#include "CTimeMgr.h"
#include "CCore.h"


CTimeMgr::CTimeMgr()
	:m_llCurCount{}
	, m_llFrequency{}
	, m_llPrevCount{}
	, m_dDT(0.)
	, m_iCallCount(0)
	, m_iFPS(0)
	,m_dAcc(0.)
{

}


CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	// 현재 카운트
	QueryPerformanceCounter(&m_llPrevCount);

	// 초당 카운트 횟수 (초당 카운트 차이 값)
	QueryPerformanceFrequency(&m_llFrequency);
}

void CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	// 이전 프레임의 카운팅과 현재 프레임 카운팅 값의 차이를 구한다. 이를 초당 카운트 횟수로 나눈다 -> 프레임카운트당 시간이 나온다
	m_dDT = (double)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (double)(m_llFrequency.QuadPart);

	// 이전 프레임카운트 값 현재 프레임카운트값으로 갱신 (다음 계산을 위해)
	m_llPrevCount = m_llCurCount;

#ifdef _DEBUG
	if (m_dDT > (1. / 60.))
		m_dDT = (1. / 60.);
#endif
}

void CTimeMgr::render()
{

	//// 호출 횟수 증가
	//++m_iCallCount;

	//m_dAcc += m_dDT;	// DT 누적

	//if (m_dAcc >= 1.) // 1초에 한번(m_dAcc에 누적 된 값이 1초가 넘어가면)
	//{
	//	m_dAcc = 0.;
	//	m_iFPS = m_iCallCount;
	//	m_iCallCount = 0;

	//	wchar_t szBuffer[255] = {};
	//	swprintf_s(szBuffer, L"FPS : %d, DT : %f", m_iFPS, m_dDT);
	//	SetWindowText(CCore::GetInst()->GetMainHwnd(), szBuffer);
	//}
}
