#pragma once

class CCollider;

union COLLIDER_ID
{
	struct {
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};


class CCollisionMgr
{
	SINGLE(CCollisionMgr)
private:
	// 그룹 개수만큼의 양수 정수 배열 설정
	UINT				 m_arrCheck[(UINT)GROUP_TYPE::END];	// 그룹간의 충돌 체크 매트릭스
	
	map<ULONGLONG, bool> m_mapColInfo;		// 충돌체 간의 이전 프레임 충돌 정보
		// first: 두 충돌체간의 키 값 , second: 충돌을 하고 있었는가? 아닌가?
public:
	void update();
	void CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eright);
	// 32 * int(byte)공간 배열을 0으로 (int) * 32바이트만큼 초기화 
	void Reset()	{memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END);}

	// memset() -> 내가 지정한 주소를 바이트 단위로 채우기

private:
	// 자기 자신의 함수에서 호출할 함수이므로 private,
	// 그룹간의 충돌 업데이트
	void CollisionUpdateGroup(GROUP_TYPE _eleft, GROUP_TYPE _eright);
	bool IsCollision(CCollider* _pLeft,CCollider* _pRight);

};

