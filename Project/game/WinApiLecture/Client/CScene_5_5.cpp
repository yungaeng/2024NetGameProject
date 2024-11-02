
#include "pch.h"
#include "CScene_5_5.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CCore.h"
#include "CPathMgr.h"
#include "CTexture.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include"CSceneMgr.h"
#include "CCamera.h"
#include"AI.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CRigidBody.h"
#include "SelectGDI.h"
#include "CTimeMgr.h"
#include "CGround.h"
#include "CBlock.h"

CScene_5_5::CScene_5_5()
	:m_bUseForce(false)
	, m_fForceRadius(500.f)
	, m_fCurRadius(0.f)
	, m_fForce(500.f)
{
}

CScene_5_5::~CScene_5_5()
{
}


void CScene_5_5::update()
{
	// 오버라이딩 하긴 했지만 부모의 upadate 코드를 갖다 쓸 것이다.
	//CScene::update();
	// 이럼 오버라이딩 왜함?
	// + @ 뭔가를 더 하겠다.
	//if (KEY_TAP(KEY::ENTER))
	//{
	//	// 엔터키가 눌리면 TOOL_SCENE으로 바꾸겠다
	//	// 근데 바로 씬을 바꾸는 게 아니라 이벤트에 등록해서 업뎃-파이널업뎃-렌더가 끝난 후 이벤트 처리
	//	ChangeScene(SCENE_TYPE::TOOL);
	//	
	//}

	/*if (KEY_TAP(KEY::LBTN))
	{
		Vec2 LookAt = CCamera::GetInst()->GetRealPos(MOUSE_POS);
		CCamera::GetInst()->SetLookAt(LookAt);
	}*/
	if (KEY_HOLD(KEY::LBTN))
	{
		m_bUseForce = true;
		CreateForce();
	}
	else
	{
		m_bUseForce = false;
	}

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		const vector<CObject*>& vecObj = GetGroupObject((GROUP_TYPE)(i));
		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (!vecObj[j]->IsDead())
			{
				if (m_bUseForce && vecObj[j]->GetRigidBody())
				{
					Vec2 vDiff = vecObj[j]->GetPos() - m_vForcePos;
					float fLen = vDiff.Length();
					// 물체와 힘이 발생한 지점간의 거리가, 힘의 반경보다 작으면 그 물체는 반경 안에 있다
					if (fLen < m_fForceRadius)
					{
						// 힘을 얼마나 받는 지에 대한 비율
						float fRatio = 1.f - (fLen / m_fForceRadius);
						// 적용될 힘
						float fForce = m_fForce * fRatio;		// 방향		*	적용될 힘의 크기
						vecObj[j]->GetRigidBody()->AddForce(vDiff.Normalize() * fForce);

					}
				}
				vecObj[j]->update();
			}
		}
	}
}

void CScene_5_5::render(HDC _dc)
{
	CScene::render(_dc);

	if (!m_bUseForce)
		return;
	SelectGDI gdi1(_dc, BRUSH_TYPE::HOLLOW);
	SelectGDI gdi2(_dc, PEN_TYPE::GREEN);

	m_fCurRadius += m_fForceRadius * 2.f * fDT;
	if (m_fCurRadius > m_fForceRadius)
	{
		m_fCurRadius = 0.f;
	}
	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(m_vForcePos);

	Ellipse(_dc
		, (int)(vRenderPos.x - m_fCurRadius)
		, (int)(vRenderPos.y - m_fCurRadius)
		, (int)(vRenderPos.x + m_fCurRadius)
		, (int)(vRenderPos.y + m_fCurRadius));
	//m_vForcePos;

}

void CScene_5_5::Enter()
{
	// 해당 신이 시작될 때
	// CObject : CPlayer 추가
	CObject* pObj = new CPlayer;
	pObj->SetName(L"Player");
	pObj->SetPos(Vec2(640.f, 384.f));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::PLAYER);

	// 현재 씬에 플레이어 등록
	RegisterPlayer(pObj);

	CreateBlock(MakeRandomInt(3, 10));

	// 플레이어 ★다운캐스팅 복사생성 >> 너무 기니까 clone함수 구현해서 하자
	/*CObject* pOtherPlayer = new CPlayer(*(CPlayer*)pObj);
	pOtherPlayer->SetPos(Vec2(740.f, 384.f));
	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);*/

	/*CObject* pOtherPlayer = pObj->Clone();
	pOtherPlayer->SetPos(Vec2(pObj->GetPos().x + 100, pObj->GetPos().y));
	AddObject(pOtherPlayer, GROUP_TYPE::PLAYER);*/

	// 카메라가 플레이어 따라다니기
	//CCamera::GetInst()->SetTarget(pObj);

	//// 몬스터 배치
	//// 몬스터 팩토리를 통한 생성
	Vec2 vResolution = CCore::GetInst()->GetResolution();
	CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::NORMAL, vResolution / 2.f - Vec2(0.f, 300.f));
	AddObject(pMon, GROUP_TYPE::MONSTER);

	//CreateObject(pMon) 이벤트 등록을 통한 enter가 아닌 update 중 오브젝트 생성하기


	// 수동
	//int iMonCount = 1;
	////float fMoveDistance = 25.f;
	//float fObjScale = 50.f;
	//Vec2 vResolution = CCore::GetInst()->GetResolution();
	////float fTerm = (vResolution.x - ((fMoveDistance + fObjScale/ 2.f) * 2)) / (float)(iMonCount - 1);
	//
	//// State를 가진 AI 생성
	//AI* pAI = new AI;
	//pAI->AddState(new CIdleState);
	//pAI->AddState(new CTraceState);

	//CMonster* pMonsterObj = nullptr;
	//// CObject : CMonster 추가
	//for (int i = 0; i < iMonCount; ++i)
	//{
	//	// CObject : CMonster 추가
	//	pMonsterObj = new CMonster;
	//	pMonsterObj->SetName(L"Monster");
	//	pMonsterObj->SetPos(vResolution / 2.f - Vec2(0.f,300.f));
	//	pMonsterObj->SetScale(Vec2(fObjScale, fObjScale));
	//	// AI 설정
	//	pMonsterObj->SetAI(pAI);
	////	pMonsterObj->SetMoveDistance(fMoveDistance);
	////	pMonsterObj->SetCenterPos(pMonsterObj->GetPos());


	//	AddObject(pMonsterObj, GROUP_TYPE::MONSTER);
	//}
	//LoadTile(L"Tile\\Start.tile");
	// 
	//// 땅 물체 배치 
	//CObject* pGround = new CGround;
	//pGround->SetName(L"Ground");
	//pGround->SetPos(Vec2(640.f, 584.f));
	//pGround->SetScale(Vec2(600.f, 60.f));
	//AddObject(pGround, GROUP_TYPE::GROUND);



	// 충돌 지정
	// Player 그룹과 Monster 그룹 간의 충돌 지정
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER);
	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::BLOCK);
//	CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	//CCollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::MONSTER, GROUP_TYPE::PROJ_PLAYER);


	// 카메라 LookAt 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

	start();
}

void CScene_5_5::Exit()
{
	DeleteAll();

	// 기존 해당 씬에서 지정해 놨던 충돌 그룹을 해제해 놔야 함
	CCollisionMgr::GetInst()->Reset();
}

void CScene_5_5::CreateForce()
{
	m_vForcePos = CCamera::GetInst()->GetRealPos(MOUSE_POS);
}
