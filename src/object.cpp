#include "object.h"

std::vector<object> objects = {
	{ {5,	5},		OBJ_PLAYER,	0, 1000,	MAGENTA "e",	false,	nullptr, nullptr		},
	{ {-1,	-1},	OBJ_WALL,	0, 0,		WHITE "#",		true,	nullptr, nullptr		},
	{ {25,	5},		OBJ_SIGN,	0, 0,		CYAN "T",		true,	message, (void *)"Sign"	},
	{ {25, 	10},	OBJ_ENEMY,	0, 0,		RED "W",		false,	Death,   nullptr		}
};
   
object *findObjectByCoords(coords xy, coords offset) {
	xy.x += offset.x;
	xy.y += offset.y;
	if (xy.x < 0 || xy.y < 0) return &objects[1];

	bool main;
	for (object &obj : objects) {
		if (coords{xy.x, xy.y} == obj.xy) {
			if (&obj == &objects[0]) {
				main = true;
				continue;
			}
			return &obj;
		}
	}
	//if (main) return &objects[0];

	if (main) for (object &obj : objects) if (obj.xy==xy) return &obj;
	return nullptr;
}
