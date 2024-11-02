#pragma once
#include "CUI.h"

#include "CScene.h"
#include "CObject.h"

class CScene;
class CScene_Tool;
class CObject;
class CTexture;



typedef void(*BTN_FUNC) (DWORD_PTR, DWORD_PTR);


// 반환 타입이 void, 인자도 void인 멤버 함수 포인터를 선언하는 방법
typedef void(CScene::*SCENE_MEMFUNC)(void);
//CScene으로 선언하면 CScene을 상속받은 자식 멤버함수에서도 함수에 접근할 수 있다.

// 만약 오브젝트 타입으로 하고싶다면
typedef void(CObject::* OBJECT_MEMFUNC)(void);


class CBtnUI :
    public CUI
{
private:
    BTN_FUNC        m_pFunc;
    DWORD_PTR       m_param1;
    DWORD_PTR       m_param2;

    SCENE_MEMFUNC   m_pSceneFunc;       // 그 객체가 호출할 함수
    CScene*         m_pSceneInst;       // 호출을 시킬 객체
   // CTexture*       m_pTex;             // 보유한 텍스처
public:

    virtual void MouseOn();             // 해당 UI에 마우스가 올라온 경우 호출
    virtual void MouseLbtnDown();       // 마우스가 올라운 경우에서 눌렸을 때 호출
    virtual void MouseLbtnUp();         // 마우스가 떼졌을 때
    virtual void MouseLbtnClicked();
    //virtual void render(HDC _dc);

    void SetClickedCallBack(BTN_FUNC _pFunc,DWORD_PTR _param1, DWORD_PTR _param2)
    {
        m_pFunc = _pFunc;
        m_param1 = _param1;
        m_param2 = _param2;
    }
    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc);
   

    //void SetTex(CTexture* _pTex) { m_pTex = _pTex; }

    CLONE(CBtnUI);
public:
    CBtnUI();
    ~CBtnUI();
};

