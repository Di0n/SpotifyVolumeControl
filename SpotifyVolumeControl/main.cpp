#include "AudioSession.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <tchar.h>

using std::string;

AudioSession audioSession;

//void PrintProcessInfo(DWORD);
//HRESULT CreateSessionManager(IAudioSessionManager2** ppSessionManager);
//HRESULT EnumerateSessions(IAudioSessionManager2* sessionManager);
//HRESULT GetVolumeControl(IAudioSessionManager2* sessionManager, const DWORD pid, ISimpleAudioVolume** isav);


int main()
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	
	HRESULT hr = audioSession.CreateSession();
	if (FAILED(hr))
	{
		std::cout << "Error creating session\n";
		return 1;
	}

	const float volume = audioSession.GetVolume();
	std::cout << "Volume: " << volume << std::endl;


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

	system("pause");
	return 0;
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