#include "pch.h"
#include "CMonster.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "AI.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CSound.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CPlayer.h"
CMonster::CMonster()
	:m_vCenterPos(Vec2(0.f,0.f))
	,m_tInfo{}
	,m_pAI(nullptr)
	,m_bGoneDead(false)
	, m_fDeadTime(0.f)
	,m_fSpeed(100)
	,m_fMaxDistance(75.f)
	,m_iDir(1)
//	,m_iHP(5)
{
	// 06/11 적 텍스쳐 추가
	CreateCollider();
	GetCollider()->SetScale(Vec2(100.f, 100.f));


	CreateAnimator();
	CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"MonsterTex", L"texture\\pepper3.bmp");

	// CreateRigidBody();


	// CreateGravity();

	GetAnimator()->CreateAnimation(
		L"Monster",
		pTex,
		Vec2(0.f, 0.f),
		Vec2(200.f, 200.f),
		Vec2(200.f, 0.f),
		0.3f,
		4);
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;
}


void CMonster::update()
{
	/*if(nullptr != m_pAI)
		m_pAI->update();*/
	//return;



	Vec2 vCurPos = GetPos();


	if (m_bGoneDead == false)
	{
		// 진행 방향으로 시간 당 m_fSpeed만큼 이동
		vCurPos.x += fDT * m_tInfo.fSpeed * m_iDir;

		float fDist = abs(m_vCenterPos.x - vCurPos.x) - m_fMaxDistance;
		// 두 위치의 차이(m_vCenterPos.x - vCurPos.x)는 절대값으로 구해야 함
		// 프레임의 변화를 고려해야한다 - 최대 배회 거리를 조금 더 넘어설 경우를 고려해야한다.
		if (0 < fDist)	// 최대 거리를 넘어 선 경우
		{
			// 방향 변경
			m_iDir *= -1;
			// 초과한 거리를 반대 방향으로 더해줌
			vCurPos.x += fDist * m_iDir;
		}

		SetPos(vCurPos);
	}



	// 06/11 적 애니메이션
	if(m_bGoneDead == false)
		GetAnimator()->Play(L"Monster", true);
	else
	{
		// 06/15 추가 
		// 죽는 상태인 경우
		if (m_fDeadTime == 0.f)
			GetRigidBody()->SetVelocity(Vec2(GetRigidBody()->GetVelocity().x, -1200.f));

		m_fDeadTime += CTimeMgr::GetInst()->GetfDT();


		if (m_fDeadTime >= 1.f)
		{
			// 06/16 사운드 추가
			CResMgr::GetInst()->LoadSound(L"Mon_Dead01", L"sound\\Mon_Dead.wav");
			CSound* pNewSound = CResMgr::GetInst()->FindSound(L"Mon_Dead01");
			pNewSound->SetVolume(60.f);
			pNewSound->Play();
			DeleteObject(this);
		}
	}
}

void CMonster::render(HDC _dc)
{
	Component_render(_dc);

	if (m_fDeadTime >= 0.8f && m_fDeadTime <= 1.f)
	{
		CTexture* pTex = CResMgr::GetInst()->LoadTexture(L"Effect", L"texture\\effect.bmp");
		Vec2 vPos = GetPos();
		Vec2 vScale = GetCollider()->GetScale();
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
		float width = (float)pTex->Width();
		float height = (float)pTex->Height();

		TransparentBlt(_dc,
			vPos.x - (float)(vScale.x)
			, vPos.y - (float)(vScale.y)
			, vScale.x * 2, vScale.y * 2
			, pTex->GetDC()
			, 0, 0, width, height
			, RGB(255,0,255));
	}
}


void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI; m_pAI->m_pOwner = this;
}

void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();	// 부딪힌 상대의 object를 받아옴
	// 부딪힌 상대 생각하기
	if (pOtherObj->GetName() == L"Player" && m_bGoneDead == false)
	{
		

		// 06.15 추가 - 플레이어와의 충돌 상호작용 구현
		// 플레이어 위치
		Vec2 vPlayerPos = pOtherObj->GetPos();

		// 몬스터(자신) 위치
		Vec2 vMyPos = GetPos();

		// 밟힐 수 있는 y위치 차이 최소 거리 > 좀 더 세세한 상호작용은 조정이 필요
		float fCanPress = 110.f;//pOtherObj->GetScale().y / 2 + this->GetCollider()->GetScale().y/2 - 20;

		// 충돌 시 플레이어와 몬스터(자신)의 y위치 차이가 밟힐 수 있는 거리보다 크면
		if (abs(vPlayerPos.y - vMyPos.y) >= fCanPress)
		{
			// 단순 삭제코드
			// DeleteObject(this);
			// 죽는 애니메이션 플레이 모드 활성화
			m_bGoneDead = true;
			CreateGravity();
			return;
		}
		((CPlayer*)pOtherObj)->Damage();
		
	}
}

void CMonster::OnCollision(CCollider* _pOther)
{

}

void CMonster::OnCollisionExit(CCollider* _pOther)
{
}
