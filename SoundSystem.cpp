#include "SoundSystem.h"

SoundSystem::SoundSystem() {
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return;
	}
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		return;
	}
	muted = false;
}

void SoundSystem::addChunk(const char* name) 
{
	Mix_Chunk *chunk = Mix_LoadWAV(name);
	if(chunk == NULL) {
		std::cout << "Could not find .wav file to add" << "\n";
		return;
	}
	soundMap.insert(std::make_pair(name, chunk));
}

void SoundSystem::playChunk(const char* name) 
{
	if(muted) {
 		return;
 	}
	std::map<const char*, Mix_Chunk*>::iterator it;
	it = soundMap.find(name);
	if(it == soundMap.end()) {
		std::cout << "Could not find .wav file in SoundSystem" << "\n";
		return;
	}
	Mix_PlayChannel(-1, it->second, 0);
}

void SoundSystem::quitSound() 
{
	std::map<const char*, Mix_Chunk*>::iterator it = soundMap.begin();
    while(it != soundMap.end())
    {
       Mix_FreeChunk(it -> second);
       soundMap.erase(it);
       it++;
    }
	Mix_CloseAudio();
	SDL_Quit();
}
 
void SoundSystem::muteSound()
{
 	muted = !muted;
 	std::cout << "Muted" << muted << "\n";
}