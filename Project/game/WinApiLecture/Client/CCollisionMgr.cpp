#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

CCollisionMgr::CCollisionMgr()
	:m_arrCheck{}
{

}
CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::update()
{
	// 충돌 검사
	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; ++iRow)
	{
		m_arrCheck[iRow];
		// 검사는 iRow 번째 비트부터 검사를 시작한다. (배열 값이  / 대칭이기 때문에 한번만 검사)
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; ++iCol)
		{
			// iRow행의 32비트(4바이트 UINT)중 iCol번째 비트에 있는 비트가 1과 end(1)인경우
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				// 현재씬의 두 그룹의 오브젝트를 가져와서 검사를 진행 - 신과 신매니저 include
				CollisionUpdateGroup((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionUpdateGroup(GROUP_TYPE _eleft, GROUP_TYPE _eright)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	// 받은 얘는 그냥 단순한 지역변수일 뿐, 받는 대상도 레퍼런스 화를 해줘야 원본과 동일해진다.
	// 따라서 받을 때도 레퍼런스로 받지 않으면 복사비용이 추가된다.
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eleft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eright);

	// 반복문에서 초기화 하지 않고 여기서 선언
	map<ULONGLONG, bool>::iterator iter;

	// 만약 같은 그룹을 받아온 경우, 자기 자신과의 충돌은 예외시켜야 한다.
	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// 만약 충돌체가 없는 경우
		if (nullptr == vecLeft[i]->GetCollider())
		{
			continue;	// 컨티뉴를 만나 그냥 이 반목문 과정을 끝내고 조건값 증가 후 다음 반복문
		}
		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 만약 충돌체가 없는 경우, 만약 자기 자신과의 충돌을 검사하려는 경우
			if (nullptr == vecRight[j]->GetCollider()
				|| (vecLeft[i] == vecRight[j]))
			{
				continue;	// 컨티뉴를 만나 그냥 이 반목문을 끝내고 다음 반복문 증가
			}
		
			CCollider* pLeftCol = vecLeft[i]->GetCollider();
			CCollider* pRightCol = vecRight[j]->GetCollider();

			// 본격적인 충돌체크 - 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();
			// 이렇게 완성된 4 + 4 8바이트 ID는 고유한 ID가 된다

			iter = m_mapColInfo.find(ID.ID);

			// 충돌 정보가 미 등록 상태인 경우 (충돌하지 않았다=false 로 설정)
			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			if (IsCollision(pLeftCol, pRightCol))
			{
				// 현재 충돌 중이다.

				if (iter->second)
				{
					// 이전에도 충돌하고 있었다. ( 누구랑 )

					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// 둘 중 하나가 죽을 예정이면 충돌을 벗어난 판정으로 해줘야한다.
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{	// 죽을 예정이 아니라면 계속 충돌 중이다.
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}
				else
				{
					// 이전에는 충돌하지 않았지만 지금 충돌했다 ( 누구랑 ) 근데 이제 막 삭제될 참이다.
					// 그럼 차라리 충돌 안 된 걸로 하자 
					// 아니라면 이제 충돌에 진입한 것이다.
					if (!vecLeft[i]->IsDead() && !vecRight[j]->IsDead())
					{			
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
					
				
				}
			}
			else// 충돌하고 있지 않은 경우
			{
				if (iter->second)
				{
					// 충돌하다가 이번에 충돌이 끝났다. ( 누구랑 )
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
				
			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();
	
	if (((abs(vRightPos.x - vLeftPos.x)) <= (vLeftScale.x + vRightScale.x) / 2.f)
		&& ((abs(vRightPos.y - vLeftPos.y)) <= (vLeftScale.y + vRightScale.y) / 2.f))
	{
		return true; 
	}
	return false;
}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eright)
{
	// 그룹과 그룹간에 충돌 지정 시 enum값이 작은 그룹의 enum을 행으로 써야 
	// 쓰는 배열 쪽에만 표시 가능
	// 더 작은 값으 그룹 타입을 행으로,
	// 큰 값을 열(비트)로 사용

	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eright;

	// 열이 더 큰 상황에는 두 값을 바꿔 다시 넣는다.
	if (iCol < iRow)
	{
	 iRow = (UINT)_eright;
	 iCol = (UINT)_eLeft;
	}
	// 이미 1이어서 end연산이 1이 나온 경우
	if (m_arrCheck[iRow] & (1 << iCol))
	{
		// 반전해서 넣는다.
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{// 1을 iCol만큼 비트 레프트 쉬프트 해서 값을 넣어준다.
		m_arrCheck[iRow] |= (1 << iCol);
	}
}

