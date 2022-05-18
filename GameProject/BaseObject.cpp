#include "stdafx.h"
#include "BaseObject.h"

BaseObject::BaseObject()
{
	p_object = NULL;
	rect_.x = 0 ;
	rect_.y = 0 ;
	rect_.w = 0 ;
	rect_.h = 0 ;
}

BaseObject::~BaseObject()
{
	Free();
}

bool BaseObject::loadImage(std::string path, SDL_Renderer * screen) 
{
	Free();

	// Result on screen
	SDL_Texture * new_texture = NULL;

	//  Load image (IMG_Load : tra ve gia tri la 1 surface )
	SDL_Surface* load_Surface = IMG_Load(path.c_str());

	if (load_Surface !=NULL) 
	{
		// transparent in character
		SDL_SetColorKey(load_Surface,SDL_TRUE, SDL_MapRGB(load_Surface -> format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		// new_texture luu tru toan bo thong tin hinh anh
		new_texture = SDL_CreateTextureFromSurface(screen, load_Surface );

		if (new_texture != NULL)
		{
			rect_.w = load_Surface -> w;
			rect_.h = load_Surface -> h;
		}
		// giai phong surface
		SDL_FreeSurface(load_Surface);
	}

	p_object = new_texture;

	return p_object != NULL ;
}

void BaseObject::Render(SDL_Renderer * des, const SDL_Rect* clip )
{
	// xay dung kich thuoc vi tri
	SDL_Rect renderquad = {rect_.x, rect_.y, rect_.w, rect_.h};
	// day toan bo thong tin cua p_object -> des voi thong so kich thuoc vi tri 'renderquad'
	SDL_RenderCopy(des, p_object, clip, &renderquad);
}

void BaseObject::Free() 
{
	if (p_object != NULL)
	{
		SDL_DestroyTexture(p_object);
		p_object = NULL;
		rect_.w = 0;
		rect_.h = 0;
	}
}