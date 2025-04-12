using namespace std;
#include "grid.h";
#include <iostream>;

int main(int argc, char *argv[]) {
    grid game_area;
    game_area.width = 10;
    game_area.height = 7;
    game_area.init();
    game_area.display();
    return 0;

}
