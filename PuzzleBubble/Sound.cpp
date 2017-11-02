#include "Sound.h"



Sound::Sound(char *soundFile)
{
	SDL_Init(SDL_INIT_AUDIO);
	SDL_LoadWAV(soundFile, &wavSpec, &wavBuffer, &wavLength);
	deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
}

void Sound::play()
{	
	SDL_QueueAudio(deviceId, wavBuffer, wavLength);
	SDL_PauseAudioDevice(deviceId, 0);
}


void Sound::stop()
{
	SDL_PauseAudioDevice(deviceId, 1);
}


Sound::~Sound()
{
	SDL_CloseAudioDevice(deviceId);
}
