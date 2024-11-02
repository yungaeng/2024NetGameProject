#pragma once
#include "CObject.h"

// 각 신에는 큰 UI하나가 있고, 그 UI로부터 다른 UI들이 상속되어 구현한다.
// 따라서 부모 UI가 자식UI를 생성하도록 한다.
// 이점 1. 렌더링 순서가 자연스럽게 지켜진다.
// 이점 2. 부모가 child 또한 업데이트,파이널업데이트,렌더시키기 때문에 따로따로 할 필요..가 없다?

class CTexture;
class CUI :
    public CObject
{
private:
    vector<CUI*>    m_vecChildUI;       // 자식 UI들의 집합을 가짐
    CUI*            m_pParentUI;        // 자신의 부모 pUI
    Vec2            m_vFinalPos;        // 최종위치
    CTexture*       m_pTex;             // 본인이 보유한 텍스처
    bool            m_bCamAffected;     // UI가 카메라에 영향을 받는 유무
    bool            m_bMouseOn;         // UI위에 마우스가 있는지
    bool            m_bLbtnDown;        // UI위에서 왼쪽 버튼이 눌린 적이 있는지
    

public:
    Vec2 GetFinalPos() { return m_vFinalPos; }
    CUI* GetParent() { return m_pParentUI; }    // 자신의 부모 pUI 알아오기
    bool IsMouseOn() { return m_bMouseOn; }
    bool ISLbtnDown() { return m_bLbtnDown; }
    void SetTex(CTexture* _pTex) { m_pTex = _pTex; }

    // 자신의 자식 UI 생성하고 부모는 나로 설정하기
    void AddChild(CUI* _pUI) { m_vecChildUI.push_back(_pUI); _pUI->m_pParentUI = this; }   
    const vector<CUI*>& GetChildUI() { return m_vecChildUI; }

private:    // 자식 관련 업데이트 - 렌더 함수
    virtual void update_child();
    virtual void finalupdate_child();
    virtual void render_child(HDC _dc);

    void MouseOnCheck();
public:     // 본인 관련 업데이트 - 렌더 함수
    virtual void finalupdate();
    virtual void update();
    virtual void render(HDC _dc);

public:     // UI Event 함수
    virtual void MouseOn();             // 해당 UI에 마우스가 올라온 경우 호출
    virtual void MouseLbtnDown();       // 마우스가 올라운 경우에서 눌렸을 때 호출
    virtual void MouseLbtnUp();         // 마우스가 떼졌을 때
    virtual void MouseLbtnClicked();    // 마우스가 클릭되었을 때: Down과 Up이 같은 UI내에서 발생..?
    virtual CUI* Clone() = 0;
public:
    // 기본생성자 대신 카메라 bool을 받는 생성자를 설정하여 반드시 카메라 영향 유무 입력 유도
    CUI(bool _bCamAff);
    CUI(const CUI& _origin);
    virtual ~CUI();

    friend class CUIMgr;
};

