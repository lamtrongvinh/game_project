
#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_

#include "CommonFunction.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define THREATS_FRAME_NUM 8
#define THREATS_GRAVITI_SPEED 0.8
#define THREATS_MAX_FALL_SPEED 10
#define THREAT_SPEED 3

class ThreatsObject : public BaseObject
{
public:
	ThreatsObject();
	~ThreatsObject();

	enum TypeMove {
		STATIC_THREATS = 0, 
		MOVE_IN_SPACE_THREATS = 1
	};

	void set_x_val(const float& x_val) { x_val_ = x_val;}
	void set_y_val(const float& y_val) { y_val_ = y_val;}
	void set_x_pos(const float& x_pos) {x_pos_ = x_pos;}
	void set_y_pos(const float& y_pos) {y_pos_ = y_pos;}
	float get_x_pos() const { return x_pos_;}
	float get_y_pos() const { return y_pos_;}
	
	void SetMapXY(const int& map_x, const int& map_y) {map_x_ = map_x; map_y_ = map_y;}

	void InitThreat();

	void set_clips();
	bool loadImg(std::string path, SDL_Renderer * screen);
	void Show(SDL_Renderer * des);
	int get_width_frame() const { return width_frame_;}
	int get_height_frame() const { return height_frame_;}
	void DoPlayer(Map& gMap);
	void CheckToMap(Map& map_data);

	void set_type_move(const int& type_move ) { type_move_ = type_move;}
	void SetAnimationPos(const int& pos_a, const int& pos_b) { animation_a_ = pos_a; animatin_b_ = pos_b;}
	void set_input_left(const int& ipLeft) { input_type_.left_ =  ipLeft;}
	
	void ImpMoveType(SDL_Renderer * screen);

	std::vector<BulletObject*> get_bullet_list() const { return bullet_list_;}
	void set_bullet_list(const std::vector<BulletObject*>& bullet_list ) { bullet_list_ = bullet_list; }
	void InitBullet(BulletObject* p_bullet, SDL_Renderer * screen);
	void MakeBullet(SDL_Renderer * screen, const int& x_limit_, const int& y_limit_);
	SDL_Rect GetRectFrame();
	void RemoveBullet(const int& index);
private:
	SDL_Rect frame_clip_[THREATS_FRAME_NUM];
	int frame_;
	int width_frame_;
	int height_frame_;

	bool on_ground_;
	int come_back_time_;
	float x_pos_;
	float y_pos_;
	float x_val_;
	float y_val_;
	int map_x_;
	int map_y_;

	int type_move_;
	int animation_a_;
	int animatin_b_;
	Input input_type_;

	std::vector<BulletObject*> bullet_list_;

};

#endif