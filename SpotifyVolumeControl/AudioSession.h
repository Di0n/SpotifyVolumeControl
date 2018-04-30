#pragma once
#include "coreaudio.h"
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
   if(x != NULL)        \
   {                    \
      x->Release();     \
      x = NULL;         \
   }
#endif
#define SPOTIFY_EXECUTABLE L"Spotify.exe"
class AudioSession
{
public:
	AudioSession() {};
	~AudioSession();
	/// <summary>
	/// Create an audio session
	/// </summary>
	HRESULT CreateSession();
	/// <summary>
	/// Set audio session volume
	/// </summary>
	void SetVolume(float volume);
	/// <summary>
	/// Gets the current audio session volume
	/// </summary>
	float GetVolume() const;
	/// <summary>
	/// Toggles mute on the current audio session
	/// </summary>
	void Mute(const bool mute);

private:
	IAudioSessionManager2* sessionManager;
	ISimpleAudioVolume* audioControl;
};

