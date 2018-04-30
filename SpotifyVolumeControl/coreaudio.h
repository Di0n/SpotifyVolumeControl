#pragma once
#include <Windows.h>
#include <audiopolicy.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
   if(x != NULL)        \
   {                    \
      x->Release();     \
      x = NULL;         \
   }
#endif
namespace CoreAudio
{
	HRESULT CreateSessionManager(IAudioSessionManager2** ppSessionManager);
	HRESULT GetVolumeControl(IAudioSessionManager2* sessionManager, const DWORD pid, ISimpleAudioVolume** isav);
	HRESULT GetVolumeControl(IAudioSessionManager2* sessionManager, const LPWSTR programName, ISimpleAudioVolume** isav);
	HRESULT EnumerateSessions(IAudioSessionManager2* sessionManager);
	HRESULT GetAudioSessionPID(IAudioSessionManager2* sessionManager, const LPWSTR& sessionName, DWORD* pid);
}