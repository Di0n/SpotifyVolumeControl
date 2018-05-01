#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <gdiplus.h>
#include "AudioSession.h"
#include "volumebar.h"
#include "utils.h"
#pragma comment(lib, "gdiplus.lib")

#define APP_UNIQUE_NAME "com_anveon_apps_svc"
#define SZ_WINDOW_CLASS "Spotify Volume Control"
#define SZ_TITLE		"Volume bar"

#define VOLUME_SHOW_TIME 3000
#define POSITION_X 50
#define POSITION_Y 50
#define WIDTH 66
#define HEIGHT 144
#define VOLUME_WIDTH 20
#define VOLUME_HEIGHT 84
#define VOLUME_UP_ID 1
#define VOLUME_DOWN_ID 2
#define TIMER_ID 1


using namespace Gdiplus;
using std::string;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK ShowTimer(HWND hwnd, UINT msg, UINT_PTR idEvent, DWORD dwTime);
void OnHotKeyPressed(WORD keyID);
void OnPaint(HWND hwnd);

//static CONST TCHAR APP_UNIQUE_NAME[] = _T("com_anveon_apps_svc");
//static CONST TCHAR szWindowClass[] = _T("Spotify Volume Control");
//static CONST TCHAR szTitle[] = _T("Volume bar");

UINT_PTR		timerPtr;
ULONG_PTR		gdiplusToken;
HINSTANCE		instance;
HWND			handle;
HANDLE			mutex;
AudioSession	audioSession;


BOOL GetMessage(LPMSG msg, HWND hwnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT timeOut)
{
	UINT_PTR timerID = NULL;
	if (timeOut > 0)
		timerID = SetTimer(NULL, NULL, timeOut, NULL);
	BOOL result = (timeOut > 0 && timerID == NULL) ? TRUE : GetMessage(msg, hwnd, wMsgFilterMin, wMsgFilterMax); // Als de timer niet is aangemaakt Wacht dan niet op message
	if (timerID != NULL) KillTimer(NULL, timerID);
	return result;
}
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpArg, int nCmdShow)
{
	mutex = CreateMutex(NULL, TRUE, APP_UNIQUE_NAME);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, _T("There is already an instance running of Spotify Volume Control."), SZ_WINDOW_CLASS, MB_OK | MB_ICONINFORMATION);
		std::cerr << "There is already an instance running of Spotify Volume Control.\n";
		return 0;
	}

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	Gdiplus::GdiplusStartupInput input = { 0 };
	GdiplusStartup(&gdiplusToken, &input, NULL);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(BLACK_PEN + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = SZ_WINDOW_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("RegisterClassEx failed."), SZ_WINDOW_CLASS, MB_OK | MB_ICONERROR);
		std::cerr << "RegisterClassEx failed.\n";
		return 1;
	}

	instance = hInstance;

	handle = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_COMPOSITED | WS_EX_NOACTIVATE, SZ_WINDOW_CLASS, SZ_TITLE, WS_POPUP,
		POSITION_X, POSITION_Y, WIDTH, HEIGHT,
		NULL, NULL, hInstance, NULL);
	if (!handle)
	{
		MessageBox(NULL, _T("Createwindow failed."), SZ_WINDOW_CLASS, MB_OK | MB_ICONERROR);
		std::cerr << "Createwindow failed.\n";
		return 1;
	}

	SetWindowLong(handle, GWL_STYLE, 0);
	UpdateWindow(handle);

	HRESULT hr = audioSession.CreateSession();
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("Error finding running Spotify instance."), SZ_WINDOW_CLASS, MB_OK | MB_ICONERROR);
		std::cerr << "Error finding running Spotify instance.\n";
		return 1;
	}

	ModifierKey modKey = MOD_ALT;
	Key volumeUpKey = VK_UP;
	Key volumeDownKey = VK_DOWN;

	if (!RegisterHotKey(handle, VOLUME_UP_ID, modKey, volumeUpKey))
	{
		std::cerr << "Failed to register hotkey.\n";
		return 1;
	}
	if (!RegisterHotKey(handle, VOLUME_DOWN_ID, modKey, volumeDownKey))
	{
		std::cerr << "Failed to register hotkey.\n";
		return 1;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_HOTKEY:
		OnHotKeyPressed(LOWORD(wParam));
		break;
	case WM_PAINT:
		OnPaint(hwnd);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		UnregisterHotKey(handle, VOLUME_UP_ID);
		UnregisterHotKey(handle, VOLUME_DOWN_ID);
		GdiplusShutdown(gdiplusToken);
		ReleaseMutex(mutex);
		CloseHandle(mutex);
		if (timerPtr != NULL)
			KillTimer(handle, timerPtr);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

void OnHotKeyPressed(WORD keyID)
{
	if (keyID == VOLUME_UP_ID)
	{
		// Volume omhoog
		const float vol = audioSession.GetVolume();
		if (vol == -1) return;
		if (vol < 100)
			audioSession.SetVolume(vol + 2);
	}
	else if (keyID == VOLUME_DOWN_ID)
	{
		// Volume omlaag
		const float vol = audioSession.GetVolume();
		if (vol == -1) return;
		if (vol > 0)
			audioSession.SetVolume(vol - 2);
	}
	else return;

	if (Utils::IsFullscreenMaximized(handle)) return;
	if (!IsWindowVisible(handle))
		ShowWindow(handle, SW_SHOWNA);

	RedrawWindow(handle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_INTERNALPAINT);
	timerPtr = SetTimer(handle, TIMER_ID, VOLUME_SHOW_TIME, static_cast<TIMERPROC>(ShowTimer));
}

void OnPaint(HWND hwnd)
{
	if (!IsWindowVisible(handle)) return;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromHDC(hdc);

	const int vol = audioSession.GetVolume();
	if (vol != -1)
		VolumeBar::DrawVolume(graphics, vol);
	EndPaint(hwnd, &ps);
}

VOID CALLBACK ShowTimer(HWND hwnd, UINT msg, UINT_PTR idEvent, DWORD dwTime)
{
	if (idEvent != TIMER_ID) return;
	if (!IsWindowVisible(handle)) return;
	ShowWindow(handle, SW_HIDE);
	KillTimer(handle, idEvent);
}


