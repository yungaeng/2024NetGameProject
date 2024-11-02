#pragma once
#include "CObject.h"
class CMissile :
    public CObject
{
private:
    float   m_fTheta;     // 이동 방향 - 각도
    
    Vec2    m_vDir;
public:
    void SetDir(float _fTheta)    {        m_fTheta =  _fTheta;    }
    void SetDir(Vec2 _vDir) 
    {
        m_vDir = _vDir; 
        m_vDir.Normalize();
    }
public:
    virtual void update();
    virtual void render(HDC _dc);

public:
    virtual void OnCollisionEnter(CCollider* _pOther);


    // 항상 이 복사생성을 할 때 복사하지말고 별개로 생성해야 할 멤버가 있나 생각하기
    CMissile* Clone() { return new CMissile(*this); }
public:
    CMissile();
    ~CMissile();
};

