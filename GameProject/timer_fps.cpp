#include "stdafx.h"
#include "timer_fps.h"
#include "CommonFunction.h"

Imptimer::Imptimer()
{
	start_tick_ = 0;
	paused_tick_ = 0;
	is_paused_ = false;
	is_started_ = false;
}

Imptimer::~Imptimer()
{

}

void Imptimer::start()
{
	is_started_ = true;
	is_paused_ = false;
	start_tick_ = SDL_GetTicks();
}

void Imptimer::stop()
{
	is_paused_ = false;
	is_started_ = false;
}

void Imptimer::paused()
{
	if (is_started_ == true && is_paused_ == false)
	{
		is_paused_ = true;
		paused_tick_ = SDL_GetTicks() - start_tick_;
	}
}

void Imptimer::unpaused()
{
	if (is_paused_ == true)
	{
		is_paused_ = false;
		start_tick_ = SDL_GetTicks() - paused_tick_;
		paused_tick_ = 0;
	}
}

int Imptimer::get_ticks()
{
	if (is_started_ == true)
	{
		if (is_paused_ == true)
		{
			return paused_tick_;
		} else
		{
			return SDL_GetTicks() - start_tick_;
		}
	}

	return 0;
}

bool Imptimer::is_started()
{
	return is_started_;
}

bool Imptimer::is_paused()
{
	return is_paused_;
}
