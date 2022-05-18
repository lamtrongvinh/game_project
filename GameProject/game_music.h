#ifndef _GAME_MUSIC_H_
#define _GAME_MUSIC_H_

#include "CommonFunction.h"

class Music 
{
public:
	Music();
	~Music();
	Mix_Music* setMusicFromFile(std::string path);
	Mix_Chunk* setSoundEffectFromFile(std::string path);
	void handleInputMusic(SDL_Event event);

private:
	Mix_Chunk* soundEffect_;
	Mix_Music* music_;
};

#endif