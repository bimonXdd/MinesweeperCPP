using namespace std;
#include "grid.h"
#include <iostream>

int main(int argc, char *argv[]) {
    cout << "------ Minesweeper --" << endl;
    cout << "Et m2ng v6ita, leia k6ik miinid ja ava suurem osa kaardist!" << endl;
    cout << endl;
    grid game_area;
    game_area.width = 10;
    game_area.height = 7;
    game_area.init(10);
    game_area.number_bombs();
    game_area.display();

    while (true) {
        string input;

        cout << "Vali tegevus: 1-Avasta, 2-M2rgi/eemalda miin, 3-Lahku m2ngust: ";
        cin >> input;

        if (input == "3") {
            return 0;
        }
        char rida;
        char veerg;
        cout << "Rida(t2ht):";
        cin >> rida;
        cout << "Veerg(t2ht):";
        cin >> veerg;
        vector<int>selected_location = game_area.lokaator(rida, veerg);

        //avastamine
        if (input == "1") {
            game_area.reveal(selected_location);
            if (game_area.onAMine(selected_location[0], selected_location[1])) {
                srand(time(0));
                int tykki = rand()%101;
                cout << "Astusid miini otsa ja lendasid " << tykki << " tykiks :(" << endl;
                game_area.reveal_all = true;
                game_area.display();
                return 0;
            }
            game_area.display();
        }
        if (input == "2") {
            game_area.plantAFlag(selected_location[0], selected_location[1]);
            game_area.display();
        }
    }
}
