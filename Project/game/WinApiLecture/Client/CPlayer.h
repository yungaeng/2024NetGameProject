#pragma once
#include "CObject.h"

enum class PLAYER_STATE
{
    IDLE,
    WALK,
    ATTACK,
    GIANT,
    DAMAGED,
    DEAD,
    JUMP,

};

enum class PLAYER_JUMP_STATE
{
    JUMP_UP = 10,
    JUMP_AIR,
    JUMP_DOWN,

    DOUBLE_JUMP_UP,
    DOUBLE_JUMP_AIR,
    DOUBLE_JUMP_DOWN,
};


enum class PLAYER_ATTACK_STATE
{
    NOMAL_ATT_1 = 20,
    NOMAL_ATT_2,
    NOMAL_ATT_3,

    SKILL_ATT_1,
    SKILL_ATT_2,
    // ...
};


enum class PLAYER_GIANT_STATE
{
    GIANT_NOT = 30,
    GIANT_ENTER,
    GIANT_PRG,
    GIANT_ESCAPE
};



class CTexture;

class CPlayer :
    public CObject
{
private:
  //  CTexture* m_pTex;

    PLAYER_STATE            m_eCurState;
    PLAYER_STATE            m_ePrevState;

    // 점프 상태
    PLAYER_JUMP_STATE       m_eCurJumpState;
    PLAYER_JUMP_STATE       m_ePrevJumpState;
    int                     m_iJumpStack;   // 점프 가능 횟수
    int                     m_iHP;          // 체력

    // 거대화 상태       06/16 수정
    PLAYER_GIANT_STATE       m_eCurGiantState;
    PLAYER_GIANT_STATE       m_ePrevGiantState;
    float                    m_fGiantTime;

    float                    m_fDeatTime;

    bool                     m_bSlow;
    float                    m_fSlowTime;
    bool                     m_bDir;
    bool                     m_bDamaged;
    

    public:
        virtual void update();
        virtual void render(HDC _dc);
        void Damage() { --m_iHP; m_bDamaged = true; }
        bool GetDir() { return m_bDir; }
        int GetHP() { return m_iHP; }
        bool GetDamaged() { return m_bDamaged; }
        void SetDamaged(bool _b) { m_bDamaged = _b; }
private:
    void CreateMissile();
    void update_state();
    void update_move();
    void update_animation();
  //  void update_gravity();

    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollision(CCollider* _pOther);


    // CObject의 clone 오버라이딩 _ 복사생성자
   // CPlayer* Clone() { return new CPlayer(*this); }
    CLONE(CPlayer);
public:
    CPlayer();
    // 굳이 만들 필요 없지만
    // 컴파일러는 이런 식으로 복사생성자를 만들 것이다
    //CPlayer(const CPlayer& _origin)
    //    :CObject(_origin)   // 부모 쪽 생성자에서 생성
    //    ,m_pTex(_origin.m_pTex)
    //{
    //}
    ~CPlayer();
};

