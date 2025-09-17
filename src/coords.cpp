#include "coords.h"
#include "object.h"

float coords::getDistance(coords c2) {
	return std::sqrt(pow(c2.x - this->x, 2) + pow(c2.y - this->y, 2));
}

coords coords::getDirection(coords target) {
	double radians = atan2(target.y - this->y, target.x - this->x);
	double degrees = radians * (180.0 / M_PI);

	if (degrees <= 45 && degrees >= -45)						return {1, 0};  // RIGHT
	else if (degrees > 0 && degrees <= 135)						return {0, 1};  // DOWN
	else if (degrees > 0 && degrees <= 180 || degrees < -135)	return {-1, 0}; // LEFT
	else if (degrees <= -45)									return {0, -1}; // UP

	return {0, 0};
}

bool coords::operator==(const coords& other) const {
	return this->y == other.y && this->x == other.x;
}