#include "AudioSession.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <gdiplus.h>
#include "keyboard.h"
#pragma comment(lib, "gdiplus.lib")

using std::string;
using namespace Gdiplus;
//void PrintProcessInfo(DWORD);
//HRESULT CreateSessionManager(IAudioSessionManager2** ppSessionManager);
//HRESULT EnumerateSessions(IAudioSessionManager2* sessionManager);
//HRESULT GetVolumeControl(IAudioSessionManager2* sessionManager, const DWORD pid, ISimpleAudioVolume** isav);

#define VOLUME_UP_ID 1
#define VOLUME_DOWN_ID 2

void keyPressed(KeyID id);
void update();

AudioSession audioSession;
bool run = true;
//HDC hdc;

int main()
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

	//update();
	//UnregisterHotKey(NULL, 1);
	//IAudioSessionManager2* audioSessionManager = NULL;
	////HRESULT hr = CreateSessionManager(&audioSessionManager);
	//if (!FAILED(hr))
	//{
	//	const DWORD pid = 4944;
	//	
	//	ISimpleAudioVolume* volume = NULL;
	//	hr = GetVolumeControl(audioSessionManager, pid, &volume);
	//	
	//	if (!FAILED(hr))
	//	{
	//		float currentVolume;
	//		hr = volume->GetMasterVolume(&currentVolume);
	//		if (!FAILED(hr))
	//			volume->SetMasterVolume(currentVolume+0.2f, NULL);
	//	}
	//}

	Keyboard keyboard;

	ModifierKey modKey = MOD_ALT | MOD_NOREPEAT;
	Key volumeUpKey = VK_UP; 
	Key volumeDownKey = VK_DOWN;

	keyboard.AddHotkey(VOLUME_UP_ID, volumeUpKey, modKey, keyPressed);
	keyboard.AddHotkey(VOLUME_DOWN_ID, volumeDownKey, modKey, keyPressed);
	keyboard.AddHotkey(3, 0x42, modKey, keyPressed);
	//keyboard.AddHotkey(2, 0x43, modifier, keyPressed);
	//keyboard.AddHotkey(3, 0x44, modifier, keyPressed);


	while (run)
	{
		keyboard.Update();
		Sleep(1);
	}

	keyboard.RemoveHotkey(VOLUME_UP_ID);
	keyboard.RemoveHotkey(VOLUME_DOWN_ID);
	keyboard.RemoveHotkey(3);
	GdiplusShutdown(gdiplusToken);

	system("pause");
	return 0;
}

Image* image;
/*
// Create a Pen object.
Pen pen (Color(255, 255, 0, 0), 2);

// Draw the original source image.
graphics.DrawImage(&image, 10, 10);

// Create a Rect object that specifies the destination of the image.
Rect destRect(200, 50, 150, 75);

// Draw the rectangle that bounds the image.
graphics.DrawRectangle(&pen, destRect);

// Draw the image.
graphics.DrawImage(&image, destRect);*/
void drawImage()
{
	//HDC hdc;// = HDC(0);
	HDC hdc = GetDC(0);

	Gdiplus::Graphics* graphics = Graphics::FromHDC(GetDC(0));


	//Gdiplus::Image image (L"D:\\Dion\\Pictures\\imgOut2.png");
	image = new Gdiplus::Image(L"D:\\Dion\\Pictures\\imgOut2.png");
	//if (image.) std::cout << "";
	//Gdiplus::Pen pen(Gdiplus::Color(255, 255, 0, 0), 2);
	//graphics.DrawImage(&image, 10, 10);
	Gdiplus::Rect rect(200, 50, 400, 400);
	
	//graphics.DrawRectangle(&pen, rect);
	graphics->DrawImage(image, rect);

}
void keyPressed(KeyID id)
{
	switch (id)
	{
	case 1:
	{
		const float vol = audioSession.GetVolume();
		audioSession.SetVolume((vol + 10 > 100) ? 100 : vol + 10);
		drawImage();
	}
		break;
	case 2:
	{
		const float vol = audioSession.GetVolume();
		audioSession.SetVolume((vol - 10 < 0) ? 0 : vol - 10);
	}
		break;
	case 3:
	
		delete image;
		image = nullptr;
		run = false;
		break;
	default:
		break;
	}

	std::cout << id <<" Key pressed!\n";
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