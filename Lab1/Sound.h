#pragma once
#include <SDL/SDL.h>
#include <iostream>

class Sound
{
public:
	Sound();
	~Sound();

	void init(const std::string& filePath);
	void play();
	void setupDevice();

private:
	SDL_AudioDeviceID _device;
	SDL_AudioSpec _audioSpec;
	Uint8* _waveStart;
	Uint32 _waveLength;
};

