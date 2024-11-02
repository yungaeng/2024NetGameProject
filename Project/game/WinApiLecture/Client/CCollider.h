#pragma once

class CObject;

// 충돌체를 소유한 오브젝트는 이동할 때 그 충돌체가 같이 이동하여야 함
class CCollider
{
	// 영역 검사
private:
	static UINT g_iNextID;
	// Collider 를 소유하고 있는 오브젝트
	CObject*m_pOwner;		

	// 오프셋, 기준 위치(오브젝트)로부터 상대적인 차이값 / 이 값을 이용하여 충돌체 각각을 이동시킨다.
	Vec2	m_vOffsetPos;

	// 오프셋을 이용하여 계산하여 최종 위치 결정
	Vec2	m_vFinalPos;

	// 충돌체의 크기
	Vec2	m_vScale;

	UINT	m_iID;		// 충돌체의 고유한 ID값
	int		m_iCol;

	bool	m_bActive;	// 충돌체 활성화 여부


public:
	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }
	Vec2 GetFinalPos() { return m_vFinalPos; }

	CObject* GetObj() { return m_pOwner; }
	UINT GetID() { return m_iID; }

public:
	void finalupdate();
	void render(HDC _dc);

public:
	// 충돌 시점 함수 목록
	void OnCollision(CCollider* _pOther);		// 충돌 중인 경우 호출되는 함수
	void OnCollisionEnter(CCollider* _pOther);	// 충돌에 막 진입한 경우 호출되는 함수
	void OnCollisionExit(CCollider* _pOther);	// 충돌에서 벗어난 경우 호출되는 함수


	// 대입 연산자 자체 삭제하기 - 쓸 이유는 없는 기본 연산자가 실수로라도 만들어지면 안 됨
	CCollider& operator = (CCollider& _origin) = delete;

	CCollider();
	// 개별 id 생성을 위한 복사생성자 깊은 복사 호출
	CCollider(const CCollider& _origin);

	~CCollider();


	friend class CObject;
};

