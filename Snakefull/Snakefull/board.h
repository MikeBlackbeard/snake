
#ifndef _BOARD_H
#define _BOARD_H

using namespace std;
enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };

class Fruit {

public:
	int posx;
	int posy;
	bool eat;
	int newtime;
	int eattime;
	int totaleat;
};


class MagicFruit {

public:
	int posx;
	int posy;
	bool eat;
	int newtime;
	int eattime;
	int totaleat;
    bool exist;
};


class Snake {
public:
	int posx[408];
	int posy[408];
	int size;
	int prex;
	int prey;
	
};


void getmov(Direction* direction, bool* gameover) {
    if (_kbhit()) {
        switch (_getch()) {
        case 'i':
        case 'I':
            if (*direction != DOWN)
                *direction = UP;
            break;
        case 'j':
        case 'J':
            if (*direction != RIGHT)
                *direction = LEFT;
            break;
        case 'k':
        case 'K':
            if (*direction != UP)
                *direction = DOWN;
            break;
        case 'l':
        case 'L':
            if (*direction != LEFT)
                *direction = RIGHT;
            break;
        case 'q':
        case 'Q':
            *gameover = true;
            break;
        case 'p':
        case 'P':
            *direction = STOP;
            break;
        default:
            break;
        }
    }
}


void move(Direction* direction, Snake* snake) {
    switch (*direction)
    {
    case STOP:
        break;
    case LEFT:
        snake->posx[0] --;
        break;
    case RIGHT:
        snake->posx[0] ++;
        break;
    case UP:
        snake->posy[0] --;
        break;
    case DOWN:
        snake->posy[0] ++;
        break;
    default:
        break;
    }
}


void movesnake(Snake* snake) {
    int tempx, tempy;
    for (int i = 1; i < snake->size; i++) {
        tempx = snake->posx[i];
        tempy = snake->posy[i];
        snake->posx[i] = snake->prex;
        snake->posy[i] = snake->prey;
        snake->prex = tempx;
        snake->prey = tempy;
    }
}
#endif // !_BOARD_H
