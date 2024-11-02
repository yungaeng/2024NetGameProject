#pragma once
#include "CScene.h"
class CUI;

class CScene_Intro :
    public CScene
{
private:
    CUI* m_pUI;

public:
    bool m_bExplain;
    virtual void update();
    virtual void Enter();
    virtual void Exit();


    void Explain();


    // 웬만하면 이넘클래스로 분기처리를 해서 여러가지 동작을 할 수 있도록 ㄱㄱ

public:
    CScene_Intro();
    ~CScene_Intro();
};


