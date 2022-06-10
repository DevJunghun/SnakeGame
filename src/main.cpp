#include "snakeGame.h"

void SetScene(double, double);
int ReadyScene();

int maxheight, maxwidth;

void SetScene(double x, double y) {
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, maxheight, maxwidth);
    move(maxheight / y, maxwidth / x);
}

int ReadyScene() {   
    SetScene(4, 2.5);
    printw("Do you want to play Snake Game? (y / n)");

    int userInput = getch();
    refresh();
    endwin();
    clear();

    return userInput;
}

int main() {
    if (ReadyScene() == 'y') {
        snakeGame SnakeGame;
        SnakeGame.StartGame();
    }
    return 0;
}