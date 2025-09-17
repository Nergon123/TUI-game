#pragma once

struct coords
{
	int x, y;
	coords(int _x, int _y) : x(_x), y(_y) {};
	coords() : x(0), y(0) {};
	float getDistance(coords c2);
	coords getDirection(coords target);
	bool operator==(const coords& other) const;
};