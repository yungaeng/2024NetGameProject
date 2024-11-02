#pragma once



class CTimeMgr
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_llCurCount;	// 현재 카운트
	LARGE_INTEGER	m_llPrevCount;	// 이전 카운트
	LARGE_INTEGER	m_llFrequency;	// 초당 카운트 횟수

	double			m_dDT;	// (FPS의 역수=), 1 프레임당 시간	(Delta Time), 프레임 사이의 시간값
	double			m_dAcc;	// 1초 체크를 위한 누적시간
	UINT			m_iCallCount;	// 함수 호출 횟수 
	UINT			m_iFPS;			// 초당 함수 호출 횟수



public:
	void init();	// 초기화
	void update();	// 매 프레임마다 업데이트
	void render();
public:
	double GetDT() { return m_dDT; }	// 8바이트 더블
	float GetfDT() { return (float)m_dDT; }	// 4바이트 플롯

};

