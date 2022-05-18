#ifndef TEXT_OBJECT_H_
#define TEXT_OBJECT_H_

#include "CommonFunction.h"

class TextObject 
{
public:
	enum TextColor
	{
		RED_TEXT = 0,
		WHITE_TEXT = 1,
		BLACK_TEXT = 2,
	};
	TextObject();
	~TextObject();

	bool loadFormRenderText(TTF_Font* font, SDL_Renderer * screen);

	void setColor(Uint8 red, Uint8 green, Uint8 blue );
	void setColor(int type);

	void renderText(SDL_Renderer * screen,
					int x_pos, int y_pos, 
					SDL_Rect * clip = NULL, 
					double angle = 0.0, 
					SDL_Point * center = NULL, 
					SDL_RendererFlip flip = SDL_FLIP_NONE);
	int GetWidth() const {return width_;}
	int GetHeight() const{ return height_;}

	void setText(std::string& text) {str_val = text;}
	std::string GetText() const { return str_val;}

	void Free();

private:
	std::string str_val;
	SDL_Color text_color_;
	SDL_Texture * textture_;
	int width_;
	int height_;
};

#endif