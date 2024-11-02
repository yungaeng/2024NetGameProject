#include "pch.h"
#include "CGravity.h"
#include "CObject.h"
#include "CRigidBody.h"

CGravity::CGravity()
	:m_pOwner(nullptr)
	, m_bGround(false)
{

}

CGravity::~CGravity()
{
	
}

void CGravity::SetGround(bool _b)
{
	m_bGround = _b;
	if (m_bGround)
	{
		// 만약 중력 컴포넌트 Ground 상태이면 지금까지 적용된 y축의 속도를 0으로 한다
		Vec2 vVel = m_pOwner->GetRigidBody()->GetVelocity();
		m_pOwner->GetRigidBody()->SetVelocity(Vec2((vVel.x), 0.f));
	}
}

void CGravity::finalupdate()
{ 
	m_pOwner->GetRigidBody()->SetAccelAlpha(Vec2(0.f, 800.f));
}

