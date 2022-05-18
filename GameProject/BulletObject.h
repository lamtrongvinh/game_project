#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include "BaseObject.h"
#include "CommonFunction.h"

class BulletObject : public BaseObject 
{
public:
	BulletObject();
	~BulletObject();

	enum Bullet_Dir
	{
		DIR_RIGHT = 20,
		DIR_LEFT = 21, 
		DIR_UP = 22,
		DIR_UP_LEFT = 23,
		DIR_UP_RIGHT = 24,
		DIR_DOWN_LEFT = 25,
		DIR_DOWN_RIGHT = 26
	};

	enum BulletType 
	{
		SPHERE_BULLET  = 50,
		LASER_BULLET = 51
	};

	void set_x_val(const int& x_val) {x_val_ = x_val;}
	void set_y_val(const int& y_val) {y_val_ = y_val;}
	int get_x_val() const {return x_val_;}
	int get_y_val() const {return y_val_;}

	void set_is_move(const bool is_move) {is_move_ = is_move;}
	bool get_is_move() const {return is_move_;}

	void set_bullet_dir(const unsigned int& bullet_dir ) {bullet_dir_ = bullet_dir;}
	int get_bullet_dir() const {return bullet_dir_;}

	void set_bullet_type(const unsigned int& bullet_type) {bullet_type_ = bullet_type;}
	unsigned int get_bullet_type() const {return bullet_type_;}

	void HandleMove(const int& x_border, const int& y_border);
	bool LoadImgBullet(SDL_Renderer * des);

private:
	int x_val_;
	int y_val_;
	bool is_move_;

	unsigned int bullet_dir_;
	unsigned int bullet_type_;
};

#endif