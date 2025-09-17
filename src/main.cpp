#include "main.h"

winsize w;

int hp = 1000;
int score = 0;
coords _offset = {0, 0};
coords minoffset = {-5, -5};
coords maxoffset = {5, 5};

void (**current_callback)(void *param) = nullptr;
void *current_param = nullptr;

ulong step = 0;

object objects[] = {
	{{5, 5}, 0, 0, 1000, MAGENTA "e", false, nullptr, nullptr},
	{{-1, -1}, 0, 0, 0, WHITE "#", true, nullptr, nullptr},
	{{25, 5}, 0, 0, 0, CYAN "T", true, message, (void *)"Sign"},
	{{25, 10}, 1, 0, 0, RED "W", false, Death, nullptr},
};

int main() {
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	int x = 0;
	int y = 0;
	printf("%d %d %d %d ", x, y, w.ws_col, w.ws_row);
	for (;;) updateCoords(objects[0].xy.x, objects[0].xy.y);

	return 0;
}

void message(void *param) {
	//horrible TODO Divide into functions
	if (param) {
		int a = 0;
		//int n = 0;
		for (char u = 255; u != '\0'; a++) u = ((const char *)(param))[a];//if (u == '\n')n++;
		system("clear");

		printf(WHITE "\n");

		for (int i = 0; i < (w.ws_col / 2 - a / 2)-1; i++) printf(" ");
		for (int i = 0; i < a+1; i++) printf("=");

		printf("\n");

		for (int i = 0; i < w.ws_col / 2 - a / 2; i++) printf(" ");

		printf("%s\n", (const char *)param);

		for (int i = 0; i < (w.ws_col / 2 - a / 2)-1; i++) printf(" ");
		for (int i = 0; i < a+1; i++) printf("=");

		//fgetc(stdin);
	}
}

void Death(void *a) {
	printf("gameover");
	exit(0);
}

int controls(char c) {
	switch (c)
	{
		case 'w': return UP;
		case 'a': return LEFT;
		case 's': return DOWN;
		case 'd': return RIGHT;
		case 'e': return SELECT;
		default: return 0;
	}
}

std::vector<object *> findObjectsByType(int type) {
	std::vector<object *> __objects;
	for (object &obj : objects) if (obj.type == type) __objects.push_back(&obj);
	return __objects;
}

void updateWinsize() {
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
}

void drawE(int _x, int _y) {
	if (sizeof(objects) == 0) return;
	system("clear");

	printf(WHITE "SCORE %d HP %d POS X%d Y%d OFFSET X%d Y%d\n ",
		objects[0].score,
		objects[0].hp,
		objects[0].xy.x,
		objects[0].xy.y,
		_offset.x,
		_offset.y);
	
	for (int y = 0; y < w.ws_row - 2; y++) {
		for (int x = 0; x < w.ws_col; x++) {
			object *current = coords {x, y}.findObjectByCoords(_offset);

			if (current) {
				if (current->xy == objects[0].xy) {
					(current_callback) = &current->callback;
					current_param = current->params;
				}
				printf("%s", current->symbol);
			}
			else printf(" ");
		}
	}
}

void Update() {
	step++;
	std::vector<object *> _objects = findObjectsByType(1);
	for (object *obj : _objects) {
		if ( step % 2 == 0 && obj->xy.getDistance(objects[0].xy) < ENEMY_VISIBILITY_DISTANCE ) {
			coords dir = obj->xy.getDirection(objects[0].xy);
			obj->xy.x += dir.x;
			obj->xy.y += dir.y;
		}
	}
}
void updateCoords(int &x, int &y) {
	bool do_draw = false;

	char c;
	c = fgetc_unlocked(stdin);
	int ctrl = controls(c);
	switch (ctrl) {
		case DOWN:
			do_draw = true;
			if (y - w.ws_row < 5) _offset.y = y - 5;
			if (y < w.ws_row) y++;
			break;
		case UP:
			do_draw = true;
			if (y - w.ws_row < 5) _offset.y = y - 5;
			if (y > 0) y--;
			break;
		case LEFT:
			do_draw = true;
			if (x + _offset.x < 5) _offset.x = x - 5;
			if (x > 0) x--;
			break;
		case RIGHT:
			do_draw = true;
			if (x + _offset.x < 5) _offset.x = x - 5;
			if (x < w.ws_col) x++;
			break;
	}
	if (do_draw) {
		Update();
		updateWinsize();
		drawE(x, y);
		if (current_callback && *current_callback) (*current_callback)(current_param);
	}
}