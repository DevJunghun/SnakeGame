#include "snakeGame.h"
#include <unistd.h>

using namespace std;

int keyPressed;

snakeGame::snakeGame() {
    snakeBody = 'O';
    wallBody = '*';
    snakeSize = 5;
    speed = 100000;
    direction = 'r';

    InitGameWindow();
    DrawWindow();
    DrawSnake();
    refresh();
}

snakeGame::~snakeGame() {
    nodelay(stdscr, false);
    getch();
    endwin();
}

void snakeGame::InitGameWindow() {
    initscr();
    nodelay(stdscr, TRUE);
    keypad(stdscr, true);                 
    noecho();                            
    curs_set(0);                          
    getmaxyx(stdscr, maxheight, maxwidth);
    return;
}

void snakeGame::DrawWindow() {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_GREEN);

    for (int i = 0; i < maxwidth; i++) {
        wall.push_back(Point(i, 0));
        attron(COLOR_PAIR(1));
        move(0, i);
        addch(wallBody);
        attroff(COLOR_PAIR(1));
    }

    for (int i = 0; i < maxwidth; i++) {
        wall.push_back(Point(i, maxheight - 1));
        attron(COLOR_PAIR(1));
        move(maxheight - 1, i);
        addch(wallBody);
        attroff(COLOR_PAIR(1));
    }

    for (int i = 0; i < maxheight; i++) {
        wall.push_back(Point(0, i));
        attron(COLOR_PAIR(1));
        move(i, 0);
        addch(wallBody);
        attroff(COLOR_PAIR(1));
    }

    for (int i = 0; i < maxheight; i++) {
        wall.push_back(Point(maxwidth - 1, i));
        attron(COLOR_PAIR(1));
        move(i, maxwidth - 1);
        addch(wallBody);
        attroff(COLOR_PAIR(1));
    }

    mvprintw(0, (maxwidth - 12) / 2, " Snake Game ");
    refresh();
}

void snakeGame::DrawSnake() {
    // 초기 snake 길이 5로 설정
    for (int i = 0; i < 5; i++)
        snake.push_back(Point(30 - i, 10));

    for (int i = 0; i < snake.size(); i++)
    {   
        move(snake[i].y, snake[i].x);
        addch(snakeBody);
    }
    return;
}

void snakeGame::MoveSnake() {
    keyPressed = getch();
    switch (keyPressed) {
    case KEY_LEFT:
        if (direction != 'r')
            direction = 'l';
        else
            GameOver();
        break;

    case KEY_RIGHT:
        if (direction != 'l')
            direction = 'r';
        else
            GameOver();
        break;

    case KEY_UP:
        if (direction != 'd')
            direction = 'u';
        else
            GameOver();
        break;

    case KEY_DOWN:
        if (direction != 'u')
            direction = 'd';
        else
            GameOver();
        break;
    }

    if (direction == 'l')
        snake.insert(snake.begin(), Point(snake[0].x - 1, snake[0].y));
    else if (direction == 'r')
        snake.insert(snake.begin(), Point(snake[0].x + 1, snake[0].y));
    else if (direction == 'u')
        snake.insert(snake.begin(), Point(snake[0].x, snake[0].y - 1));
    else if (direction == 'd')
        snake.insert(snake.begin(), Point(snake[0].x, snake[0].y + 1));
    
    move(snake[0].y, snake[0].x);
    addch(snakeBody);
    move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
    addch(' ');
    snake.pop_back();

    refresh();
    return;
}

bool snakeGame::GameOver() {
    // snake의 head가 벽에 닿으면 GameOver
    for (int i = 0; i < wall.size(); i++) {
        if (snake[0].x == wall[i].x && snake[0].y == wall[i].y)
            return true;
    }

    // snake의 head가 snake의 body에 닿으면 GameOver
    for (int i = 1; i < snake.size(); i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            return true;
    }

    // snake의 크기가 3보다 작으면 GameOver
    if (snake.size() < 3)
        return true;

    // snake의 진행 역방향으로 키를 입력하면 GameOver
    if (direction == 'r' && keyPressed == KEY_LEFT)
        return true;
    if (direction == 'l' && keyPressed == KEY_RIGHT)
        return true;
    if (direction == 'u' && keyPressed == KEY_DOWN)
        return true;
    if (direction == 'd' && keyPressed == KEY_UP)
        return true;

    return false;
}

void snakeGame::StartGame() {
    while (true) {
        if (GameOver()) {
            move((maxheight - 2) / 2, (maxwidth - 9) / 2);
            printw("GAME OVER");
            endwin();
            break;
        }

        MoveSnake();
        usleep(speed);
    }
}