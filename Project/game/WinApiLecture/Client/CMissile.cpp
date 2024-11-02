#include "pch.h"
#include "CMissile.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CCamera.h"




CMissile::CMissile()
	:m_fTheta(PI / 4.f)
	,m_vDir(Vec2(1.f,1.f))
{
	// 벡터의 정규화 (normalize)
	// 방향에 따르는 길이(크기)를 1로 정한다.
	m_vDir.Normalize();
	CreateCollider();
	GetCollider()->SetScale((Vec2(15.f, 15.f)));
}

CMissile::~CMissile()
{
}


void CMissile::update()
{
	Vec2 vPos = GetPos();

	// 어느 방향으로 가든 1초에 n만큼 갈 수 있도록 조정이 필요하다.
	// 삼각함수를 이용한 라디안, 방향 - (cos 0, sin 0)
	// 지금은 벡터를 이용한 방향 설정을 사용한다.
	// 벡터의 정규화 이용
	vPos.x += 600.f * m_vDir.x * fDT;
	vPos.y += 600.f * m_vDir.y * fDT;

	SetPos(vPos);
}

void CMissile::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	Ellipse(_dc
		, (int)(vPos.x - vScale.x / 2.f)
		, (int)(vPos.y - vScale.y / 2.f)
		, (int)(vPos.x + vScale.x / 2.f)
		, (int)(vPos.y + vScale.y / 2.f));
			   
	Component_render(_dc);
}


void CMissile::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();	// 부딪힌 상대의 object를 받아옴
	// 부딪힌 상대 생각하기
	if (pOtherObj->GetName() == L"Monster")
	{
		DeleteObject(this);
	}
}


