#ifndef BASE_OBJECT_H_

#define BASE_OBJECT_H_


#include "CommonFunction.h"

class BaseObject
{
public : 
	BaseObject();
	~BaseObject();
	// Set size of image
	void SetRect(const int& x , const int& y) 
	{
		rect_.x = x;
		rect_.y = y;
	}

	// Return size of image 
	SDL_Rect GetRect() const {return rect_;}

	//Return all image
	SDL_Texture* GetObject() const {return p_object;}

	bool loadImage (std::string path, SDL_Renderer * screen);
	void Render(SDL_Renderer * des, const SDL_Rect * clip = NULL);
	void Free();

protected :
	// All image
	SDL_Texture * p_object ;

	// Size of image
	SDL_Rect  rect_;

};




#endif