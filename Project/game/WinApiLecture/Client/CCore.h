#pragma once
// 지역
// 전역
// 정적변수 ( 데이터 영역 사용 ) (움직이지 않는 이미지 상기)
// 1. 함수 안에 선언된 경우 - 해당 함수에서만 접근 가능 
// 
// 2. 파일 어딘가에 선언된 경우 - 해당 파일 안에서만 접근 가능
//	전역변수의 경우에는 다른 파일에 똑같은 이름의 전역변수가 있으면 오류
// 
// 3. 클래스 안에 선언된 경우
// 정적 멤버 ( 데이터 영역 사용 )
// 클래스 안에서만 접근 가능 (★객체안에 있는 것이 아님!!★)
// 객체 사이즈 안에 포함되지 않음
// 객체를 스택, 데이터, 힙 등에 만들어도 정적 멤버 자체는 데이터 영역에 존재



// 싱글톤 패턴
// 해당 클래스 객체의 생성을 1개로 제한
// 언제 어디서나 접근이 가능해야함
// extern으로 선언 해놓으면 접근 가능하지만 언제나 생성가능하기 때문에
// 생성을 제한시킬 방법을 찾아야 한다.
// 1. 생성자 숨겨놓기 - 하지만 이러면 익스턴으로도 생성이 불가능
// 2. 따라서 본인의 멤버함수로 객체를 생성
// 3. 하지만 멤버함수는 객체가 있어야 접근 가능 - 모순 (this값을 넣기 위해서)
// 4. 따라서 객체가 없어도 되는 정적멤버함수를 만든다. (static)
//	static키워드가 붙은 정적멤버함수는 객체가 없이도 호출이 가능하다
// 5. 이 경우에는 this를 통한 멤버 접근이 불가능 (호출한 객체가 없기 때문에)
// 6. ★따라서 이 정적 멤버함수는 객체를 통해 호출해도 this가 없음
// 7. 일반 멤버가 아닌 정적 멤버에는 접근이 가능하다.
//	★ 정적 멤버함수는 정적 멤버에만 접근이 가능한 것.


// 정적 멤버를 활용한 싱글톤
//class CCore
//{
//private:
//	// CCore를 가리키는 포인터를 정적 멤버로 선언 - 데이터 영역에 존재
//	static CCore* g_pInst;
//public:
//	// 정적 멤버 함수
//	 static CCore* GetInstance()
//	{
//		 // 정적 변수로 선언 - 데이터 영역에 생성, 함수가 끝나도
//		 // 사라지지 않음, 접근 계속 가능
//		 // static으로 선언했기에 초기화는 초기 1번만 발동
//		//  삭제, 재생성 문제가 있어서 정적 멤버로 바꿈
//
//		 // 최초 호출 된 경우
//		 if (nullptr == g_pInst)
//		 {
//			 // CCore 객체 자체는 현재 힙 영역에 존재
//			  g_pInst = new CCore;
//		 }
//		return g_pInst;
//	}
//
//	 static void Release()
//	 {
//		 if (nullptr != g_pInst)
//		 {
//			 delete g_pInst;
//			 g_pInst = nullptr;
//		 }
//	 }
//
//	 // 외부에서는 CCore 객체를 생성할 수 없도록 접근을 private으로 막음
//private:
//	CCore();
//	~CCore();
//};
//
//// 정적 멤버는 외부에서 초기화가 반드시 필요하다.




// 정적 멤버함수의 정적 변수로 활용하는 싱글톤
// 
// 원하는대로 해제가 불가능함 - 장점이자 단점
class CCore
{
//public:
	//static CCore* GetInst()
	//{
	//	// 정적으로 선언 시 초기화 구문은 한번만 실행
	//	static CCore core;

	//	return &core;
	//} 아래로 매크로 설정
	SINGLE(CCore);
private:
	HWND m_hWnd;			// 메인 윈도우 핸들
	POINT m_ptResolution;	// 메인 윈도우 해상도
	HDC m_hDC;				// 메인 윈도우에 Draw 할 DC

	HBITMAP m_hBit;
	HDC m_memDC;
	

	// 자주 사용하는 GDI Object
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];


	HMENU	m_hMenu;	// tool_scene에서 사용할 메뉴


public:
	int m_iCoin;		// 프로그램 중 코인의 개수
	int m_iHP;



public :
	int init(HWND _hWnd,POINT _ptResolution);
	void progress();
	
private:
	void Clear();
	void CreateBrushPen();
public:
	void DockMenu();
	void DivideMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);
public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	POINT GetResolution() { return m_ptResolution; }
	//HMENU GetMenu() { return m_hMenu; }

	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }
};
