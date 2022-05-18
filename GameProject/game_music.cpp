#include "stdafx.h"
#include "game_music.h"

Music::Music() 
{
	soundEffect_ = NULL;
	music_ = NULL;

}
Music::~Music() 
{
	if (soundEffect_ != NULL)
	{
		Mix_FreeChunk(soundEffect_);
		soundEffect_ = NULL;
	}
		
	if (music_ != NULL)
	{
		Mix_FreeMusic(music_);
		music_ = NULL;
	}
}

Mix_Music * Music::setMusicFromFile(std::string path)
{
	music_ = Mix_LoadMUS(path.c_str());
	return music_;
}

Mix_Chunk* Music::setSoundEffectFromFile(std::string path)
{
	soundEffect_ = Mix_LoadWAV(path.c_str());
	return soundEffect_;
}

void Music::handleInputMusic(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
		case SDLK_p:
			if (!Mix_PlayingMusic())
			{
				Mix_PlayMusic(music_, -1);
			} 
			else if (Mix_PausedMusic())
			{
				Mix_ResumeMusic();
			}
			else 
			{
				Mix_PauseMusic();
			}
			break;
		case SDLK_s:
			Mix_HaltMusic();
			break;
		case SDLK_3:
			Mix_PlayChannel(-1, soundEffect_, 2);
		}
	}
}
