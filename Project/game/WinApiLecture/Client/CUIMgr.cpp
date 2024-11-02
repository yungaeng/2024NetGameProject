#include "pch.h"
#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CUI.h"
#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	:m_pFocusedUI(nullptr)
{

}

CUIMgr::~CUIMgr()
{

}

void CUIMgr::update()
{
	// 1. FocusedUI 확인

	m_pFocusedUI = GetFocusedUI();
	// 반환받아 온 FocusedUI가 nullptr이면 업데이트해줄 UI가 없음 -> 리턴
	if (!m_pFocusedUI)
		return;

	// 2. FocusedUI 내에서, 부모 UI포함 자식 UI들 중 실제 타겟팅 된 UI를 가져온다.
	CUI* pTargetUI =  GetTargetedUI(m_pFocusedUI);


	bool bLbtnTap = KEY_TAP(KEY::LBTN);
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	// 타겟이 되어있는게 있다면
	if (nullptr != pTargetUI)
	{
		pTargetUI->MouseOn();

		if (bLbtnTap)
		{
			pTargetUI->MouseLbtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAway)
		{
			pTargetUI->MouseLbtnUp();
			if (pTargetUI->m_bLbtnDown)
			{
				pTargetUI->MouseLbtnClicked();
			}
			// 왼쪽 버튼을  뗀 경우이니까 눌렀던 체크를 다시 해제한다. 
			pTargetUI->m_bLbtnDown = false;
		}
	}
	//// 현재 씬을 받아와서
	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	//// 그 씬이 보유한 오브젝트들 중  UI그룹에 속한 오브젝트들을 받아온다.
	//const vector<CObject*>& vecUI = pCurScene->GetGroupObject(GROUP_TYPE::UI);


	//

	//	// 다운캐스팅 필요 (오브젝트 타입 포인터기 때문에)
	//m_pFocusedUI;
	//	
	//	// 타겟이 된 ( 최하위 자식 계층까지 확인해서 마우스가 현재 올라와 있는 UI)UI를 받아
	// 원래는 현재 씬에 포함된 오브젝트 그룹 중 UI그룹을 받아와서 전부 다 체크하는 형태였음	
}

void CUIMgr::SetFocusdeUI(CUI* _pUI)
{
	// 이미 포커싱되어 있던 경우
	if (m_pFocusedUI == _pUI || nullptr == _pUI)
	{
		m_pFocusedUI = _pUI;
		return;
	}
	
	m_pFocusedUI = _pUI;
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();

	vector<CObject*>::iterator iter = vecUI.begin();	// 반복용 이터레이터

	for (; iter != vecUI.end(); ++iter)
	{
		if (m_pFocusedUI == *iter)
		{
			// 포커스된 UI 순번 iter를 찾아놓기
			break;
		}
	}


	// 벡터 내에서 맨 뒤로 순번 교체
	vecUI.erase(iter);			// 타겟이터가 가리키는 CUI* 제거 후
	vecUI.push_back(m_pFocusedUI);	// 받아놓은 pFocusedUI를 다시 맨 뒤에 추가
	
}

CUI* CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();
	bool bLbtnTap = KEY_TAP(KEY::LBTN);


	// 기존 포커싱UI를 받아두고 변경되었는지 확인
	CUI* pFocusedUI = m_pFocusedUI;


	// 왼쪽 클릭이 발생하지 않았는데 포커싱이 전환 될 일은 없음 -> 리턴
	if (!bLbtnTap)
	{
		return pFocusedUI;
	}

	// 여기서부터는 적어도 왼쪽버튼 TAP이 발생했다는 전제
	vector<CObject*>::iterator Focusiter = vecUI.end();	// 타겟용 이터레이터
	vector<CObject*>::iterator iter = vecUI.begin();	// 반복용 이터레이터

	for (; iter != vecUI.end(); ++iter)
	{
		if(((CUI*)*iter)->IsMouseOn())
		{
			// 클릭이 발생했는데 마우스가 그 UI 영역 내에 있었으면 target iterator로 설정
			// - 실제로 맨 앞에서 렌더링 돼 클릭을 받은 개체는 vecUI의 맨 뒤에 있었으므로
			// 마지막에 선택을 받을 예정임
			Focusiter = iter;
		}
	}
	// 이번 클릭에서 아무도 선택된 UI가 없다면?
	if (vecUI.end() == Focusiter)
	{
		return nullptr;
	}
	pFocusedUI = (CUI*)*Focusiter;


	// 벡터 내에서 맨 뒤로 순번 교체
	vecUI.erase(Focusiter);			// 타겟이터가 가리키는 CUI* 제거 후
	vecUI.push_back(pFocusedUI);	// 받아놓은 pFocusedUI를 다시 맨 뒤에 추가



	
	return pFocusedUI;
}

CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{

	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	// static을 쓰는 경우 계속 할당/해제 해주기에는 소모가 너무 크니, static으로 선언
	//	 부모 UI 포함, 모든 자식들을 검사한다.
	// - tree와 비슷한 레벨 순회활용
	static list<CUI*> queue;	// 리스트 큐로 사용할 queue 선언
	static vector<CUI*> vecNoneTarget;



	// static으로 선언해 주었으니 clear해주고 다시 시작
	queue.clear();
	vecNoneTarget.clear();	// 벡터의 clear는 동적할당한 공간을 해제하진 않는다.

	CUI* pTargetUI = nullptr;
	queue.push_back(_pParentUI);


	while (!queue.empty())
	{
		// 큐에서 데이터 하나 꺼내기
		CUI* pUI = queue.front();
		queue.pop_front();

		// 큐에서 꺼내온 UI 가 TargetUI인지 확인
		// 타겟 UI 들 중, 더 우선순위가 높은 기준은 더 낮은 계층의 자식 UI

		// 만약 올라와 있으면 일단 TargetUI로 설정
		if (pUI->IsMouseOn())
		{
			if (nullptr != pTargetUI)
			{
				vecNoneTarget.push_back(pTargetUI);
			}
			pTargetUI = pUI;
		}
		else
		{
			vecNoneTarget.push_back(pUI);
		}

		const vector<CUI*>& vecChild = pUI->GetChildUI();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	} 

	// 왼쪽 버튼을 떼면 눌렀던 처리된 targetUI가 아닌 UI들의 누름 처리를 해제한다.
	if (bLbtnAway)
	{
		for (size_t i = 0; i < vecNoneTarget.size(); ++i)
		{

			vecNoneTarget[i]->m_bLbtnDown = false;
		}
	}

	
	return pTargetUI;
}
