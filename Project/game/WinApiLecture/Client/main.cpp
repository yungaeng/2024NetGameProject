// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "global.h"
#include "framework.h"
#include "Client.h"
#include "CCore.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd;    //메인 윈도우 핸들
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
// *함수 전방선언
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// SAL 주석 언어 - 해당 변수의 용도를 주석처럼 적어놓음 - In, Inopt
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,         // 실행된 프로세스의 시작주소
    // 현재 윈도우 프로세스는 가상 주소를 통하여 주소의 값이 모두 같음
    _In_opt_ HINSTANCE hPrevInstance,  // 이전에 실행된 프로세스의 시작주소 - 의미x
    _In_ LPWSTR    lpCmdLine,          // 프로그램 실행시 추가로 입력한 문자열을 받아옴
    _In_ int       nCmdShow)
{
    // *쓰이지 않는 값이라는 것을 알려줌 
    UNREFERENCED_PARAMETER(hPrevInstance);
    // == hPrevInstance 전처리를 통해 의미 전달용 주석
    UNREFERENCED_PARAMETER(lpCmdLine);
    // 컴파일러는 컴파일 가치가 없는 위 코드를 무시함



    // TODO: 여기에 코드를 입력합니다.

    // ★★★ 메모리 릭(누수) 체크 ★★★
    // msdn memory leak dacator c++
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
  
    // _CrtSetBreakAlloc(117156);
    // 문제가 생겼을 경우 문제에 중단점을 걸어주는 기능 - 누수 발생시 번호를 넣어 실행해준다.
    // 문제 해결 한 후에는 다시 주석 걸어놓을 것





    // 전역 문자열을 초기화합니다.
    // IDS_APP_TITLE   = 103  리소스에 있는 아이디를 가져와서 문자열 입력
    // IDC_CLIENT      = 109
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);

    // 윈도우 정보 등록
    MyRegisterClass(hInstance);


    // 윈도우(창) 생성
    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }





    // ★Core 초기화★
    // 메세지 루프 돌기 전 코어 객체 생성 , 초기화
    // 음수이면 true - 즉 실패했을 때 실행
    if (FAILED(CCore::GetInst()->init(g_hWnd, POINT{ 1280,800 })))
    {
        MessageBox(nullptr, L"Core 객체 초기화 실패", L"ERROR", MB_OK);
        return FALSE;
    }





    // 단축키 테이블 정보 로딩
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    //  SetTimer(g_hWnd, 0, 0, nullptr);    // WM_TIMER 메세지 발생시킴
    // KillTimer(g_hWnd, 0);               // 커널오브젝트 타이머 반환(g_hWnd의 0번 타이머)        
    // GetMessage
    // 메세지 큐에서 메세지 확인될 때까지 대기
    // 확인한 메세지의 msg.message == WM_QUIT 인 경우 false 반환
    // - > 프로그램의 종료

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;

            // 기본 메시지 처리 
            // 윈도우 핸들을 주어 각자의 윈도우에서
            // 프로시저 함수를 호출하여 각자의 메시지를 처리한다
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        // 메세지가 발생하지 않는 대부분의 시간
        else
        {
            // Game 코드 수행
            // 디자인 패턴 (설계 유형)
            // 싱글톤 패턴
            CCore::GetInst()->progress();
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;//MAKEINTRESOURCEW(IDC_CLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   return TRUE;
}


//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.



POINT g_ptObjectPos = {500, 300};
POINT g_ptObjectScale = { 100,100 };

INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_MENU_TILE:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_TILE_COUNT), hWnd, TileCountProc);

                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:  // 무효화 영역(Invalidate)이 발생한 경우
        {   // ★ case 안에 지역{}괄호가 있어야만 지역변수 선언이 가능하다
            PAINTSTRUCT ps;

            // 윈도우 핸들
          // 핸들을 사용하는 이유 : 1. 명시적 표현 2. 입력 실수 방지

            HDC hDC = BeginPaint(hWnd, &ps);
            // HDC = DC의 핸들(아이디)
            // Device Context
            // 그리기 작업을 수행하는데 필요한 데이터들의 집합체
            // DC의 목적지는 hWnd
            // Dc의 펜은 기본 펜(Black)
            // Dc의 브러쉬는 기본 브러쉬(White)
           
          //  Rectangle(hDC, 1180, 668, 1280, 768);
        
            
            // 윈도우 좌표
            // (0,0)은 작업 영역의 최좌상단, 좌표의 단위는 pixel
            // pixel 자체는 메모리영역, R G B 각각 1바이트씩 3바이트로 구성 
             
            // 그리기 종료
            EndPaint(hWnd, &ps);                    
        }
        break;
    case WM_KEYDOWN:
    {
        
    }
    break;
    case WM_LBUTTONDOWN:
    {
        // 마우스 x좌표
       // g_ptLT.x = LOWORD(lParam);

        // 마우스 y좌표
      //  g_ptLT.y = HIWORD(lParam);
       

    }
    break;
    case WM_MOUSEMOVE:      //마우스 이동 중에는 우하단에 좌표 기록
        
        break;
    case WM_LBUTTONUP:
    
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// 이런 함수 있음을 알려줌 - 헤더에 선언만 하는 것과 같은 원리
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
