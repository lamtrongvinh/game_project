#include "stdafx.h"
#include "CommonFunction.h"

bool SDLCommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2) 
{
  int left_a = object1.x;
  int right_a = object1.x + object1.w;
  int top_a = object1.y;
  int bottom_a = object1.y + object1.h;
 
  int left_b = object2.x;
  int right_b = object2.x + object2.w;
  int top_b = object2.y;
  int bottom_b = object2.y + object2.h;
 
  // Case 1: size object 1 < size object 2
  if (left_a > left_b && left_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }
 
  if (left_a > left_b && left_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }
 
  if (right_a > left_b && right_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }
 
  if (right_a > left_b && right_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }
  
  // Case 2: size object 1 < size object 2
  if (left_b > left_a && left_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }
 
  if (left_b > left_a && left_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }
 
  if (right_b > left_a && right_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }
 
  if (right_b > left_a && right_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }
 
   // Case 3: size object 1 = size object 2
  if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
  {
    return true;
  }
 
  return false;
}

int SDLCommonFunc::ShowMenu(SDL_Renderer* g_screen, TTF_Font* font, 
                      const std::string& menu1, 
                      const std::string& menu2,
					  const std::string& menu3,
                      const std::string& img_name_1,
					  int point)
{
	char* ch1 =(char*) menu1.c_str();
	char* ch2 =(char*) menu2.c_str();

	// them
	char* ch3 = (char*) menu3.c_str();
	char* img_file_1 = (char*)img_name_1.c_str();
	// them

	int size1 = menu1.length();
	int size2 = menu2.length();
	int size3 = menu3.length();

	int time = 0;
	int x = 0;
	int y = 0;
	const int key_MenuNum = 3;

	char * labels[key_MenuNum];

	labels[0] = new char [size1 + 1];
	labels[1] = new char [size2 + 1];
	labels[2] = new char [size3 + 1];
	
	strcpy_s(labels[0], size1+1, ch1);
    strcpy_s(labels[1], size2+1, ch2);
	strcpy_s(labels[2], size3+1, ch3);
	std::string labels_0 = labels[0];
	std::string labels_1 = labels[1];
	std::string labels_2 = labels[2];

	SDL_Texture* menu[key_MenuNum]; 
	bool selected[key_MenuNum] = {0, 0};
	SDL_Color color[2] = { {255, 255, 255 } , {255, 0, 0} };
	TextObject text_object[key_MenuNum];

	text_object[0].setText(labels_0);
	text_object[0].setColor(color[0].r, color[0].g, color[0].b);
	text_object[0].loadFormRenderText(font, g_screen);

	text_object[1].setText(labels_1);
	text_object[1].setColor(color[0].r, color[0].g, color[0].b);
	text_object[1].loadFormRenderText(font, g_screen);

	text_object[2].setText(labels_2);
	text_object[2].setColor(color[0].r, color[0].g, color[0].b);
	text_object[2].loadFormRenderText(font, g_screen);

	SDL_Rect pos[key_MenuNum];
	pos[0].x = SCREEN_WIDTH*0.2 - 150;
    pos[0].y = SCREEN_HEIGHT*0.8 - 50;
    pos[1].x = SCREEN_WIDTH*0.2 - 150;
    pos[1].y = SCREEN_HEIGHT*0.8 + 0;
	pos[2].x = SCREEN_WIDTH*0.2 - 150;
    pos[2].y = SCREEN_HEIGHT*0.8 + 50;

	BaseObject gBackground;
	bool ret = gBackground.loadImage(img_file_1, g_screen);


	SDL_Event event;
	while(true)
	{
		time = SDL_GetTicks();
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				text_object[0].Free();
                text_object[0].Free();
                gBackground.Free();
                return 1;
			case SDL_MOUSEMOTION:
				x = event.motion.x;
				y = event.motion.y;
				for (int i = 0 ; i < key_MenuNum; i++)
				{
					if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
					{
						if (!selected[i])
						{
							selected[i] = 1;
							std::string str_labels_i = labels[i];
							text_object[i].setText(str_labels_i); 
							text_object[i].setColor(color[1].r, color[1].g, color[1].b);
							text_object[i].loadFormRenderText(font, g_screen);
						}
					 
					} else 
					{
						if (selected[i])
						{
							selected[i] = 0;
							text_object[i].Free();
							std::string str_labels_i = labels[i];
							text_object[i].setText(str_labels_i);
							text_object[i].setColor(color[0].r, color[0].g, color[0].b);
							text_object[i].loadFormRenderText(font, g_screen);
						}
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				x = event.button.x;
				y = event.button.y;
				for (int i = 0 ; i < key_MenuNum; i++)
				{
					if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
					{
						text_object[0].Free();
						text_object[1].Free();
						text_object[2].Free();
						gBackground.Free();
						return i;
					}
				}
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					text_object[0].Free();
                    text_object[1].Free();
                    gBackground.Free();
                    return 1;
				}
			}
		}

		gBackground.Render(g_screen, NULL);

		for (int i = 0 ; i < key_MenuNum; i++)
		{
			text_object[i].renderText(g_screen, pos[i].x, pos[i].y);
			pos[i].w = text_object[i].GetWidth();
			pos[i].h = text_object[i].GetHeight();
		}

		SDL_RenderPresent(g_screen);
        if (1000 / 30 > (SDL_GetTicks() - time))
		{
			SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
		}    
	}
	return 0;
}
 
