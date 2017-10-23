#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <iostream>

class SoundSystem
{
protected:
std::map<const char*, Mix_Chunk*> soundMap;
bool muted;

public:
	SoundSystem();
	void addChunk(const char* name);
	void playChunk(const char* name);
	void quitSound();
	void muteSound();
};