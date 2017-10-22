#pragma once
#include "dependencies\SDL2-2.0.6\include\SDL.h"
#include <iostream>
using namespace std;

class Sound
{
public:
	Sound(char *soundFile);
	void play();
	void playLoop();
	void stop();

	~Sound();
private:
	SDL_AudioSpec wavSpec;
	Uint32 wavLength;
	Uint8 *wavBuffer;
	SDL_AudioDeviceID deviceId;
};

