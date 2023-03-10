#include <Windows.h>
#include <CommCtrl.h>
#include <process.h>

#include "Tank.h"
#include "Wall.h"
#include "Shell.h"
#include "Logic.h"
#include "resource.h"

#define KEY_W 87
#define KEY_D 68
#define KEY_S 83
#define KEY_A 65
#define KEY_R 82
#define KEY_SPACE 32
#define FrameTime 33
#define GameSpeed 50
#define ReloadTime 125
#define BUTTON_REST 1

// ????? ???????? ????
const wchar_t* const GameZonelassClass = L"???????";

HDC      hdcBack;
HBITMAP  hbmBack;
RECT     rcClient,rcText;
HANDLE	 thGame, thDraw, thReload;
HWND	 hwndGameWindow,hmndProgressBar,hwndRest;
HFONT    hfText;

char T0=0, T1=0;
char F0=1, F1=1, F2=1;

void InitializeBackBuffer(HWND hWnd, int w, int h)
{
	HDC hdcWindow;

	hdcWindow = GetDC(hWnd);

	hdcBack = CreateCompatibleDC(hdcWindow);
	hbmBack = CreateCompatibleBitmap(hdcWindow, w, h);
	SaveDC(hdcBack);
	SelectObject(hdcBack, hbmBack);

	ReleaseDC(hWnd, hdcWindow);
}

void FinalizeBackBuffer()
{
	if (hdcBack)
	{
		RestoreDC(hdcBack, -1);
		DeleteObject(hbmBack);
		DeleteDC(hdcBack);
		hdcBack = 0;
	}
}

void  Draw() {
	while (F0)
	{
		Sleep(FrameTime);
		InvalidateRect(hwndGameWindow, NULL, 0);
	}
	F0 = 1;
}

void  Game() {
	while (F1)
	{
		T1 = 1;
		while (T0)
		{
			Sleep(10);
		}
		CheckingCollisionsShell();
		T1 = 0;
		AI();
		CheckGame();
		Sleep(GameSpeed);
	}
	F1 = 1;
}

void  Reload() {
	while (F2)
	{
		TankReload();
		if (0 != tank[PLAYER].timeToReload) {
			SendMessage(hmndProgressBar, PBM_STEPIT, 0, 0);
			Sleep(ReloadTime);
			SendMessage(hmndProgressBar, PBM_STEPIT, 0, 0);
			Sleep(ReloadTime);
			SendMessage(hmndProgressBar, PBM_STEPIT, 0, 0);
			Sleep(ReloadTime);
			SendMessage(hmndProgressBar, PBM_STEPIT, 0, 0);
			Sleep(ReloadTime);
			SendMessage(hmndProgressBar, PBM_STEPIT, 0, 0);
			Sleep(ReloadTime);
			SendMessage(hmndProgressBar, PBM_STEPIT, 0, 0);
			Sleep(ReloadTime);
			SendMessage(hmndProgressBar, PBM_STEPIT, 0, 0);
			Sleep(ReloadTime);
			SendMessage(hmndProgressBar, PBM_STEPIT, 0, 0);
			Sleep(ReloadTime);
		}
		else {
			Sleep(ReloadTime * 8);
		}
	}
	F2 = 1;
}

// ??????? ?????????
LRESULT CALLBACK GameZoneProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	// ????????? ?????????
	switch (uMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rcClient);
		InitializeBackBuffer(hWnd, rcClient.right, rcClient.bottom);
		SetRect(&rcText, 500, 400, 1100, 500);
		break;

	case WM_SIZE:
		GetClientRect(hWnd, &rcClient);
		FinalizeBackBuffer();
		InitializeBackBuffer(hWnd, rcClient.right, rcClient.bottom);
		break;

	case WM_COMMAND:
		if (BUTTON_REST == LOWORD(wParam)) {       
			FinalizeTank();
			FinalizeShell();
			FinalizeWall();

			game = 1;

			InitializationWall();
			InitializationShell();
			InitializationTank(5);
			InitializationAI();

			SetFocus(hwndGameWindow);
		}
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case KEY_W:
			MoveAndCheckingCollisionsTank(UP, PLAYER);
			break;
		case KEY_D:
			MoveAndCheckingCollisionsTank(RIGHT, PLAYER);
			break;
		case KEY_S:
			MoveAndCheckingCollisionsTank(DOWN, PLAYER);
			break;
		case KEY_A:
			MoveAndCheckingCollisionsTank(LEFT, PLAYER);
			break;
		case KEY_SPACE:
			Shot(PLAYER);
			break;
		case KEY_R:
			SendMessage(hwndGameWindow, WM_COMMAND, BUTTON_REST, 0);
			break;
		}
		break;

	// ??????????
	case WM_PAINT:
		Rectangle(hdcBack, ZONE_LEFT, ZONE_TOP, ZONE_RIGHT + 60, ZONE_BOTTOM + 60);
		if (game && 0!=tank[PLAYER].hp) {
			DrawTank(hdcBack);

			T0 = 1;
			while (T1) {
				Sleep(10);
			}
			DrawShell(hdcBack);
			T0 = 0;

			DrawWall(hdcBack);
		}
		else {
			if (0==tank[PLAYER].hp) {
				SaveDC(hdcBack);
				SetTextColor(hdcBack,RGB(255,0,0));
				SelectObject(hdcBack, hfText);
				DrawText(hdcBack, L"YOU DIED", 8, &rcText, DT_CENTER);
				ReleaseDC(hdcBack, -1);
			}
			else {
				SaveDC(hdcBack);
				SetTextColor(hdcBack, RGB(0,255,0));
				SelectObject(hdcBack, hfText);
				DrawText(hdcBack, L"YOU WIN", 7, &rcText, DT_CENTER);
				ReleaseDC(hdcBack, -1);
			}
		}

		Rectangle(hdcBack, 0, 0, ZONE_LEFT, ZONE_BOTTOM + 60);
		Rectangle(hdcBack, ZONE_RIGHT + 60, ZONE_TOP, 1600, 900);		

		DrawAllTank(hdcBack);

		hdc = BeginPaint(hWnd, &ps);

		BitBlt(hdc, 0, 0, rcClient.right, rcClient.bottom, hdcBack, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);

		break;

	case WM_DESTROY:
		F0 = 0;
		F1 = 0;
		F2 = 0;
		while (!F0 || !F1 || !F2) {
			Sleep(500);
		}
		FinalizeBackBuffer();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	WNDCLASSEX wcex;
	
	// ??????????? ??????? ??????
	memset(&wcex, 0, sizeof wcex);
	wcex.cbSize = sizeof wcex;
	wcex.style = CS_GLOBALCLASS;
	wcex.lpfnWndProc = GameZoneProc;
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));

	// ???????? ????!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
	wcex.lpszClassName = GameZonelassClass;
	RegisterClassEx(&wcex);

	InitializationWall();
	InitializationShell();
	InitializationTank(5);
	InitializationAI();

	hfText = CreateFont(100,0,0,FW_BOLD,0,0,0,0,0,0,0,0,0,0);

	thDraw = (HANDLE)_beginthread(Draw, 0, 0);
	thGame = (HANDLE)_beginthread(Game, 0, 0);
	thReload = (HANDLE)_beginthread(Reload, 0, 0);

	// ???????? ????
	hwndGameWindow = CreateWindowEx(0, GameZonelassClass, GameZonelassClass, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
									0, 0, 1600, 940, 0, 0, 0, NULL);

	hmndProgressBar = CreateWindowEx(PBS_SMOOTH, PROGRESS_CLASS, L"Reload", WS_CHILD | WS_VISIBLE,
									 ZONE_RIGHT+85, 10, 280, 40, hwndGameWindow, 0, 0, NULL);

	hwndRest = CreateWindowEx(BS_PUSHBUTTON, L"BUTTON", L"??????????",WS_VISIBLE | WS_CHILD, ZONE_RIGHT + 150, ZONE_BOTTOM - 100, 200, 40, hwndGameWindow, (HMENU)BUTTON_REST, 0, NULL);

	SendMessage(hmndProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 8));
	SendMessage(hmndProgressBar, PBM_SETSTEP, 1, 0);
	SendMessage(hmndProgressBar, PBM_DELTAPOS, 8, 0);

	while (GetMessage(&msg, 0, 0, 0))
	{
		DispatchMessage(&msg);
	}

	DeleteObject(hfText);
	FinalizeTank();
	FinalizeShell();
	FinalizeWall();

	// ????? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return msg.wParam;
}