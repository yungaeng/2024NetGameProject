#pragma once
#include "CUI.h"
class CPanelUI :
    public CUI
{
private:
    Vec2    m_vDragStart;

public:
    virtual void update();
    virtual void render(HDC _dc);

    virtual void MouseOn();             // 해당 UI에 마우스가 올라온 경우 호출
    virtual void MouseLbtnDown();       // 마우스가 올라운 경우에서 눌렸을 때 호출
    virtual void MouseLbtnUp();         // 마우스가 떼졌을 때
   
    CLONE(CPanelUI);
public:
    CPanelUI();
    ~CPanelUI();
};

