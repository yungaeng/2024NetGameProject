#pragma once
#include "CScene.h"
class CSound;
class CUI;
class CScene_Start :
    public CScene
{
private:

    Vec2    m_vForcePos;
    float   m_fForceRadius;
    float   m_fCurRadius;
    float   m_fForce;
    bool    m_bUseForce;

    vector<CObject*>    m_vecHeart;
    bool                m_bHeartDelete;
    bool                m_bSceneEnd;
    bool                m_bPhase;
    float               m_fEndTime;
    CSound*             m_csBgm;

    CUI*                m_pUI_Reset;
    CUI*                m_pUI_Resume;
    CUI*                m_pUI_End;
    CUI*                m_pUI_Main;
public:
    // ★★★ virtual 키워드가 없어도 이미 가상함수이지만 (상속에 의해)
    // 명시적으로 가상함수임을 표시한다.
    virtual void update();
    virtual void finalupdate();
    virtual void render(HDC _dc);
    virtual void Enter();           // 해당 씬이 시작될 떄의 초기작업
    virtual void Exit();            // 해당 씬을 탈출할 때의 작업

public:
    void CreateForce();
    void Resume();

public:
    CScene_Start();
    ~CScene_Start();
};

