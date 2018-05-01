#include "AudioSession.h"
#include <iostream>
#include "coreaudio.h"

AudioSession::~AudioSession()
{
	SAFE_RELEASE(audioControl);
	SAFE_RELEASE(sessionManager);
}
HRESULT AudioSession::CreateSession()
{
	HRESULT hr = CoreAudio::CreateSessionManager(&sessionManager);
	if (FAILED(hr) || sessionManager == nullptr)
		return !FAILED(hr) ? E_POINTER : hr;

	hr = CoreAudio::GetVolumeControl(sessionManager, SPOTIFY_EXECUTABLE, &audioControl);

	return hr;
}

void AudioSession::SetVolume(float volume)
{
	if (audioControl == nullptr) return;
	if (volume > 100) volume = 100;
	else if (volume < 0) volume = 0;
	audioControl->SetMasterVolume((volume / 100), NULL);
}

float AudioSession::GetVolume() const
{
	if (audioControl != nullptr)
	{
		float volume;
		if (!FAILED(audioControl->GetMasterVolume(&volume)))
			return volume * 100;
	}
	return -1;
}

void AudioSession::Mute(const bool mute)
{
	if (audioControl != nullptr) audioControl->SetMute(mute, NULL);
}
