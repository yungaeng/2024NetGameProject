#include "pch.h"
#include "CEventMgr.h"
#include "CObject.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CUIMgr.h"
#include "CState.h"
#include "AI.h"

CEventMgr::CEventMgr()
{

}
CEventMgr::~CEventMgr()
{

}

void CEventMgr::update()
{
	// 이벤트 매니저 업데이트 시작할 때 데드벡터가 채워져 있으면 이전 프레임이 등록한 이벤트일 것이다.
	// ==============================================
	// 이전 프레임에서 등록해 둔 Dead Object 들을 삭제한다
	// ==============================================
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		delete m_vecDead[i];
	}
	m_vecDead.clear();
	

	// ===================
	//	EVENT 처리
	// ===================
	for (size_t i = 0; i < m_vecEvent.size(); ++i) 
	{
		Excute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}



// 만약 Scene 체인지 이벤트가 등록되면, 그 이벤트를 처리한 후 나머지 이벤트는 없애는 방식으로 생각하기
void CEventMgr::Excute(const tEvent& _eve)
{
	switch (_eve.eEven)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam : Object Address
		// wParam :	GROUP_TYPE
		CObject* pNewObj = (CObject*)_eve.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_eve.wParam;
		CSceneMgr::GetInst()->GetCurScene()->AddObject(pNewObj, eType);
	}
	
		break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : Object Address
		// 1. Object를 Dead 상태로 변경
		// 2. 삭제 예정 오브젝트를 모아둔다 m_vecDead에
		CObject* pDeadObj = (CObject*)_eve.lParam;
		pDeadObj->SetDead();
		m_vecDead.push_back(pDeadObj);
	}
		break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam : Next SCENE_TYPE
		// Scene 변경
		SCENE_TYPE eType = (SCENE_TYPE)_eve.lParam;
		CSceneMgr::GetInst()->ChangeScene(eType);

		// 포커스 UI해제(이전 Scene의 UI를 가리키고 있기 때문)
		CUIMgr::GetInst()->SetFocusdeUI(nullptr);
	}
	break;
	case EVENT_TYPE::CHANGE_AI_STATE:
	{	
		// lParam : AI*
		// wParam : Next STATE_TYPE
		AI* pAI = (AI*)_eve.lParam;
		MON_STATE eNextState = (MON_STATE)_eve.wParam;

		pAI->ChangeState(eNextState);

	}
		break;
	}
}
