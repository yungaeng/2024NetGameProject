#include "pch.h"
#include "CObject.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CCamera.h"
#include "CRigidBody.h"
#include "CGravity.h"

CObject::CObject()
	:m_vPos{}
	,m_vScale{}
	,m_pCollider(nullptr)
	,m_pAnimator(nullptr)
	,m_pRigidBody(nullptr)
	,m_pGravity(nullptr)
	,m_bAlive(true)

{
	
}

// ★★★부모쪽 파트는 부모쪽 생성자가 생성시킨다.
CObject::CObject(const CObject& _origin)
	:m_strName(_origin.m_strName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_bAlive(true)
{
	// 원본 오브젝트가 콜라이더를 보유할 경우에만 복사
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}
	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}
	if (_origin.m_pGravity)
	{
		m_pGravity = new CGravity(*_origin.m_pGravity);
		m_pGravity->m_pOwner = this;
	}
	
}

CObject::~CObject() 
{
	if (nullptr != m_pCollider)
	{
		delete m_pCollider;
	}
	if (nullptr != m_pAnimator)
	{
		delete m_pAnimator;
	}
	if (nullptr != m_pRigidBody)
	{
		delete m_pRigidBody;
	}
	if (nullptr != m_pGravity)
	{
		delete m_pGravity;
	}

}




void CObject::finalupdate()
{
	

	// 강체 final update		--> 강체 업데이트 할 때, 플레이어의 위치를 변경시키는 경우가 있음
	if (m_pRigidBody)
		m_pRigidBody->finalupdate();

	// 충돌체 final update
	if (m_pCollider)
		m_pCollider->finalupdate();

	// 애니메이터 final update
	if (m_pAnimator)
		m_pAnimator->finalupdate();
	if (m_pGravity)
		m_pGravity->finalupdate();


}

void CObject::render(HDC _dc)
{
	
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vPos);

	Rectangle(_dc
		, (int)(vRenderPos.x - m_vScale.x / 2)
		, (int)(vRenderPos.y - m_vScale.y / 2)
		, (int)(vRenderPos.x + m_vScale.x / 2)
		, (int)(vRenderPos.y + m_vScale.y / 2));

	Component_render(_dc);
}

void CObject::Component_render(HDC _dc)
{
	
	if (nullptr != m_pAnimator)
	{
		m_pAnimator->render(_dc);
	}

	if (nullptr != m_pCollider)
	{
		m_pCollider->render(_dc);
	}
	if (nullptr != m_pRigidBody)
	{
		//m_pRigidBody->render(_dc);
	}
}


void CObject::CreateCollider()
{
	m_pCollider = new CCollider;
	// 충돌체 생성한 오브젝트의 주소를 충돌체에 입력
	m_pCollider->m_pOwner = this;
}

void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void CObject::CreateRigidBody()
{
	m_pRigidBody = new CRigidBody;
	m_pRigidBody->m_pOwner = this;
}

void CObject::CreateGravity()
{
	m_pGravity = new CGravity;
	m_pGravity->m_pOwner = this;
}

