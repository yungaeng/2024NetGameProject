#pragma once
#include "CObject.h"


class CBlock :
    public CObject
{


public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);


    CLONE(CBlock);
public:
    CBlock();
    ~CBlock();
};

