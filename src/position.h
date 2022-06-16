#include <ncurses.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <string>
#include <vector>

using namespace std;

class position{ 
public:
    int x;
    int y;
    position();
    position(int tempx, int tempy);
    position& randomPosition();

    position& operator=(const position& v);
    bool operator==(const position v);
};
