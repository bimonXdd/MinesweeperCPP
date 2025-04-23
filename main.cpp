using namespace std;
#include "grid.h";
#include <iostream>;

int main(int argc, char *argv[]) {
    grid game_area;
    game_area.width = 10;
    game_area.height = 7;
    game_area.init(10);
    game_area.display();
    while (true) {
        string input;
        char rida;
        char veerg;

        cout << "Rida:";
        cin >> rida;
        cout << "Veerg:";
        cin >> veerg;
        vector<int>selected_location = game_area.lokaator(rida, veerg);
        game_area.reveal(selected_location);
        game_area.display();
        if (input == "exit") {
            return 0;
        }
    }
}
