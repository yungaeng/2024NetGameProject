
#include "pch.h"
#include "CBlock.h"
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



CBlock::CBlock()
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(40.f, 40.f));

	CreateAnimator();
	// 캐릭터의 상태를 먼저 설정한 후 , 그 상태에 맞게 캐릭터의 애니메이션을 정하자.
	// 맞은 상태, 움직이는 상태, 점프 상태, 등등 그 상태의 전환을 완벽히 실행한 뒤


	// ==06.10 수정== coin2.bmp(양식에 맞게 수정한 이미지)를 참조하도록 변경 - 코인 3 (이미지 축소) 텍스처 생성
	CTexture* pCoinTex = CResMgr::GetInst()->LoadTexture(L"CoinTex", L"texture\\coin3.bmp");

	// 06/16 수정 각종 아이템들 텍스처 추가
	CTexture* pItemsTex = CResMgr::GetInst()->LoadTexture(L"ItemsTex", L"texture\\Items.bmp");

	//CTexture* pBananaTex = CResMgr::GetInst()->LoadTexture(L"BananaTex", L"texture\\Banana.bmp");


	//// ==06.10 수정==	(양식에 맞게 애니메이션 조정)
	//GetAnimator()->CreateAnimation(
	//	L"IDEL_1",
	//	pTex,
	//	Vec2(0.f, 0.f),
	//	Vec2(101.f, 115.f),
	//	Vec2(103.5f, 0.f),
	//	0.2f,
	//	6);

	// ==06.16 수정==	, 코인 크기 감소(양식에 맞게 애니메이션 조정)
	GetAnimator()->CreateAnimation(
		L"Coin_1",
		pCoinTex,
		Vec2(0.f, 0.f),
		Vec2(81.5f, 92.f),
		Vec2(83.f, 0.f),
		0.2f,
		6);


	// ==06.16 수정==	, 코인 크기 감소(양식에 맞게 애니메이션 조정)
	GetAnimator()->CreateAnimation(
		L"Big_Potion1",
		pItemsTex,
		Vec2(1770.f, 1208.f),
		Vec2(90.f, 90.f),
		Vec2(92.f, 0.f),
		0.2f,
		4);

	// ==06.16 수정==	, 코인 크기 감소(양식에 맞게 애니메이션 조정)
	GetAnimator()->CreateAnimation(
		L"Banana",
		pItemsTex,
		Vec2(886.f, 1801.f),
		Vec2(65.f, 65.f),
		Vec2(0.f, 0.f),
		10.f,
		1);

	/*GetAnimator()->CreateAnimation(
		L"HP",
		pItemsTex,
		Vec2(370.f, 2071.f),
		Vec2(89.f, 89.f),
		Vec2(92.f, 0.f),
		0.3f,
		4);*/


}

CBlock::~CBlock()
{
}

void CBlock::update()
{
	// 06/16 수정 코인이 아니면 코인의 애니메이션 사용 x
	if(GetName() == L"Coin")
		GetAnimator()->Play(L"Coin_1", true);
	else if (GetName() == L"Big_Potion")
		GetAnimator()->Play(L"Big_Potion1", true);
	else if(GetName() == L"Banana")
		GetAnimator()->Play(L"Banana", true);
	

}

void CBlock::render(HDC _dc)
{
	// ==06.10 수정== 코인이 아니면 CObject렌더 사용, 코인이면 컴포넌트(애니메이션) 랜더 사용
	if (L"Coin" == GetName())
		Component_render(_dc);
	else if (L"Big_Potion" == GetName())
		Component_render(_dc);
	else if (L"Banana" == GetName())
		Component_render(_dc);
	else
	{
			CTexture* pHeartTex = CResMgr::GetInst()->LoadTexture(L"HeartTex", L"texture\\Heart.bmp");
			Vec2 vPos = GetPos();
			vPos.x = abs(vPos.x - CCamera::GetInst()->GetLookAt().x);

			vPos = CCamera::GetInst()->GetRenderPos(vPos);
			Vec2 vScale = GetScale();
			TransparentBlt(_dc,
				vPos.x - (float)(vScale.x / 2)
				, vPos.y - (float)(vScale.y / 2)
				, vScale.x, vScale.y
				, pHeartTex->GetDC()
				, 0, 0, pHeartTex->Width(), pHeartTex->Height()
				, RGB(255,0,255));
		
	}

}

void CBlock::OnCollisionEnter(CCollider* _pOther)
{
	
}

void CBlock::OnCollision(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();	// 부딪힌 상대의 object를 받아옴
	if (pOtherObj->GetName() == L"Player" )
	{
		if (GetName() == L"Coin")
		{

		// 06/16 사운드 추가
			CResMgr::GetInst()->LoadSound(L"Coin_01", L"sound\\Coin.wav");
			CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Coin_01");

			pNewSound->Play();

			pNewSound->SetPosition(0.f);			// 백분율, 재생 위치 설정 ex 50% 구간
			pNewSound->SetVolume(30.f);				// 0~100 까지 볼륨 설정
		}
		else
		{
			CResMgr::GetInst()->LoadSound(L"Gigantic_01", L"sound\\gigantic_start.wav");
			CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Gigantic_01");
			pNewSound->Play();
			//pNewSound->SetPosition(0.f);			// 백분율, 재생 위치 설정 ex 50% 구간
			pNewSound->SetVolume(30.f);				// 0~100 까지 볼륨 설정
		}

		DeleteObject(this);
	}

	if (pOtherObj->GetName() == L"Monster")
	{
		DeleteObject(this);
	}
}

void CBlock::OnCollisionExit(CCollider* _pOther)
{
}
