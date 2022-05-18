// GameProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CommonFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "timer_fps.h"
#include "ThreatsObject.h"
#include "game_music.h"


TTF_Font * font_text = NULL;
bool InitData()
{

	bool succes = true ;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	
	if (ret < 0 )
	{
		return false;
	} else 
	{
		// Set ti le va chat luong
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	}

	g_Window = SDL_CreateWindow("KILL Monster 1.0",
								SDL_WINDOWPOS_UNDEFINED,
								SDL_WINDOWPOS_UNDEFINED, 
								SCREEN_WIDTH, SCREEN_HEIGHT, 
								SDL_WINDOW_SHOWN);

	if (g_Window == NULL) 
	{
		succes = false;
	} 
	else 
	{
		g_Screen = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED);
		if (g_Screen == NULL)
		{
			succes = false; 
		}
		else 
		{
			SDL_SetRenderDrawColor(g_Screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
			{
				succes = false;
			}
		}

		if (TTF_Init() == -1)
		{
			succes = false;
		}
		font_text  = TTF_OpenFont("font//dlxfont_.ttf", 15);
		if (font_text == NULL)
		{
			succes = false;
		}

		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            succes = false;
        }

	} 

	return succes;
}

BaseObject g_background;
BaseObject highscore;

SDL_Texture* loadImg( std::string path )
{
   
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( g_Screen, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}
SDL_Texture * gTexture ;

bool LoadBackground() 
{

	bool ret = g_background.loadImage("img//background1234.png", g_Screen);
	if (ret == false )
	{
		return false;
	}	
}



void fps_processing(int real_imp_time , int time_one_frame )
{
	if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			if (delay_time >= 0)
			{
				SDL_Delay(delay_time);
			}
		}
}

std::vector<ThreatsObject*> MakeThreatsList()
{
	std::vector<ThreatsObject*> list_threats;


	ThreatsObject* dynamic_threats = new ThreatsObject[20];
	for (int i = 0 ; i < 20 ; i++)
	{
		ThreatsObject*  p_threat = (dynamic_threats + i );
		if (p_threat != NULL)
		{
			p_threat->loadImg("img//threat_left.png", g_Screen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREATS);
			p_threat->set_x_pos(500 +i* 500);
			p_threat->set_y_pos(200);


			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60;
			p_threat->SetAnimationPos(pos1, pos2);
			p_threat->set_input_left(1);

			list_threats.push_back(p_threat);
		}
	}


	ThreatsObject* threats_objs = new ThreatsObject[20];

	for (int i = 0 ; i < 20 ; i++)
	{
		ThreatsObject* p_threat = (threats_objs + i);
		if (p_threat != NULL)
		{
			p_threat->loadImg("img//threat_level.png", g_Screen);
			p_threat->set_clips();
			p_threat->set_x_pos(700 + i*1200);
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatsObject::STATIC_THREATS);
			p_threat->set_input_left(0);

			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_Screen);

			list_threats.push_back(p_threat);
		}
	}

	return list_threats;
}

void Destroy_ThreatsObject(std::vector<ThreatsObject*> &threats_list_)
{
	for (int i = 0 ; i < threats_list_.size(); i++)
	{
		ThreatsObject* p_theart = threats_list_.at(i);
		if (p_theart != NULL)
		{
			p_theart->Free();
			p_theart = NULL;
		}
	}
	threats_list_.clear();
}


void close()
{
	g_background.Free();

	
	SDL_DestroyRenderer(g_Screen);
	g_Screen = NULL;

	SDL_DestroyWindow(g_Window);
	g_Window = NULL;

	IMG_Quit();
		
	SDL_Quit();
}


int main(int argc, char* argv[])
{
	Imptimer Fps_timer;

	if (InitData() == false )
	{
		return -1;
	}
	

	if (LoadBackground() == false )
	{
		return -1;
	}

	
	GameMap game_map;
	
	game_map.LoadMap("map//map.dat");
	game_map.LoadTiles(g_Screen);

	int count_time_ = 0;
play_again:
	MainObject p_player;
	p_player.loadImage("img//player_right.png", g_Screen);
	p_player.set_clips();
	
	std::vector<ThreatsObject*>  threats_list = MakeThreatsList();

	//Time Game
	TextObject time_game;
	time_game.setColor(TextObject::WHITE_TEXT);

	// set MUSIC
	Music game_music_;
	Mix_Chunk * soundEffect_;
	Mix_Chunk * eat_coin;
	Mix_Chunk * music_die;
	Mix_Chunk * threats_die;
	Mix_Chunk * player_move;
	Mix_Music * player_win;
	Mix_Music * music_;
	Mix_Music * game_over_;
	
	music_ = game_music_.setMusicFromFile("music//music.wav");
	eat_coin = game_music_.setSoundEffectFromFile("music//eat_coin.wav");
	soundEffect_ = game_music_.setSoundEffectFromFile("music//gun.wav");
	game_over_ = game_music_.setMusicFromFile("music//game_over.wav");
	music_die = game_music_.setSoundEffectFromFile("music//player_die.wav");
	player_move = game_music_.setSoundEffectFromFile("music//player_move2.wav");
	player_win = game_music_.setMusicFromFile("music//player_win.wav");
	threats_die = game_music_.setSoundEffectFromFile("music//threats_die.wav");


	bool isQuit = false;
	bool game_end_ = false;

	int num_die = 5;
	int player_score = 0;

	count_time_++;
	// menu
	int ret_menu = SDLCommonFunc::ShowMenu(g_Screen, font_text, "Join Game", "Quit", "", "img//intro.png" , 0 ); 
	if (ret_menu == 1)
	{
		isQuit = true;
	}

	
	bool is_replay = false;
	
	// Play Music
	Mix_PlayMusic( music_, 5 );
	Mix_VolumeMusic(50);
	// game loop
	while (!isQuit)
	{
		Fps_timer.start();
		
		while (SDL_PollEvent(&g_Event) != 0 )
		{
			if (g_Event.type == SDL_QUIT)
			{
				isQuit = true;
			}

			p_player.HandleInputAction(g_Event, g_Screen,soundEffect_, player_move );
			
		}

		SDL_SetRenderDrawColor(g_Screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_Screen);

		g_background.Render(g_Screen, NULL);
		
		Map map_data = game_map.getMap();

		bool is_win_ = p_player.getIsWin();

		p_player.HandleBullet(g_Screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data, eat_coin, num_die, music_die);
		p_player.Show(g_Screen);
		
		game_map.SetMap(map_data);

		game_map.DrawMap(g_Screen);

		for (int i = 0 ; i < threats_list.size() ; i++)
		{
			ThreatsObject* p_threat = threats_list.at(i);
			if (p_threat != NULL)
			{
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(g_Screen);
				p_threat->DoPlayer(map_data);
				p_threat->MakeBullet(g_Screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				p_threat->Show(g_Screen);

				SDL_Rect rect_player = p_player.GetRectFrame();
				bool flag_bCol = false;
				std::vector<BulletObject*> t_Bullet_list = p_threat->get_bullet_list();

				for (int jj = 0 ; jj < t_Bullet_list.size(); jj++)
				{
					BulletObject* pt_Bullet = t_Bullet_list.at(jj);
					if (pt_Bullet)
					{
						flag_bCol = SDLCommonFunc::CheckCollision(pt_Bullet->GetRect(), rect_player);
						if (flag_bCol)
						{
							p_threat->RemoveBullet(jj);
							break;
						}
					}
				}
		
				SDL_Rect rect_threat = p_threat->GetRectFrame();
				bool flag_bCol_2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);


				if (num_die == 0)
				{
					SDL_Delay(500);
					Mix_VolumeMusic(0);
					Mix_PlayMusic( game_over_, 0 );
					Mix_VolumeMusic(100);
					int ret_menu_1 = SDLCommonFunc::ShowMenu(g_Screen, font_text,
                                                    "Play Again", "Quit", "",
                                                    "img//game_over1.png",
													0);


					if (ret_menu_1 == 0) // Play Again
					{		
						is_replay = true; 
						break;
					} 
					if (ret_menu_1 == 1)
					{
						isQuit = true;
						break;
					}
				}

				if (flag_bCol || flag_bCol_2)
				{
					num_die--;

					Mix_PlayChannel(-1, music_die, 0);
					if (num_die > 0)
					{
						p_player.SetRect(0,0);
						p_player.setComeBackTime(60);
						SDL_Delay(500);
						continue;
					}
					else {
						
						SDL_Delay(500);
						Mix_VolumeMusic(0);
						Mix_PlayMusic( game_over_, 0 );
						Mix_VolumeMusic(100);
						

						int ret_menu_1 = SDLCommonFunc::ShowMenu(g_Screen, font_text,
                                                    "Play Again", "Quit", "",
                                                    "img//game_over1.png",
													0);


						if (ret_menu_1 == 0) // Play Again
						{		
							is_replay = true;
							break;
						} 
						else 
						{
							isQuit = true;
							break;
						}
					}
				}
				//end

			}
		}

		// begin 12/5
		if (is_win_ == true)
		{
			SDL_Delay(500);
			Mix_PlayMusic( player_win, 0 );
			Mix_VolumeMusic(100);
			int ret_menu_1 = SDLCommonFunc::ShowMenu(g_Screen, font_text,
                                                   "Play Again", "Quit", "",
                                                    "img//you_win.png",
													0);
						
			if (ret_menu_1 == 0) // Play Again
			{		
				is_replay = true;
				break;
			} 
			else 
			{
				isQuit = true;
				continue;
			}
		}
				//end

		if (is_replay == true)
		{
			break;
		}

		std::vector<BulletObject*> bullet_list = p_player.get_bullet_list();
		for (int i = 0 ; i < bullet_list.size(); i++)
		{
			BulletObject* p_bullet = bullet_list.at(i);
			if (p_bullet != NULL)
			{
				for (int  j = 0 ; j < threats_list.size(); j++)
				{
					ThreatsObject* obj_threats = threats_list.at(j);
					if (obj_threats != NULL)
					{
						SDL_Rect tRect;
						tRect.x = obj_threats->GetRect().x;
						tRect.y = obj_threats->GetRect().y;
						tRect.w = obj_threats->get_width_frame();
						tRect.h = obj_threats->get_height_frame();

						SDL_Rect bRect = p_bullet->GetRect();
						
						bool bCol = SDLCommonFunc::CheckCollision(tRect, bRect);

						if  (bCol)
						{
							p_player.RemoveBullet(i);
							Mix_PlayChannel(-1, threats_die, 0);
							player_score++;
							obj_threats->Free();
							threats_list.erase(threats_list.begin() + j);
						}
						
					}
				}
			}
		}
		
		//SHOW Diamond
		std::string str_point = ": ";
		
		int val_of_point = p_player.getCoin();
		std::string str_val_of_point = std::to_string(val_of_point);
		str_point += str_val_of_point;

		time_game.setText(str_point);
		time_game.loadFormRenderText(font_text, g_Screen);
		time_game.renderText(g_Screen, SCREEN_WIDTH - 410, 30 );

		//SHOW TEXT SCORE
		int your_score = player_score * 5 - (5 - num_die)* 5 + val_of_point * 10;
		if (your_score < 0)
		{
			your_score = 0;
		}
		std::string str_time = "Score : ";
		std::string str_val = std::to_string(your_score);
		str_time += str_val ;

		time_game.setText(str_time);
		time_game.loadFormRenderText(font_text, g_Screen);
		time_game.renderText(g_Screen, SCREEN_WIDTH - 200, 30);
		

		// SHOW HEAL

		std::string str_life = ": ";
		std::string str_heal = std::to_string(num_die);
		str_life += str_heal;

		time_game.setText(str_life);
		time_game.loadFormRenderText(font_text, g_Screen);
		time_game.renderText(g_Screen, 90, 30 );

		SDL_RenderPresent(g_Screen);

		int real_imp_time = Fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_FPS_SECOND;
		
  	    fps_processing(real_imp_time, time_one_frame );
		
	}

	if (is_replay == true)
	{
		p_player.Free();
		Destroy_ThreatsObject(threats_list);
		time_game.Free();

		goto play_again;
	}
	else
	{
	    time_game.Free();
		Mix_FreeChunk(soundEffect_);
		Mix_FreeMusic(music_);
		Mix_FreeMusic(game_over_);
		Destroy_ThreatsObject(threats_list);
		close();
	}
	return 0;
}


