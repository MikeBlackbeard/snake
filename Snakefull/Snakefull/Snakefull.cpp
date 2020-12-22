#include <conio.h>
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <thread>
#include <vector>
#include <stdio.h>
#include "board.h"

using namespace std;


unsigned int screenWidth = 120;
unsigned int screenHeight = 80;
unsigned int fieldWidth = 26;
unsigned int fieldHeight = 18;
unsigned char* pField = nullptr;
wstring map[];


void newmagic(MagicFruit* fruit, Snake* snake) {
    bool free = true;
    int prex, prey;
    do {
        free = true;
        prey = (rand() % (fieldHeight - 2)) + 1;
        prex = (rand() % (fieldWidth - 2)) + 1;
        for (int i = 0; i < snake->size; i++) {
            if ((prex == snake->posx[i]) && (prey == snake->posy[i])) {
                free = false;
                break;
            }
        }
        if (free) {
            fruit->posx = prex;
            fruit->posy = prey;
            fruit->newtime = time(0) - 1;
            fruit->exist = true;
        }
    } while (!free);
    fruit->eat = false;
}

void logic(Snake* snake, bool* gameover, Fruit* fruit, int* score, MagicFruit* magic) {
    if ((snake->posx[0] == 0) || (snake->posy[0] == 0) || (snake->posy[0] == fieldHeight - 1) || (snake->posx[0] == fieldWidth - 1))
        *gameover = true;
    if ((fruit->posx == snake->posx[0]) && (fruit->posy == snake->posy[0])) {
        fruit->eat = true;
        fruit->eattime = time(0);
        snake->size++;
        int elapsed = 5 - (fruit->eattime - fruit->newtime);
        *score += 10;
        if (elapsed > 0)
            *score += elapsed;
        fruit->totaleat++;
        if ((fruit->totaleat % 5) == 0)
            newmagic(magic, snake);
    }
    if (magic->exist) {
        /*int thistime = time(0);
        int magicelapsed = (thistime - magic->newtime);*/
        if ((5 - (time(0) - magic->newtime)) < 0) {
            magic->exist = false;
            fruit->totaleat = 0;
        }
        if (magic->exist) {
            if ((magic->posx == snake->posx[0]) && (magic->posy == snake->posy[0])) {
                *score += 30;
                magic->exist = false;
                fruit->totaleat = 0;
            }
        }
    
    }
    for (int i = 1; i < snake->size; i++) {
        if ((snake->posx[i] == snake->posx[0]) && (snake->posy[i] == snake->posy[0]))
            *gameover = true;
    }
}

void newfruit(Fruit* fruit, Snake* snake) {
    bool free = true;
    int prex, prey;
    do {
        free = true;
        prey = (rand() % (fieldHeight - 2)) + 1;
        prex = (rand() % (fieldWidth - 2)) + 1;
        for (int i = 0; i < snake->size; i++) {
            if ((prex == snake->posx[i]) && (prey == snake->posy[i])) {
                free = false;
                break;
            }
        }
        if (free) {
            fruit->posx = prex;
            fruit->posy = prey;
            fruit->newtime = time(0);
        }
    } while (!free);
    fruit->eat = false;
}




int main()
{
    bool gameover = false;
    unsigned int fruitx, fruity;
    Snake snake;
    Direction direction = STOP;
    snake.posx[0] = fieldWidth / 2;
    snake.posy[0] = fieldHeight / 2;
    snake.size = 1;
    int score = 0;
    MagicFruit magic;
    magic.exist = false;
    
    // Initializate rand

    srand(time(0));

    // Create screen buffer ---------------------------------------------------------------------------
    wchar_t* screen = new wchar_t[screenHeight * screenWidth];
    for (int i = 0; i < screenWidth * screenHeight; i++) screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBitesWritten = 0;

    pField = new unsigned char[(screenWidth + 4) * 2 * screenHeight]; // Create play field buffer
    

    Fruit fruit;
    newfruit(&fruit, &snake);


//    Board board;
//    board.play();

    while (!gameover) {

        snake.prex = snake.posx[0];
        snake.prey = snake.posy[0];
        getmov(&direction, &gameover);
        move(&direction, &snake);
        logic(&snake, &gameover, &fruit, &score, &magic);
        if (gameover) {
            snake.posx[0] = snake.prex;
            snake.posy[0] = snake.prey;
        }
        if (fruit.eat)
            newfruit(&fruit, &snake);
        movesnake(&snake);
        
        for (int x = 0; x < fieldWidth*2; x++) // Board Boundary
            for (int y = 0; y < fieldHeight; y++)
                pField[y * fieldWidth*2 + x] = (x == 2 || y == 0 || x == (fieldWidth*2 - 1) || y == (fieldHeight - 1)) ? 1 : 0;


        // Draw Field
        for (int x = 2; x < fieldWidth*2; x++)
            for (int y = 0; y < fieldHeight; y++)
                screen[(y + 2) * screenWidth + (x + 2)] = L" #"[pField[y * fieldWidth*2 + x]];

        // Draw fruit

        screen[(fruit.posy + 2) * screenWidth + (fruit.posx + 2)*2] = 'X';

        // Draw Magic fruit

        if (magic.exist)
            screen[(magic.posy + 2) * screenWidth + (magic.posx + 2)*2] = '&';

        // Draw snake

        for (int i = 0; i < snake.size; i++)
            screen[(snake.posy[i] + 2 ) * screenWidth + (snake.posx[i] + 2)*2] = 'O';

        // Draw Score
        swprintf_s(&screen[20 * screenWidth + 16], 16, L"SCORE: %8d", score);




        // Display Frame
        WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBitesWritten);

        
        Sleep(200);
     

    }

    bool exit = false;
    while (!exit) {

        for (int x = 0; x < fieldWidth * 2; x++) // Board Boundary
            for (int y = 0; y < fieldHeight; y++)
                pField[y * fieldWidth * 2 + x] = (x == 2 || y == 0 || x == (fieldWidth * 2 - 1) || y == (fieldHeight - 1)) ? 1 : 0;


        // Draw Field
        for (int x = 2; x < fieldWidth * 2; x++)
            for (int y = 0; y < fieldHeight; y++)
                screen[(y + 2) * screenWidth + (x + 2)] = L" #"[pField[y * fieldWidth * 2 + x]];

        // Draw fruit

        screen[(fruit.posy + 2) * screenWidth + (fruit.posx + 2) * 2] = 'X';

        // Draw snake

        for (int i = 0; i < snake.size; i++)
            screen[(snake.posy[i] + 2) * screenWidth + (snake.posx[i] + 2) * 2] = 'X';

        // Draw Score
        swprintf_s(&screen[20 * screenWidth + 16], 16, L"SCORE: %8d", score);
        swprintf_s(&screen[21 * screenWidth + 4], 30, L"GAME OVER - PRESS 'A' TO EXIT", score);
        

        // Display Frame
        WriteConsoleOutputCharacter(hConsole, screen, screenWidth * screenHeight, { 0,0 }, &dwBitesWritten);

        if (_kbhit)
            switch (_getch())
            {
            case 'a':
            case 'A':
                exit = true;
                break;
            default:
                break;
            }

  
    }

    
}



