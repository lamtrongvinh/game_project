
#ifndef MAIN_OBJECT_H_
#define MAIN_OBJECT_H_

#include <vector>
#include "BaseObject.h"
#include "CommonFunction.h"
#include "BulletObject.h"
#include "game_music.h"


// Toc do roi
#define GRAVITI_SPEED 0.8
#define MAX_FALL_SPEED 10
#define PLAYER_SPEED 8
#define PLAYER_JUMP_VAL 18

class MainObject : public BaseObject
{
public:
	MainObject();
	~MainObject();

	enum WalkType
	{
		WALK_NONE = 0,
		WALK_RIGHT = 1,
		WALK_LEFT = 2
	};
	virtual bool loadImage(std::string path, SDL_Renderer * screen);
	void Show(SDL_Renderer * screen);
	void HandleInputAction(SDL_Event events, SDL_Renderer * screen, Mix_Chunk * soundEffect,  Mix_Chunk * player_move);
	void set_clips();

	void DoPlayer(Map& map_data, Mix_Chunk * music_, int& num_die, Mix_Chunk * music_die);

	void CheckToMap(Map& map_data, Mix_Chunk * music_, int& num_die, Mix_Chunk * music_die);

	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }

	void CenterEntityOnMap(Map& map_data);
	void UpdateImagePlayer(SDL_Renderer * des);

	void set_bullet_list(std::vector<BulletObject*> bullet_list_)
	{
		p_bullet_list_ = bullet_list_;
	}

	std::vector<BulletObject*> get_bullet_list() const {return p_bullet_list_;}
	SDL_Rect GetRectFrame();
	void HandleBullet(SDL_Renderer * des);
	void RemoveBullet(const int& index);
	void IncreaseCoin();
	int getCoin() const { return coin_count;}
	void setComeBackTime(const int cb_time_) { come_back_time_ = cb_time_;}
	bool getIsWin() const { return is_win_;}
private:

	bool is_win_;
	int coin_count;

	std::vector<BulletObject*> p_bullet_list_;

	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	// luu tru frame nhan vat
	SDL_Rect frame_clip_[8];

	Input input_type_;
	int frame_;
	int status_;
	bool on_ground_;

	int map_x_;
	int map_y_;

	// thoi gian tro lai khi bi roi xuong vuc
	int come_back_time_;

	// music
	Mix_Chunk* music_coin;
};

#endif