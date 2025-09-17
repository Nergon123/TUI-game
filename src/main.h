#pragma once

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <math.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"
#define REVERSE "\033[7m"

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define SELECT 5

#define ENEMY_VISIBILITY_DISTANCE 10

#define _GROUND GREEN "#"
#define _food RED "*"

struct object ;
struct coords ;

void message(void*);
void Death(void*);
int controls(char);
std::vector<object*> findObjectsByType(int);
void updateWinsize();
void drawE(int, int);
void Update();
void updateCoords(int&, int&);

struct coords
{
	int x, y;
	coords(int _x, int _y) : x(_x), y(_y) {};
	coords() : x(0), y(0) {};
	float getDistance(coords c2) {
		return std::sqrt(pow(c2.x - this->x, 2) + pow(c2.y - this->y, 2));
	}

	coords getDirection(coords target) {
		double radians = atan2(target.y - this->y, target.x - this->x);
		double degrees = radians * (180.0 / M_PI);

		if (degrees <= 45 && degrees >= -45)						return {1, 0};  // RIGHT
		else if (degrees > 0 && degrees <= 135)						return {0, 1};  // DOWN
		else if (degrees > 0 && degrees <= 180 || degrees < -135)	return {-1, 0}; // LEFT
		else if (degrees <= -45)									return {0, -1}; // UP

		return {0, 0};
	}

	object *findObjectByCoords(coords offset) {
		this->x += offset.x;
		this->y += offset.y;
		if (this->x < 0 || this->y < 0) return &objects[1];

		bool main;
		for (object &obj : objects) {
			if (coords {this->x, this->y} == obj.xy) {
				if (&obj == &objects[0]) {
					main = true;
					continue;
				}
				return &obj;
			}
		}
		if (main) return &objects[0];

		//if (main) for (object &obj : objects) if (coordseq(obj.xy, xy)) return &obj;
		return nullptr;
	}

	bool operator==(const coords& other) const {
		return this->y == other.y && this->x == other.x;
	}
};

struct object
{
	coords xy;
	int type;
	int score;
	int hp;
	const char *symbol;
	bool static_;
	void (*callback)(void *param);
	void *params;
	object(coords _xy, int type, int _score, int _hp, const char *_symbol, bool _static, void (*_callback)(void *param), void *params)
		: xy(_xy), score(_score), type(type), hp(_hp), symbol(_symbol), static_(_static), callback(_callback), params(params) {};

	object() : xy(coords{0, 0}), score(0), hp(0), type(0), symbol(" "), static_(1), callback(nullptr), params(nullptr) {};
};