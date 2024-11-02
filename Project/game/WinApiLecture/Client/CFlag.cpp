#include "pch.h"
#include "CFlag.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CMissile.h"
#include "CSceneMgr.h"
#include "CScene.h"
#include "CSound.h"

#include "CPathMgr.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"
#include "CGravity.h"


CFlag::CFlag()
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(140.f, 115.f));

	CreateAnimator();
	// 캐릭터의 상태를 먼저 설정한 후 , 그 상태에 맞게 캐릭터의 애니메이션을 정하자.
	// 맞은 상태, 움직이는 상태, 점프 상태, 등등 그 상태의 전환을 완벽히 실행한 뒤


	// ==06.10 수정== coin2.bmp(양식에 맞게 수정한 이미지)를 참조하도록 변경
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"FlagTex", L"texture\\flag.bmp");

	//깃발 애니메이션
	GetAnimator()->CreateAnimation(
		L"Flag",
		pTex,
		Vec2(0.f, 0.f),
		Vec2(148.f, 150.f),
		Vec2(149.f, 0.f),
		0.1f,
		6);
}

CFlag::~CFlag()
{
} 

void CFlag::update()
{
	GetAnimator()->Play(L"Flag", true);
}

void CFlag::render(HDC _dc)
{
	Component_render(_dc);
}

void CFlag::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();	// 부딪힌 상대의 object를 받아옴
	if (pOtherObj->GetName() == L"Player")
	{
		CResMgr::GetInst()->LoadSound(L"End", L"sound\\end.wav");
		CSound* pNewSound = CResMgr::GetInst()->FindSound(L"End");
		pNewSound->Play();
		ChangeScene(SCENE_TYPE::EnD);
	}
}

void CFlag::OnCollision(CCollider* _pOther)
{
	
}

void CFlag::OnCollisionExit(CCollider* _pOther)
{
}
