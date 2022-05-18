#ifndef COMMON_FUNCTION_H_

#define COMMON_FUNCTION_H_

#include <Windows.h>
#include <string>
#include <SDL.h>
#include <vector>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "TextObject.h"
#include "BaseObject.h"


static SDL_Window* g_Window = NULL;
static SDL_Renderer* g_Screen = NULL;
static SDL_Event  g_Event ;

//SCREEN
const int FRAME_FPS_SECOND = 35;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

//COLOR KEY
const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOR = 255;
// Kich thuoc cua 1 o tren map
#define TILE_SIZE 64

// 400 cot
#define MAX_MAP_X  400
//10 hang
#define MAX_MAP_Y  10

// trang trai
#define BLANK_TILE 0
#define STATE_COIN 12 
//Move
typedef struct Input
{
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;
};


typedef struct Map
{
	int start_x_;
	int start_y_;

	int max_x_;
	int max_y_;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	char * file_name_;
};

namespace SDLCommonFunc 
{
	bool CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2);
	int ShowMenu(SDL_Renderer* g_screen, TTF_Font* font, 
                      const std::string& menu1, 
                      const std::string& menu2,
					  const std::string& menu3,
                      const std::string& img_name_1,
					  int point);
};

#endif