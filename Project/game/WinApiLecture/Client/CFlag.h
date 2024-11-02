#include "CObject.h"

class CFlag :
    public CObject
{


public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther);

    CLONE(CFlag);
public:
    CFlag();
    ~CFlag();
};


