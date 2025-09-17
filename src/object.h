#pragma once
#include <vector>
#include "coords.h"
#include "colors.h"
#include "callbacks.h"
struct object
{
	//friend coords;
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
    void findObjectByCoords(coords xy, coords offset) ;
};
object *findObjectByCoords(coords xy, coords offset);
extern std::vector<object> objects;