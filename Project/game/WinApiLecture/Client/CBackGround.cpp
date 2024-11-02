#include "pch.h"
#include "CCore.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CBackGround.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CAnimator.h"


CBackGround::CBackGround()
{
	CreateAnimator();
	m_pTex = CResMgr::GetInst()->LoadTexture(L"MapTex", L"texture\\mapbg2.bmp");
}

CBackGround::~CBackGround()
{
}

void CBackGround::start()
{

}

void CBackGround::update()
{
}


void CBackGround::render(HDC _dc)
{
	Vec2 vPos = CCamera::GetInst()->GetLookAt();
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	float width = (float)m_pTex->Width();
	float height = (float)m_pTex->Height();

	/*TransparentBlt(_dc,
		vPos.x - (float)(vResolution.x / 2)
		, vPos.y - (float)(vResolution.y / 2)
		, vResolution.x, vResolution.y
		, m_pTex->GetDC()
		,0,0, width, height
		,NULL);*/

	BitBlt(_dc
		, vPos.x - (float)(vResolution.x / 2)
		, vPos.y - (float)(vResolution.y / 2)
		, width, height,
		m_pTex->GetDC(), 0, 0, SRCCOPY);


	
}

void CBackGround::OnCollisionEnter(CCollider* _pOther)
{

	
}

void CBackGround::OnCollision(CCollider* _pOther)
{
	
}

void CBackGround::OnCollisionExit(CCollider* _pOther)
{
}

