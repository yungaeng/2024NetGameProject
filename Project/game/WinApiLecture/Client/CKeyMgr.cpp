#include "pch.h"
#include "CKeyMgr.h"
#include "CCore.h"


int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT, // LEFT
	VK_RIGHT, //RIGHT,
	VK_UP, //UP,
	VK_DOWN, //DOWN,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'F',
	'Z',
	'X',
	'C',
	'V',
	'B',

	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_RETURN,
	VK_SPACE,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,



	//LAST, // enum의 끝
};

CKeyMgr::CKeyMgr()
{

}
CKeyMgr::~CKeyMgr()
{

}


void CKeyMgr::init()
{
	for (int i = 0; i < (int)KEY::LAST; ++i)
	{
		m_vecKey.push_back( tKeyInfo { KEY_STATE::NONE, false });
	}

	
}

void CKeyMgr::update()
{
	// 윈도우 포커싱 알아내기
	// HWND hMainWnd = CCore::GetInst()->GetMainHwnd();	
	// 현재 포커싱 되어있는 윈도우 핸들값(HWND)을 알려줌(없는 경우 NULL)
	HWND hWnd = GetFocus();

	// 윈도우 포커싱 중일 때 키 이벤트 동작
	if (nullptr != hWnd)
	{

		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			// 키가 눌렸는지 체크
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{

				if (m_vecKey[i].bPrevPush)
				{
					// 이전에도 눌린 키인 경우 계속 눌린 상태인 HOLD 상태
					m_vecKey[i].eState = KEY_STATE::HOLD;
				}
				else
				{
					// 이번에 처음 눌린 경우이면 TAP 상태
					m_vecKey[i].eState = KEY_STATE::TAP;
				}
				m_vecKey[i].bPrevPush = true;
			}

			// 키가 눌려있지 않은 경우
			else
			{
				if (m_vecKey[i].bPrevPush)
				{
					// 이전에 눌려있었다면 이제 막 뗀 상태인 AWAY
					m_vecKey[i].eState = KEY_STATE::AWAY;
				}
				else
				{
					// 계속 안 눌려있었다면 안 눌린 상태인 NONE
					m_vecKey[i].eState = KEY_STATE::NONE;
				}
				m_vecKey[i].bPrevPush = false;
			}

		}
		// Mouse 위치 계산
		POINT ptPos = {};
		// 윈도우 자체 마우스 위치 계산 함수 - 윈도우 전체 기준
		GetCursorPos(&ptPos);
		
		// 내가 실행 중인 클라이언트 프로그램 기준으로 바꿔줌
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptPos);

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
	}


	// 윈도우 포커싱 해제상태
	else
	{
		for (int i = 0; i < (int)KEY::LAST; ++i)
		{
			m_vecKey[i].bPrevPush = false;
			// 포커싱이 해제된 순간 TAP 또는 HOLD였다면 AWAY로 상태 변경
			if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::HOLD == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;

			}
			else if (KEY_STATE::AWAY == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;

			}
		}
	}
}


