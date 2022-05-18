#include "stdafx.h"
#include "TextObject.h"

TextObject::TextObject()
{
	text_color_.r = 255;
	text_color_.g = 255;
	text_color_.b = 255;
	textture_ = NULL;
}
TextObject::~TextObject()
{

}

bool TextObject::loadFormRenderText(TTF_Font* font, SDL_Renderer * screen)
{
	SDL_Surface * text_surface = TTF_RenderText_Solid(font, str_val.c_str(), text_color_);
	if (text_surface)
	{
		textture_ = SDL_CreateTextureFromSurface(screen, text_surface);
		width_ = text_surface->w;
		height_ = text_surface->h;
		SDL_FreeSurface(text_surface);
	}

	return textture_ != NULL;
}

void TextObject::Free() {
	if (textture_ != NULL)
	{
		SDL_DestroyTexture(textture_);
		textture_ = NULL;
	}
}

void TextObject::setColor(Uint8 red, Uint8 green, Uint8 blue )
{
	text_color_.r = red;
	text_color_.g = green;
	text_color_.b = blue;
}

void TextObject::setColor(int type)
{
	if (type == RED_TEXT)
	{
		SDL_Color color = {255,0,0};
		text_color_ = color;
	} else if (type == BLACK_TEXT)
	{
		SDL_Color color = {0,0,0};
		text_color_ = color;
	} else if (type == WHITE_TEXT)
	{
		SDL_Color color = {255,255,255};
		text_color_ = color;
	}
}

void TextObject::renderText(SDL_Renderer * screen,int x_pos, int y_pos, SDL_Rect * clip , double angle , SDL_Point * center , SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = {x_pos, y_pos, width_, height_};
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(screen, textture_, clip, &renderQuad, angle, center,flip);
	
}