#include "global.h"
#include "CCore.h"
#include "CObject.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CResMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "SelectGDI.h"
#include "CSound.h"

#include"resource.h"

// 동적할당 방식으로 하면 따로 초기화 필요
//CCore* CCore::g_pInst = nullptr;



CCore::CCore()
	:m_hWnd(0)
	,m_ptResolution{}
	,m_hDC(0)
	,m_hBit(0)
	,m_memDC(0)
	,m_arrBrush{}
	,m_arrPen{}
	,m_hMenu(0)
	,m_iCoin(0)
	,m_iHP(5)
{
	
}

CCore::~CCore()
{
	ReleaseDC(this->m_hWnd, this->m_hDC);

	// CreateCompatiableDC로 얻어온 DC는 DeleteDC해줘야됨, (윈도우 핸들로 얻은 것이 아닐 때)
	DeleteDC(m_memDC);
	DeleteObject(m_hBit);

	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
	DestroyMenu(m_hMenu);
}


// CCore 초기화 함수
int CCore::init(HWND _hWnd, POINT _ptResolution)
{
	// 윈도우 핸들 입력
	m_hWnd = _hWnd;
	// 윈도우 해상도 값 입력
	m_ptResolution = _ptResolution;

	// ★★★★★★★ 해상도 설정 
	// 내가 원하는 클라이언트 영역의 실제 RECT값을 rt에 세팅해 줌
	// 내가 원하는 클라이언트의 영역 설정
	ChangeWindowSize(Vec2((float)_ptResolution.x, (float)_ptResolution.y), false);

	// 메뉴 바 생성
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_CLIENT));

	// DC 얻어오기
	m_hDC = GetDC(m_hWnd);


	// 이중 버퍼링 용도의 비트맵과 DC를 만든다
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);
	// m_memDC의 목적지로 m_hBit 선택, m_memDC의 기존 목적지 비트맵인 1픽셀 비트맵 반환받은 후 삭제
	HBITMAP hOldBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(hOldBit);
	
	// 자주 사용할 펜 및 브러쉬 생성
	CreateBrushPen();


	// ★★★Manager 초기화
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();
	CPathMgr::GetInst()->init();


	CSoundMgr::GetInst()->init();
	//// 각 신 업데이트에 넣으면 될듯
	//// Sound 로드 테스트★★★★★★
	//CResMgr::GetInst()->LoadSound(L"BGM_01", L"sound\\BGM.wav");
	//CSound* pNewSound = CResMgr::GetInst()->FindSound(L"BGM_01");

	////pNewSound->Play();

	//pNewSound->SetPosition(0.f);			// 백분율, 재생 위치 설정 ex 50% 구간
	//pNewSound->PlayToBGM(true);				// BGM으로 Play 
	////pNewSound->SetVolume(60.f);			// 0~100 까지 볼륨 설정


	// 현재 Sound당 버퍼는 한 개이기 때문에 한 사운드가 재생중일 때 그 사운드를 다시 재생시키면
	// 버퍼가 내용이 바뀌어 기존 사운드는 재생되지 않는다.

	return S_OK;
}



// 메시지가 발생하지 않을 때 작동하는 함수 - 항상 발동
void CCore::progress()
{
	// =============
	// ★Manager update
	// =============
	CTimeMgr::GetInst()->update();
	CKeyMgr::GetInst()->update();
	CCamera::GetInst()->update();

	// =============
	// ★Scene Update
	// =============
	// 모든 신의 업데이트
	CSceneMgr::GetInst()->update();

	// ★ 충돌체크
	// 신 업데이트 이후 충돌매니저 업데이트 순서로 진행
	CCollisionMgr::GetInst()->update();

	// ★ UI 이벤트 체크
	CUIMgr::GetInst()->update();


	// =============
	// ★Rendering
	// =============
	// 화면 clear
	Clear();

	// 코어의 DC를 전달
	CSceneMgr::GetInst()->render(m_memDC);
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
		m_memDC, 0, 0, SRCCOPY);

	CTimeMgr::GetInst()->render();

	// =============
	// 이벤트 지연처리
	// =============
	CEventMgr::GetInst()->update();

}

void CCore::Clear()
{
	SelectGDI gdi(m_memDC, BRUSH_TYPE::BLACK);
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
}

void CCore::CreateBrushPen()
{
	// hollow brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);
	//r g b pen
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));


}

void CCore::DockMenu()
{
	SetMenu(m_hWnd,m_hMenu);
	ChangeWindowSize(GetResolution(), true);
}

void CCore::DivideMenu()
{
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(GetResolution(), false);


}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt = { 0,0,(long)_vResolution.x,(long)_vResolution.y };

	// 해상도에 맞게 윈도우 크기 조정하는 작업
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	// 가로길이
	// rt.right - rt.left;
	// 세로길이
	// rt.bottom - rt.top; 
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
	// ★ 해상도 설정 
}


