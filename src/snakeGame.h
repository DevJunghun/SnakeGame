#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>

#ifndef SNAKEGAME_H
#define SNAKEGAME_H

using namespace std;

struct Point {
    int x, y;
    Point() : x(0), y(0) { };
    Point(int x, int y) : x(x), y(y) { };
};

class snakeGame {
private:
    char snakeBody, wallBody, direction;
    int maxwidth, maxheight, snakeSize, speed;

    vector<Point> snake;   
    vector<Point> wall;

    void InitGameWindow();
    void DrawWindow();
    void DrawSnake();
    void MoveSnake();
    bool GameOver();

public:
    snakeGame();
    ~snakeGame();
    void StartGame();
};

#endif