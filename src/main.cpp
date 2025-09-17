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

#define _GROUND GREEN "#"
#define _food RED "*"
winsize w;
struct coords
{
    int x, y;
    coords(int _x, int _y) : x(_x), y(_y) {};
    coords() : x(0), y(0) {};
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
        : xy(_xy), score(_score), type(type),
          hp(_hp), symbol(_symbol), static_(_static), callback(_callback), params(params) {};
    object()
        : xy(coords{0, 0}), score(0), hp(0), type(0), symbol(" "), static_(1), callback(nullptr), params(nullptr) {};
};

int hp = 1000;
int score = 0;
coords _offset = {0, 0};
coords minoffset = {-5, -5};
coords maxoffset = {5, 5};

void (**current_callback)(void *param) = nullptr;
void *current_param = nullptr;

ulong step = 0;

void message(void *param)
{

    //horrible TODO Divide into functions
    if (param)
    {
        int a = 0;
        int n = 0;
        for (char u = 255; u != '\0'; a++)
        {
            u = ((const char *)(param))[a];
            if (u == '\n')
            {
                n++;
            }
        }
        system("clear");
        printf(WHITE "\n");
        for (int i = 0; i < (w.ws_col / 2 - a / 2)-1; i++)
        {
            printf(" ");
        }
        for (int i = 0; i < a+1; i++)
        {
            printf("=");
        }
        printf("\n");
        for (int i = 0; i < w.ws_col / 2 - a / 2; i++)
        {
            printf(" ");
        }

            printf("%s\n", (const char *)param);
        for (int i = 0; i < (w.ws_col / 2 - a / 2)-1; i++)
        {
            printf(" ");
        }
        for (int i = 0; i < a+1; i++)
        {
            printf("=");
        }
        fgetc(stdin);
    }
}
void Death(void *a)
{
    exit(0);
}
object objects[] = {
    {{5, 5}, 0, 0, 1000, MAGENTA "e", false, nullptr, nullptr},
    {{-1, -1}, 0, 0, 0, WHITE "#", true, nullptr, nullptr},
    {{25, 5}, 0, 0, 0, CYAN "T", true, message, (void *)"Sign"},
    {{25, 10}, 1, 0, 0, RED "W", false, Death, nullptr},
};

#define maxheight 9

int controls(char c)
{
    switch (c)
    {
    case 'w':
        return UP;
    case 'a':
        return LEFT;
    case 's':
        return DOWN;
    case 'd':
        return RIGHT;
    case 'e':
        return SELECT;
    }
    return 0;
}
coords getDirection(coords source, coords target)
{
    double radians = atan2(target.y - source.y, target.x - source.x);
    double degrees = radians * (180.0 / M_PI);

    if (degrees <= 45 && degrees >= -45)
    {
        return {1, 0}; // RIGHT
    }
    else if (degrees > 0 && degrees <= 135)
    {
        return {0, 1}; // DOW
    }
    else if (degrees > 0 && degrees <= 180 || degrees < -135)
    {
        return {-1, 0}; // LEFT
    }
    else if (degrees <= -45)
    {
        return {0, -1}; // UP
    }
    return {0, 0};
}
float getDistance(coords c1, coords c2)
{
    return std::sqrt(pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2));
}
bool coordseq(coords c1, coords c2)
{
    return c1.x == c2.x && c1.y == c2.y;
}
object *findObjectByCoords(coords xy, coords offset)
{
    xy.x += offset.x;
    xy.y += offset.y;
    if (xy.x < 0 || xy.y < 0)
        return &objects[1];

    bool main;
    for (object &obj : objects)
    {
        if (coordseq(obj.xy, xy))
        {
            if (&obj == &objects[0])
            {
                main = true;
                continue;
            }
            return &obj;
        }
    }
    if (main)
    {
        for (object &obj : objects)
        {
            if (coordseq(obj.xy, xy))
            {
                return &obj;
            }
        }
    }
    return nullptr;
}

std::vector<object *> findObjectsByType(int type)
{
    std::vector<object *> __objects;
    for (object &obj : objects)
    {
        if (obj.type == type)
        {
            __objects.push_back(&obj);
        }
    }
    return __objects;
}
void updateWinsize(winsize &w)
{
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
}

void drawE(int _x, int _y, winsize w)
{
    if (sizeof(objects) == 0)
        return;
    system("clear");

    printf(WHITE "SCORE %d HP %d POS X%d Y%d OFFSET X%d Y%d\n ", objects[0].score, objects[0].hp, objects[0].xy.x, objects[0].xy.y, _offset.x, _offset.y);
    for (int y = 0; y < w.ws_row - 2; y++)
    {
        for (int x = 0; x < w.ws_col; x++)
        {
            object *current = findObjectByCoords({x, y}, _offset);

            if (current)
            {
                if (coordseq(current->xy, objects[0].xy))
                {

                    (current_callback) = &current->callback;
                    current_param = current->params;
                }
                printf("%s", current->symbol);
            }
            else
            {
                printf(" ");
            }
        }
    }
}
void Update()
{

    step++;
    std::vector<object *> _objects = findObjectsByType(1);
    for (object *obj : _objects)
    {
        if (obj->type == 1)
        {
            if (step % 2 == 0)
                if (getDistance(obj->xy, objects[0].xy) < 10)
                {
                    coords dir = getDirection(obj->xy, objects[0].xy);
                    obj->xy.x += dir.x;
                    obj->xy.y += dir.y;
                };
        }
    }
}
void updateCoords(int &x, int &y, winsize &w)
{
    bool b = false;

    char c;
    c = fgetwc_unlocked(stdin);
    while (!c)
        ;
    int ctrl = controls(c);
    switch (ctrl)
    {
    case SELECT:

        break;
    case DOWN:
        b = 1;
        if (y - w.ws_row < 5)
        {
            _offset.y = y - 5;
        }
        if (y < w.ws_row)
        {
            y++;
        }
        break;
    case UP:
        b = 1;
        if (y - w.ws_row < 5)
        {
            _offset.y = y - 5;
        }
        if (y > 0)
        {
            y--;
        }
        break;
    case LEFT:
        b = 1;
        if (x + _offset.x < 5)
        {
            _offset.x = x - 5;
        }
        if (x > 0)
        {
            x--;
        }
        break;
    case RIGHT:
        b = 1;
        if (x + _offset.x < 5)
        {
            _offset.x = x - 5;
        }
        if (x < w.ws_col)
        {
            x++;
        }
        break;
    default:
        break;
    }
    if (b)
    {
        Update();
        updateWinsize(w);
        drawE(x, y, w);
        if (current_callback && *current_callback)
        {
            (*current_callback)(current_param);
        }
    }
}

int main()
{

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int x = 0;
    int y = 0;
    printf("%d %d %d %d ", x, y, w.ws_col, w.ws_row);
    for (;;)
    {
        updateCoords(objects[0].xy.x, objects[0].xy.y, w);
    }
}