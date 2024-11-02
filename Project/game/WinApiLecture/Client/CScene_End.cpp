#include "pch.h"
#include "CScene_End.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CGround.h"
#include "CCore.h"
#include "CPathMgr.h"
#include "CTexture.h"
#include "CCollisionMgr.h"
#include "CResMgr.h"
#include "CKeyMgr.h"
#include "CSceneMgr.h"
#include "CCamera.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CRigidBody.h"
#include "SelectGDI.h"
#include "CTimeMgr.h"
#include "CBackGround.h"
#include "CUI.h"
#include "CPanelUI.h"
#include "CSound.h"

CScene_End::CScene_End()
	:m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f)
{
}

CScene_End::~CScene_End()
{
}

void CScene_End::update()
{

}

void CScene_End::render(HDC _dc)
{
	CScene::render(_dc);

	Vec2 vRes = CCore::GetInst()->GetResolution();
	vRes = CCamera::GetInst()->GetRenderPos(vRes);
	LOGFONT lf;
	lf.lfHeight = 50;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 0;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = HANGEUL_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	lstrcpy(lf.lfFaceName, TEXT("궁서"));

	HFONT hFont = CreateFontIndirect(&lf);

	HFONT OldFont = (HFONT)SelectObject(_dc, hFont);
	wchar_t wsRecord[100] = L"점수";
	wsprintfW(wsRecord, L"점수 : %d ", CCore::GetInst()->m_iCoin * CCore::GetInst()->m_iHP );
	TextOut(_dc, 600,50, wsRecord, lstrlenW(wsRecord));

	SelectObject(_dc, OldFont);
	DeleteObject(hFont);

	//if (!m_bUseForce)
	//	return;
	//SelectGDI gdi1(_dc,BRUSH_TYPE::HOLLOW);
	//SelectGDI gdi2(_dc, PEN_TYPE::GREEN);

	//m_fCurRadius += m_fForceRadius * 2.f * fDT;
	//if (m_fCurRadius > m_fForceRadius)
	//{
	//	m_fCurRadius = 0.f;
	//}
	//Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vForcePos);

	//Ellipse(_dc
	//	, (int)(vRenderPos.x - m_fCurRadius)
	//	, (int)(vRenderPos.y - m_fCurRadius)
	//	, (int)(vRenderPos.x + m_fCurRadius)
	//	, (int)(vRenderPos.y + m_fCurRadius));
	//	//m_vForcePos;

}

void CScene_End::Enter()
{
	CResMgr::GetInst()->LoadSound(L"res", L"sound\\result.wav");
	CSound* pNewSound = CResMgr::GetInst()->FindSound(L"res");
	pNewSound->Play();


	CCamera::GetInst()->SetLookAt(Vec2(0.f, 0.f));
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	Vec2 vPos = CCamera::GetInst()->GetLookAt();
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	/*CCamera::GetInst()->SetLookAt(vPos);*/
	CBackGround* pBg = new CBackGround;
	pBg->SetName(L"Cookie_Run_End");
	pBg->SetScale(vResolution);
	pBg->SetPos(Vec2(0.f, 0.f));
	pBg->SetTex(CResMgr::GetInst()->LoadTexture(L"Main_ENd", L"texture\\Main_Title2.bmp"));
	AddObject(pBg, GROUP_TYPE::BG);

	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"Cookie_Run_Panel");
	pPanelUI->SetScale(Vec2(vResolution.x / 2, vResolution.y/2));
	pPanelUI->SetPos(Vec2(vPos.x - pPanelUI->GetScale().x / 4 - 150,400.f));
	pPanelUI->SetTex(CResMgr::GetInst()->LoadTexture(L"Record", L"texture\\Record.bmp"));
	AddObject(pPanelUI, GROUP_TYPE::UI);
	//m_pUI = pPanelUI;



	//start();
}

void CScene_End::Exit()
{
	DeleteAll();

	// 기존 해당 씬에서 지정해 놨던 충돌 그룹을 해제해 놔야 함
	CCollisionMgr::GetInst()->Reset();
}





