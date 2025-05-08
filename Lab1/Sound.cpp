#include "Sound.h"


Sound::Sound() {}

Sound::~Sound()
{
	SDL_FreeWAV(_waveStart);
	SDL_CloseAudioDevice(_device);
}

void Sound::init(const std::string& filePath)
{
	if (SDL_LoadWAV(filePath.c_str(), &_audioSpec, &_waveStart, &_waveLength) == nullptr)
	{
		std::cerr << "Error loading sound: " << filePath << " , " << SDL_GetError() << std::endl;
	}
}

void Sound::play()
{
	SDL_ClearQueuedAudio(_device);
	int status = SDL_QueueAudio(_device, _waveStart, _waveLength);
	SDL_PauseAudioDevice(_device, 0);
}

void Sound::setupDevice()
{
	_device = SDL_OpenAudioDevice(nullptr, 0, &_audioSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);
	if (0 == _device)
	{
		std::cerr << "Error: sound device: " << SDL_GetError() << std::endl;
	}
}