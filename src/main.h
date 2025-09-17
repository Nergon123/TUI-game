#pragma once

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <math.h>

#include "coords.h"
#include "object.h"
#include "colors.h"

#define LEFT 1
#define RIGHT 2
#define UP 3
#define DOWN 4
#define SELECT 5

#define ENEMY_VISIBILITY_DISTANCE 10

#define _GROUND GREEN "#"
#define _food RED "*"

int controls(char);
void updateWinsize();
void drawE(int, int);
void Update();
void updateCoords(int&, int&);
