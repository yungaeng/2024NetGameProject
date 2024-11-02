#pragma once
#include "CObject.h"


struct tMonInfo
{
    float       fSpeed;           // 속도
    float       fHP;              // 체력
    float       fRecgRange;       // 인지 범위
    float       fAttRange;        // 공격 범위
    float       fAtt;             // 공격력
};


class AI;
class CMonster :
    public CObject
{

private:
    tMonInfo     m_tInfo;            
    AI*          m_pAI;              // AI

    Vec2    m_vCenterPos;       // 중심점
    float   m_fSpeed;           // 속도
    float   m_fMaxDistance;     // 최대 이동거리
    int     m_iDir;             // 1이면 오른쪽, -1이면 왼쪽
    //int     m_iHP;
public:
    bool m_bGoneDead;         // 죽는 상태 활성 유무
    float m_fDeadTime;
 

public:
    float GetSpeed() { return m_tInfo.fSpeed; }
    void SetSpeed(float _f) { m_tInfo.fSpeed = _f; }
   /* void SetMoveDistance(float _f) { m_fMaxDistance = _f; }
    void SetCenterPos(Vec2 _vPos) { m_vCenterPos = _vPos; }*/
  
    void SetAI(AI* _AI);

    // 수정불가능 참조로 복사 크기를 줄여줌
    const tMonInfo& GetMonInfo() { return m_tInfo; }

private:
    void SetMonInfo(const tMonInfo& _info) { m_tInfo = _info; }
 
public:
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);
public:
    virtual void update();
    virtual void render(HDC _dc);


    CMonster* Clone() { return new CMonster(*this); }
   
public:
    CMonster();
    ~CMonster();
    friend class CMonFactory;
};

