#pragma once
#include "global.h"

class CCollider;
class CAnimator;
class CRigidBody;
class CGravity;

#include "CCamera.h"

// 물체의 좌표와 위치
// 모든 오브젝트의 부모 파트, 실제로 이 객체를 쓸 일은 없다!
// = > 순수가상함수를 포함한 추상클래스로 대체한다.
class CObject
{
private:

	wstring			m_strName;	// Name = 무슨 오브젝트인지 체크
	Vec2			m_vPos;		// 좌표 정보
	Vec2			m_vScale;	// 크기 정보

	// ★★★ component
	// 충돌 기능에 필요한 정보 컴포넌트로 추가, 포인터니까 필요하면 동적할당, 아니면 null -> 비활성화
	CCollider*		m_pCollider;
	CAnimator*		m_pAnimator;
	CRigidBody*		m_pRigidBody;
	CGravity*		m_pGravity;
	bool m_bAlive;

public:
	void SetPos(Vec2 _vPos) { this->m_vPos = _vPos; }
	Vec2 GetPos() { return this->m_vPos; } 

	void SetScale(Vec2 _vScale) { this->m_vScale = _vScale; }
	Vec2 GetScale() { return this->m_vScale; }

	void SetName(const wstring& _strName) { m_strName = _strName; }
	wstring& GetName() { return m_strName; }
	
	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }
	CRigidBody* GetRigidBody() { return m_pRigidBody; }
	CGravity* GetGravity() { return m_pGravity; }



	// 충돌클래스가 필요한 객체는 충돌객체 생성(collider)
	void CreateCollider();
	void CreateAnimator();
	void CreateRigidBody();
	void CreateGravity();
	bool IsDead() { return !m_bAlive; }



	// 충돌 시점 함수 목록 - 오브젝트가 처리할 충돌
	virtual void OnCollision(CCollider* _pOther) {}		// 충돌 중인 경우 호출되는 함수
 	virtual void OnCollisionEnter(CCollider* _pOther) {}	// 충돌에 막 진입한 경우 호출되는 함수
	virtual void OnCollisionExit(CCollider* _pOther) {}

private:
	void SetDead() { m_bAlive = false; }

public:
	virtual void start() {};		// Scene이 시작되기 직전에 호출되는 함수
	// 각 오브젝트의 상속받은 클래스들마다 update와 render 필요
	virtual void update() = 0;		// 순수가상함수 > 반드시 자식에서 오버라이드 해야함

	// 가상함수로 오버라이드 시키는 것이 아닌, 오브젝트의 컴포넌트로 하여금 실행하게 함
	virtual void finalupdate();	// final 키워드로 더 이상의 오버라이딩을 금지
	virtual void render(HDC _dc);

	void Component_render(HDC _dc);

	// 순수가상함수가 있는 추상클래스이므로 이것도 순수가상함수로 강제 오버라이딩 시킨다
	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual~CObject();

	friend class CEventMgr;
};

// 컴포넌트 기반 구조
// 너무 상속의 깊이가 깊고 그러면 확장성이 떨어진다.
// 필요한 멤버 변수를 모두 포함시키고, 필요한 경우에만 활성화 되도록 한다.
