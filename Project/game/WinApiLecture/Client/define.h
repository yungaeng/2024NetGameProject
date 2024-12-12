#pragma once

// SIngleton 매크로
#define SINGLE(type) public:\
						static type* GetInst()\
						{\
							static type mgr;\
								return &mgr;\
						}\
						private:\
						type();\
						~type();\

#define fDT CTimeMgr::GetInst()->GetfDT()	// 1프레임당 시간 값 - float 버전
#define	DT	CTimeMgr::GetInst()->GetDT()	// 1프레임당 시간 값 - double 버전
#define CLONE(type) type* Clone() {return new type(*this);}

#define KEY_CHECK(key,state) CKeyMgr::GetInst()->GetKeyState(key) == state
#define KEY_HOLD(key) KEY_CHECK(key,KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key,KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key,KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key,KEY_STATE::NONE)
#define MOUSE_POS CKeyMgr::GetInst()->GetMousePos()

#define PI 3.1415926535f

// 타일 이미지는 웬만하면 2의 승수로 하는 것이 좋다.
#define TILE_SIZE 40

enum class GROUP_TYPE
{
	// 그려지는 순서 또한 중요
	DEFAULT,
	TILE,
	BG,
	BLOCK,
	GROUND,
	FLAG,	//0613 깃발 추가

	MONSTER,
	PLAYER,
	OTHER_PLAYER,
	PROJ_PLAYER,	// 플레이어 투사체
	PROJ_MONSTER,	// 몬스터 투사체


	COIN,	// 코인 타입 추가

	// 여기서 UI는 모든 오브젝트의 위에 그려지도록 순서 조정
	UI = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	INTRO,
	START,
	//end scene 추가
	EnD,


	STAGE_01,
	STAGE_02,


	

	END,
};

enum class BRUSH_TYPE
{
	HOLLOW,
	BLACK,
	END,
};

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	END,
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,
	END,
};
enum class MON_STATE
{
	IDLE,
	PATROL,
	TRACE,
	ATT,
	RUN,
	DEAD,

	END,
};

//static CCore* GetInst()
//{
//	// 정적으로 선언 시 초기화 구문은 한번만 실행
//	static CCore core;
//
//	return &core;
//}