#include "snakeGame.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <unistd.h>	//usleep(microseconds);
using namespace std;

extern int map[5][40][60];
extern int map[5][40][60];
extern void appearposion(int stage_num, WINDOW *win1);
extern void appeargrowth(int stage_num, WINDOW *win1);
extern void disappearPoison(int stage_num, WINDOW *win1);
extern void disappeargrowth(int stage_num, WINDOW *win1);
extern void crushItem(WINDOW *win1);
extern vector<Vector> minusSnake(WINDOW *win1);
extern position getHead();
extern vector<position> vpoison_item;
extern vector<position> vgrow_item;

char missionB = 'X'; 
char missionGrowth = 'X';
char missionPoison = 'X';
char missionGate = 'X';
string whyDead = "";  

int num_missionB = 6; 
int num_missionGrowth = 2;
int num_missionPoison = 2;
int num_missionGate = 1;



void newWindow(float y, float x){ 
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, y, x);
}


int UserInput(){ 
    int UserInput = getch();
    refresh();
    endwin();
    clear();
    return UserInput;
}


int startGame(float y, float x) {
    newWindow(y,x);
    printw("Do you want to start snake game? (y/n)");
    return UserInput();
}


int finishWindow(float y, float x){ 
    newWindow(y,x);
    printw(whyDead.data());
    printw("\n \nYou Dead! Press any button to finish~");
    return UserInput();
}
 

void drawGameMap(WINDOW* win, Snake& snake, char* table, int row, int col) //맵 그리기
{
	werase(win);

	for(int i=0; i<(row*col); i++)
	{
		if(table[i]!=' ')
		{
			int y = i / col;
			int x = i - (y*col);
			int ch;
			switch(table[i])
			{
				case '0': 
                    ch = ' ';
                    break;
                case '1':
                    ch = '*';
                    break;
                case '2':
                    ch = '+';
                    break;
                case '4':
                    ch = '@';
                    break;
				case 'h':
					ch = '#';
					break;
				case 'b':
					ch = '#';
					break;
				case '8': 
					ch = '1';
					break;
				case '9': 
					ch = '2';
					break;
				case '3':
                    ch = '0';
                    break;
                case '6':
                    ch = 'x';
                    break;
			}
			mvwaddch(win, y, x, ch);
		}
	}
	wrefresh(win);
}


void updateMap(Snake& snake, int map[40][60]) { 
	snake.setGate(map); 
}


void printScoreBoard(WINDOW* w, int snakeLen, int level, int growthItem, int poisonItem, int Gate){
	werase(w);
	wbkgd(w, COLOR_PAIR(level));
	wborder(w, '|','|','~','~','.','.','.','.');
	mvwprintw(w, 1, 1, "Score Board");
  mvwprintw(w, 2, 1, "B:(CurrentLength)/(Max  Length) %d/%d", snakeLen, snakeMaxLen);
  mvwprintw(w, 3, 1, "+:(GrowthItems Num): %d ", growthItem);
  mvwprintw(w, 4, 1, "-:(PoisonItems Num): %d ", poisonItem);
  mvwprintw(w, 5, 1, "G(Number of Entered Gate) : %d ", Gate);
  mvwprintw(w, 6, 1, "Level : %d ", level);
	wrefresh(w);
}


void printMission(WINDOW* w, int level){
  werase(w);
  wbkgd(w, COLOR_PAIR(level));
  wborder(w, '|','|','~','~','.','.','.','.');
  mvwprintw(w, 1, 1, "Mission");
  mvwprintw(w, 2, 1, "B: 6 ( %c )", missionB);
  mvwprintw(w, 3, 1, "+: 2  ( %c )", missionGrowth);
  mvwprintw(w, 4, 1, "-: 2  ( %c )", missionPoison);
  mvwprintw(w, 5, 1, "G: 1  ( %c )" ,missionGate);
  wrefresh(w);
}


int noticeChangeLevel(float y, float x, int level){ 
  clear();
  initscr();
  noecho();
 
  getmaxyx(stdscr, y, x);
  newWindow(y,x);
  if (level==1){
    printw("Let's start Game! \n Level 1");
  }
  else{
    string lev = to_string(level);
    printw("Let's go to next level! \n");
    printw(lev.data());
    printw("\nPress Enter button~!");
  }
  return UserInput();
}


void setMission(Snake& snake, WINDOW *win1){
  if(vgrow_item.empty() ==0){
    position head = snake.getHead();
    if(head == vgrow_item.back()){
      snake.crushItem(win1);
      snake.changeSnakeLen();
      snake.growthItem++;
    }
  }
  if(vpoison_item.empty() ==0){
    position head = snake.getHead();
    if(head == vpoison_item.back()){
      snake.minusSnake(win1);
      snake.changeSnakeLen();
      snake.poisonItem++;
    }
  }
  if(snake.getSize() == num_missionB) {missionB ='O';}
  if(snake.growthItem == num_missionGrowth) {missionGrowth = 'O';}
  if(snake.poisonItem == num_missionPoison) {missionPoison = 'O';}
  if(snake.getGateCnt() == num_missionGate) {missionGate = 'O';}
}


void nextLevel(Snake& snake,WINDOW *win1){
  if((missionB == 'O')&&(missionGate=='O')&&(missionGrowth=='O')&&(missionPoison=='O')){
    snake.resize(3);
    snake.growthItem =0;
    snake.poisonItem =0;
    snake.setGateCnt(0);
    missionB = 'X'; 
    missionGrowth = 'X';
    missionPoison = 'X';
    missionGate = 'X';
	disappeargrowth(snake.getLevel()-1,win1);
	disappearPoison(snake.getLevel()-1,win1);
    snake.setLevel(snake.getLevel()+1);
    if (noticeChangeLevel(0,0, snake.getLevel()) == 13) {}; 
  }
}


void game() { 

	float x, y;
	initscr();
	noecho();
	cbreak();

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_CYAN);
	init_pair(2, COLOR_WHITE, COLOR_GREEN);
	init_pair(3, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(4, COLOR_BLACK, COLOR_YELLOW);
	init_pair(5, COLOR_BLACK, COLOR_BLUE);

	getmaxyx(stdscr, y, x);
	WINDOW *win1 = newwin(40, 60, 0, 0); 
  WINDOW *scoreBoard = newwin(10, 40, 0,60);
  wrefresh(scoreBoard);


  WINDOW *mission = newwin(10, 40, 12,60);
  wrefresh(mission);

	Snake snake(40, 60);
	
	int growCnt = 0;
	int poisonCnt = 0;
	int mapCnt = 0;

	while(!snake.getEnd())
	{
	WINDOW *win1 = newwin(40, 60, 0, 0);
	printScoreBoard(scoreBoard, snake.getSnakeLen(), snake.getLevel(), snake.growthItem, snake.poisonItem, snake.getGateCnt());
	printMission(mission, snake.getLevel());

	srand(time(NULL));
	char *map_table = snake.setMaptoList(map[snake.getLevel()-1]);
	wbkgd(win1, COLOR_PAIR(snake.getLevel()));
	wattron(win1, COLOR_PAIR(snake.getLevel()));
	nodelay(win1, TRUE);
	keypad(win1, TRUE);
	refresh();

	wrefresh(win1);
		drawGameMap(win1, snake, map_table, snake.getRow(), snake.getCol()); 
    setMission(snake,win1);
		if (mapCnt == 0) {
			(snake, map[snake.getLevel()-1]);
		}
		mapCnt+= 1;
		if (mapCnt == 100) {
			snake.removeGate(map[snake.getLevel()-1]);
			updateMap(snake, map[snake.getLevel()-1]);
			mapCnt = 1;
		}
		if (growCnt == 0) {
			appeargrowth(snake.getLevel()-1,win1);
		}
		growCnt+= 1;
		if (growCnt == 77) { 
			disappeargrowth(snake.getLevel()-1,win1);
			appeargrowth(snake.getLevel()-1,win1);
			growCnt = 1;
		}
		if (poisonCnt == 0) {
			appearposion(snake.getLevel()-1,win1);

		}
		poisonCnt+= 1;
		if (poisonCnt == 67) { 
			disappearPoison(snake.getLevel()-1,win1);
			appearposion(snake.getLevel()-1,win1);
			poisonCnt = 1;
		}

		int input = wgetch(win1); 
		char d = snake.getDirection(); 
		switch(input)
		{
			case 'w':
			case KEY_UP: 
				if(d!='u' && d!='d') snake.setDirection(0);
				else if (d=='d') {
          snake.setEnd(true); 
          whyDead = "You've moved in the tail direction! You must move towards head.";
        }
				break;
			case 's':
			case KEY_DOWN: 
				if(d!='d' && d!='u') snake.setDirection(2);
				else if (d=='u') {
          snake.setEnd(true);
          whyDead = "You've moved in the tail direction! You must move towards head.";
        }
				break;
			case 'a':
			case KEY_LEFT: 
				if(d!='l' && d!='r') snake.setDirection(3);
				else if (d=='r') {
          snake.setEnd(true);
          whyDead = "You've moved in the tail direction! You must move towards head.";
        }
				break;
			case 'd':
			case KEY_RIGHT: 
				if(d!='r' && d!='l') snake.setDirection(1);
				else if (d=='l') {
          snake.setEnd(true);
          whyDead = "You've moved in the tail direction! You must move towards head.";
        }
				break;
			case 'r' :
			case 'R' :
				snake.setEnd(true);
				snake.removeGate(map[snake.getLevel()]);
				disappeargrowth(snake.getLevel()-1,win1);
				disappearPoison(snake.getLevel()-1,win1);
				game();
		}
		if(snake.getSize() <3) {
      snake.setEnd(TRUE); 
      whyDead = "Snake length is less than 3!";
    }

		snake.moveSnakeBody(); 
		snake.moveSnakeHead(map[snake.getLevel()-1]); 
    nextLevel(snake, win1); 
		usleep(snake.getSpeed()); 
	}
}


int main(){
	if (startGame(0, 0) == 'y') {
			game();
		}
  finishWindow(0, 0);
	endwin();
	return 0;
}