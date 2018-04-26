#include "AudioSession.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <gdiplus.h>
#include "keyboard.h"
#include "volumebar.h"
#pragma comment(lib, "gdiplus.lib")

#define WIDTH 66
#define HEIGHT 144
#define VOLUME_WIDTH 20
#define VOLUME_HEIGHT 84

#define VOLUME_UP_ID 1
#define VOLUME_DOWN_ID 2

using namespace Gdiplus;
using std::string;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void keyPressed(KeyID id);
void update();

static TCHAR szWindowClass[] = _T("SpotifyVolumeControl");
static TCHAR szTitle[] = _T("Spotify Volume Control");

ULONG_PTR gdiplusToken;
HINSTANCE instance;
HWND handle;

AudioSession audioSession;

/*PeekMessage(msg, hwnd, wMsgFilterMin, wMsgFilterMax, PM_REMOVE)*/
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
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("RegisterClassEx failed"), _T("Title"), NULL);
		return 1;
	}

	instance = hInstance;
	/*CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT, CW_USEDEFAULT, 500, 100,
	NULL, NULL, hInstance, NULL);*/

	//int width = 64;
	//int height = 140;
	handle = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, szWindowClass, szTitle, WS_POPUP,
		50, 50, WIDTH, HEIGHT,
		NULL, NULL, hInstance, NULL);
	if (!handle)
	{
		MessageBox(NULL, _T("Createwindow failed"), _T("Title"), NULL);
		return 1;
	}

	SetWindowLong(handle, GWL_STYLE, 0);
	ShowWindow(handle, nCmdShow);
	UpdateWindow(handle);



	/*while (!exit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}*/

	/*DWORD waitTime = 1000;
	while (MsgWaitForMultipleObjects(0, NULL, FALSE, waitTime, QS_ALLINPUT) == WAIT_OBJECT_0)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

		}
	}*/

	HRESULT hr = audioSession.CreateSession();
	if (FAILED(hr))
	{
		std::cerr << "Error finding spotify\n";
		return 1;
	}
	ModifierKey modKey = MOD_ALT | MOD_NOREPEAT;
	Key volumeUpKey = VK_UP;
	Key volumeDownKey = VK_DOWN;
	BOOL r = RegisterHotKey(handle, VOLUME_UP_ID, modKey, volumeUpKey);
	RegisterHotKey(handle, VOLUME_DOWN_ID, modKey, VK_DOWN);

	UINT timeOut = 0;

	RedrawWindow(handle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_INVALIDATE);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnregisterHotKey(handle, VOLUME_UP_ID);
	UnregisterHotKey(handle, VOLUME_DOWN_ID);
	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
}
/*
if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
{
	if (msg.message != WM_HOTKEY) return;

	const WORD value = LOWORD(msg.wParam);

	for (auto const& v : hotkeyList)
	{
		if (value == v.first)
		{
			v.second(v.first);
		}
	}
}*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (msg)
	{
	case WM_HOTKEY:
	{
		const WORD value = LOWORD(wParam);

		if (value == VOLUME_UP_ID)
		{
			//ShowWindow(handle, SW_SHOW);
			const float vol = audioSession.GetVolume();
			if (vol == -1 || vol == 100) break;
			audioSession.SetVolume(vol + 2);
			// Volume omhoog
		}
		else if (value == VOLUME_DOWN_ID)
		{
			// Volume omlaag
			const float vol = audioSession.GetVolume();
			if (vol == -1 || vol == 0) break;
			audioSession.SetVolume(vol - 2);
		}
		RECT rect;
		//GetWindowRect(handle, &rect);
		RedrawWindow(handle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_INTERNALPAINT | RDW_INVALIDATE);
	}
	break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		Gdiplus::Graphics* graphics = Gdiplus::Graphics::FromHDC(hdc);
		const int vol = audioSession.GetVolume();
		if (vol != -1)
			VolumeBar::DrawVolume(graphics, vol);
		//drawVolumeBar(hdc);
		//drawVolumeLevelBar(hdc, 30);
		//drawVolumeText(hdc);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		GdiplusShutdown(gdiplusToken);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}
int mmain()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	Gdiplus::GdiplusStartupInput input = { 0 };
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &input, NULL);


	/*image = (HBITMAP)LoadImageA(
		NULL,
		"D:/Dion/Documents/GitHub/SpotifyVolumeControl/Debug/b35.jpg",
		IMAGE_BITMAP,
		0,0,
		LR_DEFAULTSIZE | LR_LOADFROMFILE
	);*/



	HRESULT hr = audioSession.CreateSession();
	if (FAILED(hr))
	{
		std::cout << "Error creating session\n";
		return 1;
	}

	const float volume = audioSession.GetVolume();
	std::cout << "Volume: " << volume << std::endl;

	/*Key modifier = MOD_ALT | MOD_NOREPEAT;

	Key key = 0x42;
	hotkey = key | modifier;*/

	/*if (RegisterHotKey(NULL, 1, modifier, key))
	{
		std::cout << "Hotkey registered: " << hotkey << "\n";
	}*/


	Keyboard keyboard;

	ModifierKey modKey = MOD_ALT | MOD_NOREPEAT;
	Key volumeUpKey = VK_UP;
	Key volumeDownKey = VK_DOWN;

	keyboard.AddHotkey(VOLUME_UP_ID, volumeUpKey, modKey, keyPressed);
	keyboard.AddHotkey(VOLUME_DOWN_ID, volumeDownKey, modKey, keyPressed);
	keyboard.AddHotkey(3, 0x42, modKey, keyPressed);
	//keyboard.AddHotkey(2, 0x43, modifier, keyPressed);
	//keyboard.AddHotkey(3, 0x44, modifier, keyPressed);


	/*while (run)
	{
		keyboard.Update();
		Sleep(1);
	}*/

	keyboard.RemoveHotkey(VOLUME_UP_ID);
	keyboard.RemoveHotkey(VOLUME_DOWN_ID);
	keyboard.RemoveHotkey(3);
	GdiplusShutdown(gdiplusToken);

	system("pause");
	return 0;
}

void keyPressed(KeyID id)
{
	switch (id)
	{
	case 1:
	{
		const float vol = audioSession.GetVolume();
		audioSession.SetVolume((vol + 10 > 100) ? 100 : vol + 10);
		//drawImage();
	}
	break;
	case 2:
	{
		const float vol = audioSession.GetVolume();
		audioSession.SetVolume((vol - 10 < 0) ? 0 : vol - 10);
	}
	break;
	case 3:
		//run = false;
		break;
	default:
		break;
	}

	std::cout << id << " Key pressed!\n";
}
void update()
{
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{

			std::cout << " Key Pressed\n";

			Key key = (((Key)msg.lParam >> 16) & 0xFFFF);
			Key modifier = ((Key)msg.lParam & 0xFFFF);
			int c = key | modifier;
			std::cout << key + modifier << std::endl;
			switch (LOWORD(msg.wParam))
			{
			case 1:
				std::cout << "Key: " << key << "\nModifier: " << modifier << std::endl;
				return;
			default:
				break;
			}

		}
	}
}


//
//HRESULT GetVolumeControl(IAudioSessionManager2* sessionManager, const LPWSTR programName, ISimpleAudioVolume** isav)
//{
//	HRESULT hr = S_OK;
//	IAudioSessionEnumerator* sessionEnumerator = NULL;
//	hr = sessionManager->GetSessionEnumerator(&sessionEnumerator);
//
//	if (FAILED(hr))
//	{
//		SAFE_RELEASE(sessionEnumerator);
//		return hr;
//	}
//
//	int sessionCount = 0;
//	hr = sessionEnumerator->GetCount(&sessionCount);
//
//	if (FAILED(hr))
//	{
//		SAFE_RELEASE(sessionEnumerator);
//		return hr;
//	}
//
//	bool found = false;
//	for (int i = 0; i < sessionCount && !found; i++)
//	{
//		IAudioSessionControl* sessionControl = NULL;
//		IAudioSessionControl2* sessionControl2 = NULL;
//
//		hr = sessionEnumerator->GetSession(i, &sessionControl);
//		hr = sessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
//
//		LPWSTR sessionIdentifier;
//		hr = sessionControl2->GetSessionIdentifier(&sessionIdentifier);
//		
//		if (wcsstr(sessionIdentifier, programName) != 0)
//		{
//			std::cout << "Found" << std::endl;
//			found = true;
//			ISimpleAudioVolume* _isav;
//			hr = sessionControl2->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&_isav);
//			*(isav) = _isav;
//			(*isav)->AddRef();
//
//			SAFE_RELEASE(_isav);
//		}
//
//		CoTaskMemFree(sessionIdentifier);
//		SAFE_RELEASE(sessionControl);
//		SAFE_RELEASE(sessionControl2);
//	}
//	SAFE_RELEASE(sessionEnumerator);
//	return hr;
//}
//HRESULT GetVolumeControl(IAudioSessionManager2* sessionManager, const DWORD pid, ISimpleAudioVolume** isav)
//{
//	HRESULT hr = S_OK;
//	IAudioSessionEnumerator* sessionEnumerator = NULL;
//	hr = sessionManager->GetSessionEnumerator(&sessionEnumerator);
//
//	if (FAILED(hr))
//	{
//		SAFE_RELEASE(sessionEnumerator);
//		return hr;
//	}
//
//	int sessionCount = 0;
//	hr = sessionEnumerator->GetCount(&sessionCount);
//
//	if (FAILED(hr))
//	{
//		SAFE_RELEASE(sessionEnumerator);
//		return hr;
//	}
//
//	bool found = false;
//	for (int i = 0; i < sessionCount && !found; i++)
//	{
//		IAudioSessionControl* sessionControl = NULL;
//		IAudioSessionControl2* sessionControl2 = NULL;
//
//		hr = sessionEnumerator->GetSession(i, &sessionControl);
//		hr = sessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
//
//		DWORD procID;
//		sessionControl2->GetProcessId(&procID);
//		std::cout << procID << std::endl;
//
//		if (procID == pid)
//		{
//			std::cout << "Found" << std::endl;
//			found = true;
//			ISimpleAudioVolume* _isav;
//			hr = sessionControl2->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&_isav);
//			*(isav) = _isav;
//			(*isav)->AddRef();
//
//			SAFE_RELEASE(_isav);
//		}
//		SAFE_RELEASE(sessionControl);
//		SAFE_RELEASE(sessionControl2);
//	}
//	SAFE_RELEASE(sessionEnumerator);
//	return hr;
//}
//
//HRESULT EnumerateSessions(IAudioSessionManager2* sessionManager)
//{
//	HRESULT hr = S_OK;
//	std::cout << "Enum sessions\n";
//	IAudioSessionEnumerator* sessionEnumerator = NULL;
//	hr = sessionManager->GetSessionEnumerator(&sessionEnumerator);
//
//	int sessionCount = 0;
//
//	hr = sessionEnumerator->GetCount(&sessionCount);
//
//	LPWSTR session = NULL;
//	IAudioSessionControl* sessionControl = NULL;
//	IAudioSessionControl2* sessionControl2 = NULL;
//
//	for (int i = 0; i < sessionCount; i++)
//	{
//		hr = sessionEnumerator->GetSession(i, &sessionControl);
//		hr = sessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
//		
//		hr = sessionControl->GetDisplayName(&session);
//
//		DWORD procID;
//		sessionControl2->GetProcessId(&procID);
//		LPWSTR sessionIdentifier = NULL;
//		sessionControl2->GetSessionIdentifier(&sessionIdentifier);
//
//		wprintf_s(L"Session Name: %s\n", session);
//		std::cout << "Proc ID: " << procID << std::endl;
//		
//		CoTaskMemFree(sessionIdentifier);
//	}
//
//	CoTaskMemFree(session);
//	SAFE_RELEASE(sessionControl2);
//	SAFE_RELEASE(sessionControl);
//	SAFE_RELEASE(sessionEnumerator);
//
//	return hr;
//}
//HRESULT CreateSessionManager(IAudioSessionManager2** ppSessionManager)
//{
//	HRESULT hr = S_OK;
//
//	IMMDevice* pDevice = NULL;
//	IMMDeviceEnumerator* pEnumerator = NULL;
//	IAudioSessionManager2* pSessionManager = NULL;
//
//	// Create the device enumerator.
//	hr = CoCreateInstance(
//		__uuidof(MMDeviceEnumerator),
//		NULL, CLSCTX_ALL,
//		__uuidof(IMMDeviceEnumerator),
//		(void**)&pEnumerator);
//	
//	if (FAILED(hr))
//	{
//		SAFE_RELEASE(pEnumerator);
//		return hr;
//	}
//
//	// Get the default audio device.
//	hr = pEnumerator->GetDefaultAudioEndpoint(
//		eRender, eConsole, &pDevice);
//
//	if (FAILED(hr))
//	{
//		SAFE_RELEASE(pEnumerator);
//		SAFE_RELEASE(pDevice);
//		
//		return hr;
//	}
//
//	// Get the session manager.
//	hr = pDevice->Activate(
//		__uuidof(IAudioSessionManager2), CLSCTX_ALL,
//		NULL, (void**)&pSessionManager);
//
//	if (FAILED(hr))
//	{
//		SAFE_RELEASE(pEnumerator);
//		SAFE_RELEASE(pDevice);
//		SAFE_RELEASE(pSessionManager);
//		
//		return hr;
//	}
//	
//	// Return the pointer to the caller.
//	*(ppSessionManager) = pSessionManager;
//	(*ppSessionManager)->AddRef();
//
//	// Clean up.
//	SAFE_RELEASE(pSessionManager);
//	SAFE_RELEASE(pEnumerator);
//	SAFE_RELEASE(pDevice);
//
//	return hr;
//}
//
//void PrintProcessInfo(DWORD processID)
//{
//	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
//
//	// Get a handle to the process.
//
//	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
//		PROCESS_VM_READ,
//		FALSE, processID);
//
//	// Get the process name.
//
//	if (NULL != hProcess)
//	{
//		HMODULE hMod;
//		DWORD cbNeeded;
//
//		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
//			&cbNeeded))
//		{
//			GetModuleBaseName(hProcess, hMod, szProcessName,
//				sizeof(szProcessName) / sizeof(TCHAR));
//		}
//	}
//
//	// Print the process name and identifier.
//	
//	_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
//
//	// Release the handle to the process.
//
//	CloseHandle(hProcess);
//}
//HRESULT GetAudioSessionPID(IAudioSessionManager2* sessionManager, const LPWSTR& sessionName, DWORD* pid)
//{
//	HRESULT hr = S_OK;
//	*pid = 0;
//
//	IAudioSessionEnumerator* sessionEnumerator = NULL;
//	hr = sessionManager->GetSessionEnumerator(&sessionEnumerator);
//	std::cout << "HEr 2\n";
//	if (FAILED(hr))
//	{
//		SAFE_RELEASE(sessionEnumerator);
//		return hr;
//	}
//	std::cout << "HEr 2\n";
//	int sessionCount = 0;
//	hr = sessionEnumerator->GetCount(&sessionCount);
//
//	if (FAILED(hr))
//	{
//		SAFE_RELEASE(sessionEnumerator);
//		return hr;
//	}
//
//	LPWSTR wstrSessionName = NULL;
//	IAudioSessionControl* sessionControl = NULL;
//	IAudioSessionControl2* sessionControl2 = NULL;
//
//	bool found = false;
//	for (int i = 0; i < sessionCount && !found; i++)
//	{
//		hr = sessionEnumerator->GetSession(i, &sessionControl);
//		hr = sessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void**)&sessionControl2);
//
//		hr = sessionControl->GetDisplayName(&wstrSessionName);
//
//		DWORD procID;
//		sessionControl2->GetProcessId(&procID);
//		LPWSTR sessionIdentifier = NULL;
//		sessionControl2->GetSessionIdentifier(&sessionIdentifier);
//		if (wcsstr(sessionIdentifier, sessionName) != 0)
//		{
//			found = true;
//			*pid = procID;
//		}
//
//		GUID guid;
//		LPOLESTR guidString;
//
//		sessionControl2->GetGroupingParam(&guid);
//		StringFromCLSID(guid, &guidString);
//		wprintf_s(L"Session Name: %s\nIdentifier: %s\n", wstrSessionName, sessionIdentifier);
//		std::cout << "Proc ID: " << procID << std::endl;
//		std::cout << guidString << std::endl;
//
//
//		if (found)
//		{
//			ISimpleAudioVolume* v = NULL;
//			sessionControl->QueryInterface(__uuidof(ISimpleAudioVolume), (void**)&v);
//			float level;
//			v->SetMasterVolume(1.0f, NULL);
//			//sessionControl2->
//				//std::cout << "LEvel: " << level << std::endl;
//				/*hr = sessionManager->GetSimpleAudioVolume(&guid, TRUE, &v);
//				LPCGUID eventContext = NULL;
//				std::cout << "FOR: " << procID << std::endl;
//				float level;
//				v->GetMasterVolume(&level);
//				std::cout << "LEvel: " << level << std::endl;*/
//		}
//		CoTaskMemFree(sessionIdentifier);
//		CoTaskMemFree(guidString);
//	}
//
//	CoTaskMemFree(wstrSessionName);
//	SAFE_RELEASE(sessionControl2);
//	SAFE_RELEASE(sessionControl);
//	SAFE_RELEASE(sessionEnumerator);
//
//	return hr;
//}