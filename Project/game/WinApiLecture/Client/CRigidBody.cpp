#include "pch.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CObject.h"

CRigidBody::CRigidBody()
	:m_pOwner(nullptr)
	,m_fMass(1.f)
	, m_vMaxVelocity(Vec2(200.f,600.f))
	,m_fFricCoef(100.f)
{
}

CRigidBody::~CRigidBody()
{
}


void CRigidBody::finalupdate()
{
	// 힘의 크기
	float fForce = m_vForce.Length();	

	// 강체에 적용된 힘이 0이면 진행하지 않음
	if (0.f != fForce)
	{
		// 힘의 방향으로 정규화
		m_vForce.Normalize();

		//가속도의 크기 = 힘의크기(|f| / m)
		float m_fAccel = fForce / m_fMass;


		//	힘의 방향 * 가속도의 크기		 가속도 : f = ma, a = f / m	
		m_vAccel =	m_vForce * m_fAccel; // m_vForce / m_fMass;	
	}

	// 추가 가속도
	m_vAccel += m_vAccelA;

	// 속도 : 현재 속도에 가속도 * fDT 만큼을 더해 업데이트한다.
	m_vVelocity += m_vAccel * fDT;


	if (!m_vVelocity.IsZero())
	{
		Vec2 vFricDir = -m_vVelocity;
		vFricDir.Normalize();
		// 마찰력에 의한 반대 방향으로의 가속도 적용 
		// 현재 속도의 반대 방향 단위 벡터에 마찰계수를 곱해 현재 속도에 더함
		Vec2 vFriction = vFricDir * m_fFricCoef * fDT;
		if (m_vVelocity.Length() <= vFriction.Length())
		{
			// 마찰 가속도가 본래 속도보다 더 큰 경우 속도를 멈춤
			m_vVelocity = Vec2(0.f, 0.f);
		}
		else
		{
			m_vVelocity += vFriction;
		}
	}

	// 속도 제한 검사
	if (abs(m_vMaxVelocity.x) < abs(m_vVelocity.x))
	{
		// 현재 자신의 x축 속력의 크기로 x축 속도를 나누어 정규화 -> x축 최대 속도를 곱함
		m_vVelocity.x = (m_vVelocity.x / abs(m_vVelocity.x)) * abs(m_vMaxVelocity.x);
	}

	if (abs(m_vMaxVelocity.y) < abs(m_vVelocity.y))
	{
		// 현재 자신의 x축 속력의 크기로 x축 속도를 나누어 정규화 -> x축 최대 속도를 곱함
		m_vVelocity.y = (m_vVelocity.y / abs(m_vVelocity.y)) * abs(m_vMaxVelocity.y);
	}

	// 속도에 따른 이동
	Move();

	// 다음 적용될 힘을 온전히 받기 위해 자신이 이번 프레임에 받았던 힘을 초기화
	m_vForce = Vec2(0.f, 0.f);

	// 추가 가속도, 가속도 초기화
	m_vAccel = Vec2(0.f, 0.f);
	m_vAccelA = Vec2(0.f,0.f);
	
}


void CRigidBody::Move()
{
	// 속력
	float fSpeed = m_vVelocity.Length();
	
	if (0.f != fSpeed)
	{
		// 이동 방향
		Vec2 vDir = m_vVelocity;
		// 노말라이즈하면 원본을 훼손하므로 지역변수로 정규화
		vDir.Normalize();
		Vec2 vPos = m_pOwner->GetPos();
		vPos += m_vVelocity * fDT;

		m_pOwner->SetPos(vPos);
	}

	

}

