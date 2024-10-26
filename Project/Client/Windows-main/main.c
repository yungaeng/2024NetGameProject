#define window_size_w 750
#define window_size_d 650
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <mmsystem.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include "resource.h"
#pragma comment(lib, "winmm.lib")

typedef struct box {
	int left, top, right, bottom;
} box;

int map_kind = 1, fst_char_kind = 1, sec_char_kind = 1;

int collision(box A, box B);
int collision(box A, box B)
{
	if (A.left >= B.left && A.left < B.right && A.top < B.bottom && A.bottom > B.top) // left
		return 1;
	if (A.right > B.left && A.right <= B.right && A.top < B.bottom && A.bottom > B.top) // right
		return 1;
	if (A.left < B.right && A.right > B.left && A.top < B.bottom && A.top >= B.top) // top
		return 1;
	if (A.left < B.right && A.right > B.left && A.bottom <= B.bottom && A.bottom > B.top) // bottom
		return 1;
	return 0;
}

void break_block();
void break_block()
{

}

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming Lab";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlalog_Proc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	int FrameX = GetSystemMetrics(SM_CXFRAME), FrameY = GetSystemMetrics(SM_CYFRAME), Caption = GetSystemMetrics(SM_CYCAPTION);

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 100, 100, window_size_w, window_size_d, NULL, (HMENU)NULL, hInstance, NULL);
	
	box stWindowRect; GetWindowRect(hWnd, &stWindowRect);
	box stClientRect; GetClientRect(hWnd, &stClientRect);
	int nClientWidth = stClientRect.right - stClientRect.left;
	int nClientHeight = stClientRect.bottom - stClientRect.top;
	int nWindowWidth = stWindowRect.right - stWindowRect.left;
	int nWindowHeight = stWindowRect.bottom - stWindowRect.top;

	nWindowWidth += nWindowWidth - nClientWidth;
	nWindowHeight += nWindowHeight - nClientHeight;
	int nResolutionX = GetSystemMetrics(SM_CXSCREEN);
	int nResolutionY = GetSystemMetrics(SM_CYSCREEN);
	box window = { 0,0,800,800 };
	AdjustWindowRect(&window, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(hWnd, NULL, nResolutionX / 2 - nWindowWidth / 2, nResolutionY / 2 - nWindowHeight / 2, nWindowWidth, nWindowHeight, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

typedef struct BAKCGROUND {
	int MAX_X, MAX_Y, kind;
}background;
typedef struct CHARACTER {
	int x, y, state, kind, diff;
	int speed, num_bubble, bubble_len;
}Character;
typedef struct BUBBLE {
	int x, y, len, time, on, pop;
}Bubble;
typedef struct OBJECT {
	int x, y, kind, random_block;
}Object;
typedef struct ITEM {
	int kind, on;
}Item;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, memdc;
	//HDC mem1dc, mem2dc;
	HBRUSH hBrush, oldBrush;
	static HBITMAP hBit1, hBit2, hBit3, bgBit1, bgBit2, bgBit3, itemBit;
	//HBITMAP oldBit1, oldBit2;
	static int bubble_num[2] = { 0,0 }, count1 = 0, count2 = 0, movementA = 0, movementB = 0, Bubble_move = 0, playtime = 0, Ascore = 0, Bscore = 0;
	static int x = 0, y = 0, StartAX = 0, StartAY = 0, StartBX = 0, StartBY = 0, Adead_time = 0, Bdead_time = 0, AMovingTime = 30, BMovingTime = 30;
	static int blocking_L[14], blocking_R[14], blocking_T[14], blocking_B[14];
	static Character character[2];
	static Bubble bubble[14];
	static Object obj[13][15];
	static Item item[13][15];
	int random = rand() % 4 + 1;

	switch (uMsg) {
	case WM_CREATE:
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlalog_Proc);

		PlaySound(TEXT("ca_bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		itemBit = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));

		character[0].x = 13; character[0].y = 11;
		character[1].x = 1; character[1].y = 1;
		switch (map_kind) // MAP SETTING
		{
		case 1:		// forest07
			bgBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
			for (int y = 0; y < 13;y++)
				for (int x = 0; x < 15; x++) {
					obj[y][x].x = x; obj[y][x].y = y; obj[y][x].kind = 2; obj[y][x].random_block = rand() % 3;
				}

			{	
			obj[0][0].kind = 1; obj[1][0].kind = 1; obj[4][0].kind = 1;
			obj[5][0].kind = 1; obj[8][0].kind = 1; obj[9][0].kind = 1;
			obj[11][0].kind = 1; obj[12][0].kind = 1; obj[1][1].kind = 1;
			obj[5][1].kind = 1; obj[3][1].kind = 1; obj[11][1].kind = 1;
			obj[1][5].kind = 1; obj[1][6].kind = 1; obj[2][6].kind = 1;
			obj[11][8].kind = 1; obj[11][9].kind = 1; obj[10][9].kind = 1;
			obj[7][9].kind = 1; obj[7][10].kind = 1; obj[6][10].kind = 1;
			obj[3][9].kind = 1; obj[3][10].kind = 1; obj[2][10].kind = 1; 
			obj[9][11].kind = 1; 
			}

			{
				obj[0][1].kind = 3; obj[0][3].kind = 3; obj[0][5].kind = 3; obj[0][7].kind = 3; obj[0][9].kind = 3; obj[0][14].kind = 3;
				obj[1][11].kind = 3; obj[1][12].kind = 3;
				obj[2][1].kind = 3; obj[2][3].kind = 3; obj[2][5].kind = 3; obj[2][7].kind = 3; obj[2][9].kind = 3; obj[2][14].kind = 3;
				obj[3][11].kind = 3; obj[3][12].kind = 3;
				obj[4][1].kind = 3; obj[4][3].kind = 3; obj[4][5].kind = 3; obj[4][7].kind = 3; obj[4][9].kind = 3; obj[4][14].kind = 3;
				obj[5][11].kind = 3; obj[5][12].kind = 3;
				obj[6][1].kind = 3; obj[6][3].kind = 3; obj[6][5].kind = 3; obj[6][7].kind = 3; obj[6][9].kind = 3; obj[6][14].kind = 3;
				obj[8][11].kind = 3; obj[7][12].kind = 3; obj[8][12].kind = 3; obj[9][12].kind = 3; obj[10][12].kind = 3;
				obj[8][1].kind = 3; obj[8][3].kind = 3; obj[8][5].kind = 3; obj[8][7].kind = 3; obj[8][9].kind = 3; obj[8][14].kind = 3;
				obj[10][1].kind = 3; obj[10][3].kind = 3; obj[10][6].kind = 3; obj[10][8].kind = 3; obj[10][10].kind = 3;
				obj[12][1].kind = 3; obj[12][3].kind = 3; obj[12][6].kind = 3; obj[12][8].kind = 3; obj[12][10].kind = 3; obj[11][4].kind = 3;
				obj[6][14].kind = 3; obj[7][14].kind = 3; obj[8][14].kind = 3; obj[9][14].kind = 3; obj[10][14].kind = 3;
			}

			StartAX = 1, StartAY = 11, StartBX = 10, StartBY = 3;
			character[0].x = StartAX; character[0].y = StartAY;
			character[1].x = StartBX; character[1].y = StartBY;

			break;
		case 2:		// village10
			bgBit2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
			for (int y = 0; y < 13; y++)
				for (int x = 0; x < 15; x++) {
					obj[y][x].x = x; obj[y][x].y = y; obj[y][x].kind = 2; obj[y][x].random_block = rand() % 3;
				}

			{
				obj[0][0].kind = 1; obj[1][0].kind = 1; obj[2][0].kind = 1; obj[2][1].kind = 1;
				obj[0][13].kind = 1; obj[1][12].kind = 1; obj[1][13].kind = 1; obj[1][14].kind = 1;
				obj[0][6].kind = 1; obj[0][7].kind = 1; obj[1][7].kind = 1; obj[1][8].kind = 1;
				obj[2][6].kind = 1;
				obj[3][7].kind = 1; obj[3][8].kind = 1; obj[4][6].kind = 1; obj[4][7].kind = 1;
				obj[5][8].kind = 1; obj[5][9].kind = 1;
				obj[6][6].kind = 1; obj[6][7].kind = 1; obj[7][7].kind = 1; obj[7][8].kind = 1;
				obj[7][5].kind = 1; obj[8][6].kind = 1;
				obj[9][8].kind = 1; obj[9][7].kind = 1; obj[10][6].kind = 1; obj[10][7].kind = 1;
				obj[11][8].kind = 1;
				obj[12][6].kind = 1; obj[12][7].kind = 1;
				obj[11][0].kind = 1; obj[10][1].kind = 1; obj[11][1].kind = 1; obj[12][1].kind = 1;
				obj[10][14].kind = 1; obj[11][14].kind = 1; obj[12][14].kind = 1; obj[12][13].kind = 1; }

			{
				obj[1][1].kind = 3; obj[1][3].kind = 3; obj[3][1].kind = 3; obj[3][3].kind = 3; obj[5][1].kind = 3; obj[5][3].kind = 3;
				obj[7][11].kind = 3; obj[7][13].kind = 3; obj[9][11].kind = 3; obj[9][13].kind = 3; obj[11][11].kind = 3; obj[11][13].kind = 3;
				obj[8][0].kind = 4; obj[8][2].kind = 4; obj[8][4].kind = 4; obj[10][0].kind = 4; obj[10][2].kind = 4; obj[10][4].kind = 4; obj[12][0].kind = 4; obj[12][2].kind = 4; obj[12][4].kind = 4;
				obj[0][10].kind = 5; obj[2][10].kind = 5; obj[4][10].kind = 5; obj[0][12].kind = 5; obj[2][12].kind = 5; obj[4][12].kind = 5; obj[0][14].kind = 5; obj[2][14].kind = 5; obj[4][14].kind = 5;
				obj[6][0].kind = 6; obj[6][2].kind = 6; obj[6][4].kind = 6; obj[6][14].kind = 6; obj[6][12].kind = 6; obj[6][10].kind = 6;
				obj[1][5].kind = 6; obj[1][9].kind = 6; obj[3][5].kind = 6; obj[3][9].kind = 6; obj[5][5].kind = 6;
				obj[11][5].kind = 6; obj[11][9].kind = 6; obj[9][5].kind = 6; obj[9][9].kind = 6; obj[7][9].kind = 6;
			}

			StartAX = 1, StartAY = 2, StartBX = 13, StartBY = 1;
			character[0].x = StartAX; character[0].y = StartAY;
			character[1].x = StartBX; character[1].y = StartBY;

			break;
		case 3:		// patrit14
			bgBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));
			for (int y = 0; y < 13; y++)
				for (int x = 0; x < 15; x++) {
					obj[y][x].x = x; obj[y][x].y = y; obj[y][x].kind = 2; obj[y][x].random_block = rand() % 3;
				}

			{
				obj[0][4].kind = 1; obj[0][5].kind = 1; obj[0][6].kind = 1; obj[0][8].kind = 1; obj[0][9].kind = 1; obj[0][10].kind = 1;
				obj[1][3].kind = 1; obj[1][7].kind = 1; obj[1][11].kind = 1;
				obj[2][2].kind = 1; obj[2][4].kind = 1; obj[2][6].kind = 1; obj[2][8].kind = 1; obj[2][10].kind = 1; obj[2][12].kind = 1;
				obj[3][1].kind = 1; obj[4][1].kind = 1; obj[5][1].kind = 1; obj[6][1].kind = 1; 
				obj[3][13].kind = 1; obj[4][13].kind = 1; obj[5][13].kind = 1; obj[6][13].kind = 1; 
				obj[3][3].kind = 1; obj[3][11].kind = 1; obj[5][3].kind = 1; obj[5][11].kind = 1;
				obj[9][4].kind = 1; obj[9][6].kind = 1; obj[9][8].kind = 1; obj[9][10].kind = 1;
				obj[8][3].kind = 1; obj[8][11].kind = 1;
				obj[7][10].kind = 1; obj[7][10].kind = 1; obj[7][10].kind = 1; obj[7][10].kind = 1;
				obj[10][2].kind = 1; obj[11][2].kind = 1; obj[12][2].kind = 1; obj[12][1].kind = 1; 
				obj[10][12].kind = 1; obj[11][12].kind = 1; obj[12][12].kind = 1; obj[12][13].kind = 1;
				obj[10][5].kind = 1; obj[10][9].kind = 1; obj[11][6].kind = 1; obj[11][7].kind = 1; obj[11][8].kind = 1;
			}

			{
				/*obj[0][4].kind = 1; obj[0][5].kind = 1; obj[0][6].kind = 1; obj[0][8].kind = 1; obj[0][9].kind = 1; obj[0][10].kind = 1;
				obj[1][3].kind = 1; obj[1][7].kind = 1; obj[1][11].kind = 1;
				obj[2][2].kind = 1; obj[2][4].kind = 1; obj[2][6].kind = 1; obj[2][8].kind = 1; obj[2][10].kind = 1; obj[2][12].kind = 1;
				obj[3][1].kind = 1; obj[3][3].kind = 1; obj[3][11].kind = 1; obj[3][13].kind = 1;
				obj[5][2].kind = 1; obj[6][2].kind = 1; obj[7][2].kind = 1; obj[5][13].kind = 1; obj[6][13].kind = 1; obj[7][13].kind = 1;
				obj[6][3].kind = 1; obj[6][11].kind = 1;*/
				obj[1][1].kind = 3; obj[1][13].kind = 3; obj[11][1].kind = 3; obj[11][13].kind = 3;
			}

			StartAX = 2, StartAY = 12, StartBX = 12, StartBY = 12;
			character[0].x = StartAX; character[0].y = StartAY;
			character[1].x = StartBX; character[1].y = StartBY;

			break;
		default:
			break;
		}

		// character setting
		
		switch (fst_char_kind)
		{
		case 1:
			hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
			character[0].num_bubble = 1;
			character[0].speed = 1;
			character[0].bubble_len = 1;
			character[0].kind = 1;
			character[0].state = 0;
			break;
		case 2:
			hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
			character[0].num_bubble = 1;
			character[0].speed = 1;
			character[0].bubble_len = 1;
			character[0].kind = 2;
			character[0].state = 0;
			break;
		case 3:
			hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
			character[0].num_bubble = 1;
			character[0].speed = 1;
			character[0].bubble_len = 1;
			character[0].kind = 3;
			character[0].state = 0;
			break;
		case 4:
			hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
			character[0].num_bubble = 1;
			character[0].speed = 1;
			character[0].bubble_len = 1;
			character[0].kind = 4;
			character[0].state = 0;
			break;
		case 5:
		{
			switch (random)
			{
			case 5:
				hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
				character[0].num_bubble = 1;
				character[0].speed = 1;
				character[0].bubble_len = 1;
				character[0].kind = 1;
				character[0].state = 0;
				break;
			case 6:
				hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
				character[0].num_bubble = 1;
				character[0].speed = 1;
				character[0].bubble_len = 1;
				character[0].kind = 2;
				character[0].state = 0;
				break;
			case 7:
				hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
				character[0].num_bubble = 1;
				character[0].speed = 1;
				character[0].bubble_len = 1;
				character[0].kind = 3;
				character[0].state = 0;
				break;
			case 8:
				hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
				character[0].num_bubble = 1;
				character[0].speed = 1;
				character[0].bubble_len = 1;
				character[0].kind = 4;
				character[0].state = 0;
				break;
			default:
				break;
			}
		}
			break;
		default:
			break;
		}

		switch (sec_char_kind)
		{
		case 1:
			hBit2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
			character[1].num_bubble = 1;
			character[1].speed = 1;
			character[1].bubble_len = 1;
			character[1].kind = 1;
			character[1].state = 0;
			break;
		case 2:
			hBit2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
			character[1].num_bubble = 1;
			character[1].speed = 1;
			character[1].bubble_len = 1;
			character[1].kind = 2;
			character[1].state = 0;
			break;
		case 3:
			hBit2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
			character[1].num_bubble = 1;
			character[1].speed = 1;
			character[1].bubble_len = 1;
			character[1].kind = 3;
			character[1].state = 0;
			break;
		case 4:
			hBit2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
			character[1].num_bubble = 1;
			character[1].speed = 1;
			character[1].bubble_len = 1;
			character[1].kind = 4;
			character[1].state = 0;
			break;
		case 5:
		{
			random = (random + 1) % 4;
			switch (random)
			{
			case 1:
				hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
				character[1].num_bubble = 1;
				character[1].speed = 1;
				character[1].bubble_len = 1;
				character[1].kind = 1;
				character[1].state = 0;
				break;
			case 2:
				hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
				character[1].num_bubble = 1;
				character[1].speed = 1;
				character[1].bubble_len = 1;
				character[1].kind = 2;
				character[1].state = 0;
				break;
			case 3:
				hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
				character[1].num_bubble = 1;
				character[1].speed = 1;
				character[1].bubble_len = 1;
				character[1].kind = 3;
				character[1].state = 0;
				break;
			case 4:
				hBit1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
				character[1].num_bubble = 1;
				character[1].speed = 1;
				character[1].bubble_len = 1;
				character[1].kind = 4;
				character[1].state = 0;
				break;
			default:
				break;
			}
		}
		default:
			break;
		}

		AMovingTime = 10 - character[0].speed; 
		BMovingTime = 10 - character[1].speed;

		playtime = 1800;
		SetTimer(hWnd, 1, 100, NULL);

		for (int i = 0; i < 14; i++)
		{
			bubble[i].on = 0; bubble[i].pop = 0;
			bubble[i].time = 0;
		}
		break;
	case WM_TIMER:
	{
		int pop_bubble = 0;

		box CharA = { character[0].x, character[0].y, character[0].x + 1, character[0].y + 1 };
		box CharB = { character[1].x, character[1].y, character[1].x + 1, character[1].y + 1 };

		// characters move
		if (AMovingTime != 0) --AMovingTime;
		if (movementA == 1) {
			if (AMovingTime == 0) {
				switch (character[0].diff)
				{
				case 1:
					character[0].y -= 1;
					break;
				case 2:
					character[0].y += 1;
					break;
				case 3:
					character[0].x -= 1;
					break;
				case 4:
					character[0].x += 1;
					break;
				default:
					break;
				}
				AMovingTime = 10 - character[0].speed;
			}
		}
		if (BMovingTime != 0) --BMovingTime;
		if (movementB == 1) {
			if (BMovingTime == 0) {
				switch (character[1].diff)
				{
				case 1:
					character[1].y -= 1;
					break;
				case 2:
					character[1].y += 1;
					break;
				case 3:
					character[1].x -= 1;
					break;
				case 4:
					character[1].x += 1;
					break;
				default:
					break;
				}
				BMovingTime = 10 - character[1].speed;
			}
		}

		for (int i = 0; i < 14; i++) // bubble setting
		{
			if (bubble[i].pop == 1) {
				hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
				bubble[i].pop = 0;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			if (bubble[i].on == 1) {
				hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
				bubble[i].time = (bubble[i].time + 1);
				Bubble_move = (Bubble_move + 1) % 4;
				if (bubble[i].time % 20 == 0) {
					if (i < 6) { count1 -= 1; bubble_num[0] -= 1; }
					else { count2 -= 1; bubble_num[1] -= 1; }
					
					pop_bubble = i; bubble[i].on = 0; bubble[i].pop = 1; bubble[i].time = 0;

					//collision
					for (int l = 0; l <= bubble[pop_bubble].len; l++) {
						if (obj[bubble[pop_bubble].y][bubble[pop_bubble].x + l].kind != 1) {
							blocking_R[pop_bubble] = bubble[pop_bubble].len - l; break;
						}
					}
					for (int l = 0; l <= bubble[pop_bubble].len; l++) {
						if (obj[bubble[pop_bubble].y][bubble[pop_bubble].x - l].kind != 1) {
							blocking_L[pop_bubble] = bubble[pop_bubble].len - l; break;
						}
					}
					for (int l = 0; l <= bubble[pop_bubble].len; l++) {
						if (obj[bubble[pop_bubble].y + l][bubble[pop_bubble].x].kind != 1) {
							blocking_B[pop_bubble] = bubble[pop_bubble].len - l; break;
						}
					}
					for (int l = 0; l <= bubble[pop_bubble].len; l++) {
						if (obj[bubble[pop_bubble].y - l][bubble[pop_bubble].x].kind != 1) {
							blocking_T[pop_bubble] = bubble[pop_bubble].len - l; break;
						}
					}
					box B_1 = { bubble[pop_bubble].x + 0, bubble[pop_bubble].y + 0, bubble[pop_bubble].x + 1, bubble[pop_bubble].y + 1 };							// Center
					box B_2 = { bubble[pop_bubble].x + 1, bubble[pop_bubble].y + 0, bubble[pop_bubble].x + 1 + (bubble[pop_bubble].len - blocking_R[pop_bubble]), bubble[pop_bubble].y + 1};		// Right
					box B_3 = { bubble[pop_bubble].x + 0 - (bubble[pop_bubble].len - blocking_L[pop_bubble]), bubble[pop_bubble].y + 0, bubble[pop_bubble].x + 0, bubble[pop_bubble].y + 1};		// Left
					box B_4 = { bubble[pop_bubble].x + 0, bubble[pop_bubble].y + 0 - (bubble[pop_bubble].len - blocking_T[pop_bubble]), bubble[pop_bubble].x + 1, bubble[pop_bubble].y + 0};		// Top
					box B_5 = { bubble[pop_bubble].x + 0, bubble[pop_bubble].y + 1, bubble[pop_bubble].x + 1, bubble[pop_bubble].y + 1 + (bubble[pop_bubble].len - blocking_B[pop_bubble])};		// Bottom

					if (collision(CharA, B_1) == 1 || collision(CharA, B_2) == 1 || collision(CharA, B_3) == 1 || collision(CharA, B_4) == 1 || collision(CharA, B_5) == 1) {
						character[0].state = 1; ++Bscore;
					}
					if (collision(CharB, B_1) == 1 || collision(CharB, B_2) == 1 || collision(CharB, B_3) == 1 || collision(CharB, B_4) == 1 || collision(CharB, B_5) == 1) {
						character[1].state = 1; ++Ascore;
					}

					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
		}

		for (int y = 0; y < 14; y++) // item setting
			for (int x = 0; x < 12; x++) {
				if (item[y][x].on == 1) {
					box ITEM = { x,y,x + 1,y + 1 };
					if (collision(CharA, ITEM) == 1) {
						switch (item[y][x].kind)
						{
						case 0:
							if (character[0].speed < 8) character[0].speed = (character[0].speed + 2);
							item[y][x].on = 0;
							break;
						case 1:
							if (character[0].bubble_len < 5)
								character[0].bubble_len = (character[0].bubble_len + 1);
							item[y][x].on = 0;
							break;
						case 2:
							if (character[0].num_bubble < 8)
								character[0].num_bubble = (character[0].num_bubble + 1);
							item[y][x].on = 0;
							break;
						case 3:
							character[0].bubble_len = 8;
							item[y][x].on = 0;
							break;
						case 4:
							item[y][x].on = 0;
							break;
						default:
							break;
						}
					}
					else if (collision(CharB, ITEM) == 1) {
						switch (item[y][x].kind)
						{
						case 0:
							if (character[1].speed < 8) character[1].speed = (character[0].speed + 2);
							item[y][x].on = 0;
							break;
						case 1:
							if (character[1].bubble_len < 5)
								character[1].bubble_len = (character[0].bubble_len + 1);
							item[y][x].on = 0;
							break;
						case 2:
							if (character[1].num_bubble < 8)
								character[1].num_bubble = (character[1].num_bubble + 1);
							item[y][x].on = 0;
							break;
						case 3:
							character[1].bubble_len = 8;
							item[y][x].on = 0;
							break;
						case 4:
							item[y][x].on = 0;
							break;
						default:
							break;
						}
					}
				}
			}
		
		if (character[0].state == 1) {
			if (++Adead_time % 30 == 0) {
				character[0].state = 0; Adead_time = 0;
				character[0].x = StartAX; character[0].y = StartAY;
			}
		}
		if (character[1].state == 1) {
			if (++Bdead_time % 30 == 0) {
				character[1].state = 0; Bdead_time = 0;
				character[1].x = StartBX; character[1].y = StartBY;
			}
		}

		
		if (--playtime == 0) {
			if (Ascore > Bscore)
				MessageBox(hWnd, L"Player 1 WIN", L"GameResult", MB_OK);
			if (Ascore < Bscore)
				MessageBox(hWnd, L"Player 2 WIN", L"GameResult", MB_OK);
			if (Ascore == Bscore)
				MessageBox(hWnd, L"DRAW", L"GameResult", MB_OK);
		}
	}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYDOWN: // MOVE: wasd // BUBBLE: f  0
		switch (wParam)
		{
		box CharA = { character[0].x, character[0].y, character[0].x + 1, character[0].y + 1 };
		box CharB = { character[1].x, character[1].y, character[1].x + 1, character[1].y + 1 };
			

		case 'a':
		case 'A':
			if ((character[0].x) * 50 > 0) {
				if (obj[character[0].y][character[0].x - 1].kind == 1) {
					//character[0].x -= 1;// character[0].speed;
					character[0].diff = 3;
					movementA = 1; //AMovingTime = 30 - character[0].speed;
				}
			}
			break;
		case 'D':
		case 'd':
			if ((character[0].x + 1) * 50 < window_size_w) {
				if (obj[character[0].y][character[0].x + 1].kind == 1) {
					//character[0].x += 1;//character[0].speed; 
					character[0].diff = 4;
					movementA = 1; //AMovingTime = 30 - character[0].speed;
				}
			}
			break;
		case 'W':
		case 'w':
			if ((character[0].y) * 50 > 0) {
				if (obj[character[0].y - 1][character[0].x].kind == 1) {
					character[0].diff = 1;
					movementA = 1;
				}
			}
			break;
		case 'S':
		case 's':
			if ((character[0].y + 1) * 50 < window_size_d) {
				if (obj[character[0].y + 1][character[0].x].kind == 1) {
					character[0].diff = 2;
					movementA = 1;
				}
			}
			break;


		case VK_RIGHT:
			if ((character[1].x + 1) * 50 < window_size_w) {
				if (obj[character[1].y][character[1].x + 1].kind == 1) {
					character[1].diff = 4;
					movementB = 1;
				}
			}
			break;
		case VK_UP:
			if ((character[1].y) * 50 > 0) {
				if (obj[character[1].y - 1][character[1].x].kind == 1) {
					character[1].diff = 1;
					movementB = 1;
				}
			}
			break;
		case VK_DOWN:
			if ((character[1].y + 1) * 50 < window_size_d) {
				if (obj[character[1].y + 1][character[1].x].kind == 1) {
					character[1].diff = 2;
					movementB = 1;
				}
			}
			break;
		case VK_LEFT:
			if ((character[1].x) * 50 > 0) {
				if (obj[character[1].y][character[1].x - 1].kind == 1) {
					character[1].diff = 3;
					movementB = 1;
				}
			}
			break;


		case 'f':
		case 'F':
			if (character[0].num_bubble > count1 && character[0].state == 0 && bubble_num[0] < character[0].num_bubble) {
				bubble[count1].x = character[0].x; bubble[count1].y = character[0].y;
				bubble[count1].time = 0; bubble[count1].on = 1; bubble[count1].len = character[0].bubble_len;
				count1++;// = (count1 + 1) % (character[0].num_bubble);
				bubble_num[0]++;// += 1;
			}
			break;
		case 0x60:
			if (character[1].num_bubble > count2 && character[1].state == 0 && bubble_num[1] < character[1].num_bubble) {
				bubble[6 + count2].x = character[1].x; bubble[6 + count2].y = character[1].y;
				bubble[6 + count2].time = 0; bubble[6 + count2].on = 1; bubble[6 + count2].len = character[1].bubble_len;
				count2++;// = (count2 + 1) % (character[1].num_bubble);
				bubble_num[1]++;// += 1;
			}
			break;


		case VK_ESCAPE:
			exit(1);
			break;

		default:
			break;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_KEYUP:
		movementA = 0; movementB = 0;
		AMovingTime = 0; BMovingTime = 0;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBit1);

		// BG_DRAW
		for(int y = 0; y < 13; y++)
			for (int x = 0; x < 15; x++)
			{
				if (map_kind == 1) {
					SelectObject(memdc, bgBit1);
					if (obj[y][x].kind == 1) {
						StretchBlt(hdc, x * 50, y * 50, 50, 50, memdc, 68 * 3, 68 * 3, 68, 68, SRCCOPY);
					}
					if (obj[y][x].kind == 2) {
						StretchBlt(hdc, x * 50, y * 50, 50, 50, memdc, 68 * 0, 68 * obj[y][x].random_block, 68, 68, SRCCOPY);
					}
					if (obj[y][x].kind == 3) {
						StretchBlt(hdc, x * 50, y * 50, 50, 50, memdc, 68 * obj[y][x].random_block, 68 * 3, 68, 68, SRCCOPY);
					}
				}
				if (map_kind == 2) {
					SelectObject(memdc, bgBit2);
					if (obj[y][x].kind == 1) {
						StretchBlt(hdc, x * 50, y * 50, 50, 50, memdc, 68 * 0, 68 * 3, 68, 68, SRCCOPY);
					}
					if (obj[y][x].kind == 2) {
						StretchBlt(hdc, x * 50, y * 50, 50, 50, memdc, 68 * 0, 68 * obj[y][x].random_block, 68, 68, SRCCOPY);
					}
					if (obj[y][x].kind == 3) {
						StretchBlt(hdc, x * 50, (y * 50) - 25, 50, 75, memdc, 68 * 0, (68 * 4)+41, 68, 95, SRCCOPY);
					}
					if (obj[y][x].kind == 4) {
						StretchBlt(hdc, x * 50, (y * 50) - 25, 50, 75, memdc, 68 * 1, (68 * 4)+41, 68, 95, SRCCOPY);
					}
					if (obj[y][x].kind == 5) {
						StretchBlt(hdc, x * 50, (y * 50) - 25, 50, 75, memdc, 68 * 2, (68 * 4)+41, 68, 95, SRCCOPY);
					}
					if (obj[y][x].kind == 6) {
						StretchBlt(hdc, x * 50, (y * 50) - 25, 50, 75, memdc, 68 * 3, (68 * 4)+32, 68, 104, SRCCOPY);
					}
				}
				if (map_kind == 3) {
					SelectObject(memdc, bgBit3);
					if (obj[y][x].kind == 1) {
						StretchBlt(hdc, x * 50, y * 50, 50, 50, memdc, 68 * 0, 68 * 3, 68, 68, SRCCOPY);
					}
					if (obj[y][x].kind == 2) {
						StretchBlt(hdc, x * 50, y * 50, 50, 50, memdc, 68 * 0, 68 * obj[y][x].random_block, 68, 68, SRCCOPY);
					}
					if (obj[y][x].kind == 3) {
						StretchBlt(hdc, x * 50, y * 50, 50, 50, memdc, 68 * 2, 68 * 3, 68, 68, SRCCOPY);
					}
				}
			}

		// ITEM_DRAW
		for (int y = 0; y < 12; y++)
			for (int x = 0; x < 14; x++)
				if (item[y][x].on == 1 && item[y][x].kind < 6) {
					SelectObject(memdc, itemBit);
					StretchBlt(hdc, x * 50, y * 50, 50, 50, memdc, 40 * item[y][x].kind, 0, 40, 50, SRCCOPY);
				}

		// BUBBLE_DRAW
		for (int i = 0; i < character[0].num_bubble; i++)
		{
			if (bubble[i].on == 1) {
				SelectObject(memdc, hBit3);
				StretchBlt(hdc, bubble[i].x * 50, bubble[i].y * 50, 50, 50, memdc, 37 * Bubble_move, 0, 37, 50, SRCCOPY);
			}
		}
		for (int i = 0; i < character[1].num_bubble; i++)
		{
			if (bubble[6 + i].on == 1) {
				SelectObject(memdc, hBit3);
				StretchBlt(hdc, bubble[6 + i].x * 50, bubble[6 + i].y * 50, 50, 50, memdc, 37 * Bubble_move, 0, 37, 50, SRCCOPY);
			}
		}

		for (int i = 0; i < 14; i++) {
			if (bubble[i].pop == 1) {
				for (int l = 0; l <= bubble[i].len; l++) {
					if (obj[bubble[i].y][bubble[i].x + l].kind != 1) {
						blocking_R[i] = bubble[i].len - l; break;
					}
				}
				for (int l = 0; l <= bubble[i].len; l++) {
					if (obj[bubble[i].y][bubble[i].x - l].kind != 1) {
						blocking_L[i] = bubble[i].len - l; break;
					}
				}
				for (int l = 0; l <= bubble[i].len; l++) {
					if (obj[bubble[i].y + l][bubble[i].x].kind != 1) {
						blocking_B[i] = bubble[i].len - l; break;
					}
				}
				for (int l = 0; l <= bubble[i].len; l++) {
					if (obj[bubble[i].y - l][bubble[i].x].kind != 1) {
						blocking_T[i] = bubble[i].len - l; break;
					}
				}
			}
		}

		for (int i = 0; i < character[0].num_bubble; i++)
		{
			if (bubble[i].pop == 1)
			{
				hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
				SelectObject(memdc, hBit3);
				StretchBlt(hdc, (bubble[i].x + 0) * 50, (bubble[i].y + 0) * 50, 50, 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
				//Right
				if (obj[bubble[i].y][bubble[i].x + 1].kind == 1) {
					hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
					SelectObject(memdc, hBit3);
					StretchBlt(hdc, (bubble[i].x + 1) * 50, (bubble[i].y + 0) * 50, (character[0].bubble_len-blocking_R[i]) * 50, 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
					blocking_R[i] = 0;
				}
				else if (obj[bubble[i].y][bubble[i].x + 1].kind == 2) {
					obj[bubble[i].y][bubble[i].x + 1].kind = 1;
					item[bubble[i].y][bubble[i].x + 1].kind = (rand() % 51) / 10;
					if (item[bubble[i].y][bubble[i].x + 1].kind != 5)
						item[bubble[i].y][bubble[i].x + 1].on = 1;
				}
				//Left
				if (obj[bubble[i].y][bubble[i].x - 1].kind == 1) {
					hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
					SelectObject(memdc, hBit3);
					StretchBlt(hdc, (bubble[i].x - (character[0].bubble_len - blocking_L[i])) * 50, (bubble[i].y + 0) * 50, (character[0].bubble_len-blocking_L[i]) * 50, 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
					blocking_L[i] = 0;
				}
				else if (obj[bubble[i].y][bubble[i].x - 1].kind == 2) {
					obj[bubble[i].y][bubble[i].x - 1].kind = 1;
					item[bubble[i].y][bubble[i].x - 1].kind = (rand() % 51) / 10;
					if (item[bubble[i].y][bubble[i].x - 1].kind != 5)
						item[bubble[i].y][bubble[i].x - 1].on = 1;
				}
				//Top
				if (obj[bubble[i].y + 1][bubble[i].x].kind == 1) {
					hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
					SelectObject(memdc, hBit3);
					StretchBlt(hdc, (bubble[i].x + 0) * 50, (bubble[i].y + 1) * 50, 50, (character[0].bubble_len-blocking_T[i]) * 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
					blocking_T[i] = 0;
				}
				else if (obj[bubble[i].y + 1][bubble[i].x].kind == 2) {
					obj[bubble[i].y + 1][bubble[i].x].kind = 1;
					item[bubble[i].y + 1][bubble[i].x].kind = (rand() % 51) / 10;
					if (item[bubble[i].y + 1][bubble[i].x].kind != 5)
						item[bubble[i].y + 1][bubble[i].x].on = 1;
				}
				//Bottom
				if (obj[bubble[i].y - 1][bubble[i].x].kind == 1) {
					hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
					SelectObject(memdc, hBit3);
					StretchBlt(hdc, (bubble[i].x + 0) * 50, (bubble[i].y - (character[0].bubble_len - blocking_B[i])) * 50, 50, (character[0].bubble_len-blocking_B[i]) * 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
					blocking_B[i] = 0;
				}
				else if (obj[bubble[i].y - 1][bubble[i].x].kind == 2) {
					obj[bubble[i].y - 1][bubble[i].x].kind = 1;
					item[bubble[i].y - 1][bubble[i].x].kind = (rand() % 51) / 10;
					if (item[bubble[i].y - 1][bubble[i].x].kind != 5)
						item[bubble[i].y - 1][bubble[i].x].on = 1;
				}
			}
		}
		for (int i = 0; i < character[1].num_bubble; i++)
		{
			if (bubble[6 + i].pop == 1)
			{
				hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
				SelectObject(memdc, hBit3);
				StretchBlt(hdc, (bubble[6 + i].x + 0) * 50, (bubble[6 + i].y + 0) * 50, 50, 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
				//Right
				if (obj[bubble[6+i].y][bubble[6+i].x+1].kind == 1) {
					hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
					SelectObject(memdc, hBit3);
					StretchBlt(hdc, (bubble[6 + i].x + 1) * 50, (bubble[6 + i].y + 0) * 50, (character[1].bubble_len-blocking_R[6 + i]) * 50, 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
					blocking_R[i] = 0;
				}
				else if (obj[bubble[6 + i].y][bubble[6 + i].x + 1].kind == 2) {
					obj[bubble[6 + i].y][bubble[6 + i].x + 1].kind = 1;
					item[bubble[6 + i].y][bubble[6 + i].x + 1].kind = (rand() % 51) / 10;
					if (item[bubble[6 + i].y][bubble[6 + i].x + 1].kind != 5)
						item[bubble[6 + i].y][bubble[6 + i].x + 1].on = 1;
				}
				//Left
				if (obj[bubble[6+i].y][bubble[6+i].x-1].kind == 1) {
					hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
					SelectObject(memdc, hBit3);
					StretchBlt(hdc, (bubble[6 + i].x - (character[1].bubble_len - blocking_L[6 + i])) * 50, (bubble[6 + i].y + 0) * 50, (character[1].bubble_len-blocking_L[6 + i]) * 50, 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
					blocking_L[i] = 0;
				}
				else if (obj[bubble[6 + i].y][bubble[6 + i].x - 1].kind == 2) {
					obj[bubble[6 + i].y][bubble[6 + i].x - 1].kind = 1;
					item[bubble[6 + i].y][bubble[6 + i].x - 1].kind = (rand() % 51) / 10;
					if (item[bubble[6 + i].y][bubble[6 + i].x - 1].kind != 5)
						item[bubble[6 + i].y][bubble[6 + i].x - 1].on = 1;
				}
				//Top
				if (obj[bubble[6+i].y+1][bubble[6+i].x].kind == 1) {
					hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
					SelectObject(memdc, hBit3);
					StretchBlt(hdc, (bubble[6 + i].x + 0) * 50, (bubble[6 + i].y + 1) * 50, 50, (character[1].bubble_len-blocking_T[6 + i]) * 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
					blocking_T[i] = 0;
				}
				else if (obj[bubble[6 + i].y + 1][bubble[6 + i].x].kind == 2) {
					obj[bubble[6 + i].y + 1][bubble[6 + i].x].kind = 1;
					item[bubble[6 + i].y + 1][bubble[6 + i].x].kind = (rand() % 51) / 10;
					if (item[bubble[6 + i].y + 1][bubble[6 + i].x].kind != 5)
						item[bubble[6 + i].y + 1][bubble[6 + i].x].on = 1;
				}
				//Bottom
				if (obj[bubble[6+i].y-1][bubble[6+i].x].kind == 1) {
					hBit3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
					SelectObject(memdc, hBit3);
					StretchBlt(hdc, (bubble[6 + i].x + 0) * 50, (bubble[6 + i].y - (character[1].bubble_len - blocking_B[6 + i])) * 50, 50, (character[1].bubble_len-blocking_B[6 + i]) * 50, memdc, 40 * Bubble_move, 0, 40, 40, SRCCOPY);
					blocking_B[i] = 0;
				}
				else if (obj[bubble[6 + i].y - 1][bubble[6 + i].x].kind == 2) {
					obj[bubble[6 + i].y - 1][bubble[6 + i].x].kind = 1;	
					item[bubble[6 + i].y - 1][bubble[6 + i].x].kind = (rand() % 51) / 10;
					if (item[bubble[6 + i].y - 1][bubble[6 + i].x].kind != 5)
						item[bubble[6 + i].y - 1][bubble[6 + i].x].on = 1;
				}
			}
		}

		// CHARACTER_DRAW
		if (character[0].state == 0) {
			SelectObject(memdc, hBit1);
			StretchBlt(hdc, (character[0].x * 50), character[0].y * 50, 50, 50, memdc, 50 * (AMovingTime % 4), 60 * character[0].diff, 50, 60, SRCCOPY); 
			//if (movementA == 1) {
			//	//AMovingTime = 30 - character[0].speed;
			//}
		}

		if (character[1].state == 0) {
			SelectObject(memdc, hBit2);
			StretchBlt(hdc, (character[1].x * 50), character[1].y * 50, 50, 50, memdc, 50 * (BMovingTime % 4), 60 * character[1].diff, 50, 60, SRCCOPY); 
			//if (movementB == 1) {
			//	//BMovingTime = 10 - character[1].speed;
			//}
		}

		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PlaySound(NULL, NULL, NULL);
		DeleteObject(hBit1);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK Dlalog_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int radio;
	int id = LOWORD(wParam), event = HIWORD(wParam);
	char str[128];
	DWORD state;

	switch (iMsg) {
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
		CheckRadioButton(hDlg, IDC_RADIO4, IDC_RADIO8, IDC_RADIO4);
		CheckRadioButton(hDlg, IDC_RADIO9, IDC_RADIO13, IDC_RADIO9);
		break;
	case WM_COMMAND:
		id = LOWORD(wParam); event = HIWORD(wParam);

		switch (LOWORD(wParam)) {
		case IDC_RADIO1:
			map_kind = 1;
			break;
		case IDC_RADIO2:
			map_kind = 2;
			break;
		case IDC_RADIO3:
			map_kind = 3;
			break;

		case IDC_RADIO4:
			fst_char_kind = 1;
			break;
		case IDC_RADIO5:
			fst_char_kind = 2;
			break;
		case IDC_RADIO6:
			fst_char_kind = 3;
			break;
		case IDC_RADIO7:
			fst_char_kind = 4;
			break;
		case IDC_RADIO8:
			fst_char_kind = 5;
			break;

		case IDC_RADIO9:
			sec_char_kind = 1;
			break;
		case IDC_RADIO10:
			sec_char_kind = 2;
			break;
		case IDC_RADIO11:
			sec_char_kind = 3;
			break;
		case IDC_RADIO12:
			sec_char_kind = 4;
			break;
		case IDC_RADIO13:
			sec_char_kind = 5;
			break;

		case IDOK:
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	return 0;
}