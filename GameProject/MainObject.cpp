
#include "stdafx.h"
#include "MainObject.h"

MainObject::MainObject ()
{
	frame_ = 0;
	x_pos_ = 0;
	y_pos_ = 0;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;
	input_type_.left_ = 0 ; 
	input_type_.right_ = 0;
	input_type_.jump_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
	on_ground_ = false;
	map_x_ = 0;
	map_y_ = 0;
	come_back_time_ = 0;
	coin_count = 0;
	is_win_ = false;
	music_coin = NULL;
}

MainObject::~MainObject()
{
	
}



bool MainObject::loadImage(std::string path, SDL_Renderer * screen)
{
	bool ret = BaseObject::loadImage(path, screen);

	if (ret == true)
	{
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;
	}

	return ret;
}

void MainObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0 )
	{
		
		// cach set 2
		for (int i = 0; i < 8; i ++)
		{
			 frame_clip_[i].x = i * width_frame_;
			 frame_clip_[i].y = 0;
			 frame_clip_[i].w = width_frame_;
			 frame_clip_[i].h = height_frame_;
		}

	}
}

void MainObject::Show(SDL_Renderer * des)
{
	UpdateImagePlayer(des);
	if (on_ground_ == true)
	{
		if (status_ == WALK_LEFT )
		{
			loadImage("img//player_left.png", des);
		} else {
			loadImage("img//player_right.png", des);
		}
	}

	if (input_type_.left_ == 1 || input_type_.right_ == 1)
	{
		frame_++;
	} else {
		frame_ = 0 ;
	}

	if (frame_ >= 8)
	{
		frame_ = 0;
	}

	if (come_back_time_ == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect * current_clip = &frame_clip_[frame_];
		SDL_Rect renderQuad = {rect_.x, rect_.y, width_frame_, height_frame_};

		SDL_RenderCopy(des, p_object, current_clip ,&renderQuad);
	}
	
}

void MainObject::HandleInputAction(SDL_Event event, SDL_Renderer * screen, Mix_Chunk * soundEffect, Mix_Chunk * player_move)
{
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_RIGHT:
			{
				status_ = WALK_RIGHT;
				input_type_.right_ = 1;
				input_type_.left_ = 0;
				Mix_PlayChannel(-1, player_move, 0);
				UpdateImagePlayer(screen);
				break;
			}
			case SDLK_LEFT:
			{
				status_ = WALK_LEFT;
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				UpdateImagePlayer(screen);
				Mix_PlayChannel(-1, player_move, 0);
				break;
			}
			//jump
			case SDLK_UP:
			{
				input_type_.jump_ = 1;
				break;
			}

			
		default:
			break;
		}
	} else if (event.type == SDL_KEYUP)
	{
		switch (event.key.keysym.sym)
		{
			case SDLK_RIGHT:
			{
				input_type_.right_ = 0;
				break;
			}
			case SDLK_LEFT:
			{
				input_type_.left_ = 0;
				break;
			}
			//jump
			case SDLK_UP:
			{
				input_type_.jump_ = 0;
				break;
			}
			
		}
	}

	//MOUSEBUTTON
	

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			input_type_.jump_ = 1;
		} else if (event.button.button == SDL_BUTTON_LEFT)
		{
			BulletObject* p_bullet  = new BulletObject();
			p_bullet->set_bullet_type(BulletObject::SPHERE_BULLET);
			p_bullet->LoadImgBullet(screen);
			Mix_PlayChannel(-1, soundEffect, 0);
	        int xp = x_pos_ - map_x_;
			int yp = y_pos_ - map_y_;
			if (status_ == WALK_LEFT)
			{
				p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
				p_bullet->SetRect(xp, yp +height_frame_*0.25);
			}
			else if (status_ == WALK_RIGHT)
			{
				p_bullet->set_bullet_dir(BulletObject::DIR_RIGHT);
				p_bullet->SetRect(xp + width_frame_ - 20, yp +height_frame_*0.25);
			}

			p_bullet->set_x_val(20);
			p_bullet->set_y_val(20);
			p_bullet->set_is_move(true);

			p_bullet_list_.push_back(p_bullet);
		}
	}
}

void MainObject::HandleBullet(SDL_Renderer * des)
{
	for (int i = 0 ; i < p_bullet_list_.size(); i++)
	{
		BulletObject * p_bullet = p_bullet_list_.at(i);
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move() == true)
			{
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_bullet->Render(des);
			} else {
				p_bullet_list_.erase(p_bullet_list_.begin() + i);
				if (p_bullet != NULL)
				{
					delete p_bullet;
				}
				
				p_bullet = NULL;
			}
		}
	}
}

void MainObject::RemoveBullet(const int& index)
{
	int size = p_bullet_list_.size();
	if (size > 0 && index < size)
	{
		BulletObject* p_bullet = p_bullet_list_.at(index);
		p_bullet_list_.erase(p_bullet_list_.begin() + index);

		if (p_bullet != NULL)
		{
			delete p_bullet;
			p_bullet =NULL;
		}
	}
}

SDL_Rect MainObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;

	return rect;
}


void MainObject::DoPlayer(Map& map_data, Mix_Chunk * music_, int& num_die, Mix_Chunk * music_die)
{
	if (come_back_time_ == 0)
	{
		x_val_ = 0;
		y_val_ += GRAVITI_SPEED;
		if ( y_val_ >= MAX_FALL_SPEED )
		{
			y_val_ = MAX_FALL_SPEED;
		}

		if (input_type_.left_ == 1)
		{
			x_val_ -= PLAYER_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += PLAYER_SPEED;
		}

		if (input_type_.jump_ == 1)
		{
			if (on_ground_ == true)
			{
				y_val_  = - PLAYER_JUMP_VAL;
			}

			on_ground_ = false;
			input_type_.jump_ = 0;
		}

		CheckToMap(map_data, music_, num_die, music_die);
		CenterEntityOnMap(map_data);
	}

	if (come_back_time_ > 0)
	{

		come_back_time_--;

		if (come_back_time_ == 0)
		{
			
			on_ground_ == false;
			if (x_pos_ > 256)
			{
				// lui lai 4 o
				x_pos_ -= 256;
			} else {
				x_pos_ = 0;
			}
			y_pos_ = 0;
			x_val_ = 0;
			y_val_ = 0;
		}
	}

}

void MainObject::CenterEntityOnMap(Map& map_data)
{
	map_data.start_x_  = x_pos_ - (SCREEN_WIDTH / 2  );
	if (map_data.start_x_ < 0)
	{
		map_data.start_x_ = 0;
	} else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
	{
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
	}

	map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);
	if (map_data.start_y_ < 0)
	{
		map_data.start_y_ = 0;
	} else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
	{
		map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
	}
}

// kiem tra di chuyen cua nhan vat voi map
void MainObject::CheckToMap(Map& map_data, Mix_Chunk * music_, int& num_die, Mix_Chunk * music_die)
{
	// gioi han kiem tra di chuyen theo OX
	int x1 = 0;
	int x2 = 0;
	// gioi han kiem tra di chuyen theo OY
	int y1 = 0;
	int y2 = 0;

	// check theo OX
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

	// xac dinh vi tri o thu may
	x1 = (x_pos_  + x_val_) / TILE_SIZE;
	// so sanh kich thuoc
	x2 = (x_pos_ + x_val_ + width_frame_  - 1) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_frame_ -1 ) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (x_val_ > 0) // di chuyen sang ben phai
		{
			int val_1 = map_data.tile[y1][x2];
			int val_2 = map_data.tile[y2][x2];

			if (val_1 == STATE_COIN || val_2 == STATE_COIN)
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				Mix_PlayChannel(-1, music_, 0);
				IncreaseCoin(); 
			} else 
			{
				if (map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
				{
					x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ +1;
					x_val_ = 0;
				}
			}
			
		} else if (x_val_ < 0 )
		{
			int val_1 = map_data.tile[y1][x1];
			int val_2 = map_data.tile[y2][x1];

			if (val_1 == STATE_COIN || val_2 == STATE_COIN)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y2][x1] = 0;
				Mix_PlayChannel(-1, music_, 0);
				IncreaseCoin(); 
			} else {
				if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1]  != BLANK_TILE)
				{
					x_pos_ =  (x1 + 1 ) * TILE_SIZE;
					x_val_ = 0;
				}
			}
			
		}
	}

	// check OY
	int width_min = width_frame_ > TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = (x_pos_ ) / TILE_SIZE;
	x2 = (x_pos_ + width_frame_ ) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ -1 ) / TILE_SIZE;


	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >=0 && y2 < MAX_MAP_Y )
	{

		if (y_val_ > 0 )
		{
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];

			if (val1 == STATE_COIN || val2 == STATE_COIN)
			{
				map_data.tile[y2][x1] = 0;
				map_data.tile[y2][x2] = 0;
				Mix_PlayChannel(-1, music_, 0);
				IncreaseCoin();

			} else 
			{
				if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
				{
					y_pos_ = y2 * TILE_SIZE;
					y_pos_ -= ( height_frame_  + 1 );
					y_val_ = 0;

					on_ground_ = true;
				
					if (status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
					}
				}
			}
			
		} else if (y_val_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];
			if (val1 == STATE_COIN || val2 == STATE_COIN)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				Mix_PlayChannel(-1, music_, 0);
				IncreaseCoin();
			} else 
			{
				if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
				{
					y_pos_ = (y1 + 1 ) * TILE_SIZE;
					y_val_ = 0;
				}
			}
			
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0 )
	{
		x_pos_ = 0;
	} else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}


	if (y_pos_ > map_data.max_y_)
	{
		come_back_time_ = 60;
		num_die--;
		Mix_PlayChannel(-1 ,music_die, 0);
	}	

	// is_win
	if (x_pos_ + width_frame_ > map_data.max_x_ - TILE_SIZE)
	{
		is_win_ = true;
	}
}

void MainObject::IncreaseCoin()
{
	coin_count++;
}


void MainObject::UpdateImagePlayer(SDL_Renderer * des)
{
	if (on_ground_ == true)
	{
		if (status_ == WALK_LEFT)
		{
			loadImage("img//player_left.png", des);
		}
		else if (status_ == WALK_RIGHT)
		{
			loadImage("img//player_right.png", des);
		}	
	} else {
		if (status_ == WALK_LEFT)
		{
			loadImage("img//jum_left.png", des);
		}
		else if (status_ == WALK_RIGHT)
		{
			loadImage("img//jum_right.png", des);
		}
	}
}