#include "pch.h"
#include "CSceneMgr.h"
#include "CScene_Start.h"
#include "CScene_Tool.h"
#include "CScene_End.h"
#include "CScene_Intro.h"
#include "CEventMgr.h"


CSceneMgr::CSceneMgr()
	:m_arrScene{}
	,m_pCurScene(nullptr)
{

}

CSceneMgr::~CSceneMgr()
{
	// 씬 전부 삭제할 건데 부모 타입 포인터로 가리키고 있었으니까
	// virtual소멸자를 호출하지 않으면 부모 소멸자만 호출되고 끝남
	// ★★★ 따라서 각각 객체의 소멸자가 호출될 수 있도록 가상 소멸자로 만들어야함
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; ++i)
	{
		if(nullptr != m_arrScene[i])
			delete m_arrScene[i];
	}
}

void CSceneMgr::init()
{
	// Scene 생성
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start Scene");

	// Scene 생성
	m_arrScene[(UINT)SCENE_TYPE::INTRO] = new CScene_Intro;
	m_arrScene[(UINT)SCENE_TYPE::INTRO]->SetName(L"Intro Scene");



	m_arrScene[(UINT)SCENE_TYPE::TOOL] = new CScene_Tool;
	m_arrScene[(UINT)SCENE_TYPE::TOOL]->SetName(L"Tool Scene");


	// end scene 추가
	m_arrScene[(UINT)SCENE_TYPE::EnD] = new CScene_End;
	m_arrScene[(UINT)SCENE_TYPE::EnD]->SetName(L"End Scene");


	//m_arrScene[(UINT)SCENE_TYPE::STAGE_01] = new CScene_Stage01;
	//m_arrScene[(UINT)SCENE_TYPE::STAGE_02] = new CScene_Stage02;



	// 현재 씬 지정
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::INTRO];
	m_pCurScene->Enter();
} 

// 현재 씬을 업데이트
void CSceneMgr::update()
{
	m_pCurScene->update();

	m_pCurScene->finalupdate();
}

void CSceneMgr::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}

void CSceneMgr::ChangeScene(SCENE_TYPE _eNext)
{
	// 이제 시점은 이벤트 처리할 때 발동 되는 단계
	// 이 씬을 벗어날 때 발동되어야 하는 함수 Exit() 를 발동
	m_pCurScene->Exit();	// 해당 씬에 있던 오브젝트 그룹들의 오브젝트들 삭제

	m_pCurScene = m_arrScene[(UINT)_eNext];		// 씬 변경

	m_pCurScene->Enter();						// 변경된 씬의 enter실행
}


