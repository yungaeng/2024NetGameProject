#include "pch.h"
#include "CCore.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CTexture.h"
#include "CGround.h"
#include "CPlayer.h"
#include "CCollider.h"
#include "CResMgr.h"
#include "CGravity.h"


CGround::CGround()
{
	CreateCollider();
	m_pTex = CResMgr::GetInst()->LoadTexture(L"GroundTex", L"texture\\Ground3.bmp");
}

CGround::~CGround()
{
}

void CGround::start()
{
	GetCollider()->SetScale(Vec2(GetScale()));
}

void CGround::update()
{

}

void CGround::render(HDC _dc)
{
	//CObject::render(_dc);
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	vPos = CCamera::GetInst()->GetRenderPos(vPos);

	float width = (float)m_pTex->Width();
	float height = (float)m_pTex->Height();

	TransparentBlt(_dc,
		vPos.x - (float)(vScale.x / 2)
		, vPos.y - (float)(vScale.y / 2)
		, vScale.x, vScale.y
		, m_pTex->GetDC()
		, 0, 0, width, height
		, RGB(255,0,255));
}

void CGround::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		if (vObjPos.y + vObjScale.y / 2 - 20 <= vPos.y - vScale.y / 2
			&& ((CPlayer*)pOtherObj)->GetDir() == false)
		{
			pOtherObj->GetGravity()->SetGround(true);
			// 두 세로 반경의 절반 간의 합 - 두 중심점과의 차이 = 파고든 거리
			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			// 파고든 거리만큼 올려줌
			vObjPos = pOtherObj->GetPos();
			vObjPos.y -= (fValue);
			pOtherObj->SetPos(vObjPos);
		}
	}
}

void CGround::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		

		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();
		if (vObjPos.y + vObjScale.y / 2 - 5 <= vPos.y - vScale.y / 2
			&& ((CPlayer*)pOtherObj)->GetDir() == false)
		{
			pOtherObj->GetGravity()->SetGround(true);
			// 두 세로 반경의 절반 간의 합 - 두 중심점과의 차이 = 파고든 거리
			float fLen = abs(vObjPos.y - vPos.y);
			float fValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			// 파고든 거리만큼 올려줌
			vObjPos = pOtherObj->GetPos();
			vObjPos.y -= (fValue);
			pOtherObj->SetPos(vObjPos);
		}
	}
}
 
void CGround::OnCollisionExit(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player")
	{
		pOtherObj->GetGravity()->SetGround(false);		
 	}
}

