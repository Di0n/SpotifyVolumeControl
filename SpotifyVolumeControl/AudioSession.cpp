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
		return hr;

	hr = CoreAudio::GetVolumeControl(sessionManager, SPOTIFY_EXECUTABLE, &audioControl);

	return hr;
}

void AudioSession::SetVolume(const float volume)
{
	if (audioControl != nullptr)
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
