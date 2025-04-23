#include <iostream>
#include <vector>
#include <array>
#include <random>
using namespace std;
#ifndef GRID_H
#define GRID_H

struct cell {
    int value;
    bool hidden{true};
    bool bomb{false};
};

struct grid {
    int width, height;
    vector<vector<cell>> data;
    // populates the game grid with just zeros
    void init(int bombCount) {
        int currentBombCount{0};
        for (int i = 0; i < height; i++) {
            vector<cell> row(width, cell(0));
            data.push_back(row);
        }
        for (vector<cell>& row : data) {
            for (int j = 0; j < row.size(); j++) {
                if ( bombCount <= currentBombCount) {
                    break;
                }
                //TODO pommid suvalisemalt ja et loopib niikaua kuni pomme on piisavalt.

                if (suvaline_arv()) {
                    currentBombCount++;
                    row.at(j).bomb = true;
                }
                if (j == row.size() -1 && bombCount != 10) {
                    j = 0;
                }
            }
        }
    }

    // Genereerib suvalise arvu vahemikus 1- 100 ning kui arv on alla 20ne siis returnib true
    bool suvaline_arv() {
        std::random_device seed_gen;
        std::mt19937 generator(seed_gen());
        std::uniform_int_distribution<> distribution(1, 100);
        int number = distribution(generator);
        if (number < 20){
            return true;
        }
        return false;
    }

        /*
        * Prints the current grid
        */
        void display() {
            char tahed[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
            cout << "  A B C D E F G H I J" << endl;
            for (int i = 0; i < height; i++) {
                cout << tahed[i] << ' ';
                for (int j = 0; j < width; j++) {
                    cout <<  data.at(i).at(j).bomb;
                    if (data[i][j].hidden && data[i][j].bomb) {
                        cout << "$ ";
                    } else if (data[i][j].hidden) {
                        cout << "# ";
                    }
                    else{cout << data[i][j].value << " ";}
                }
                cout << endl;
            }
        }

        vector<int> lokaator(char &rida, char &veerg) {
            char tahed[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
            vector<int> selected(2);
            int rea_nr = 0, veeru_nr = 0;
            for (int i = 0; i < 10; i++) {
                //G  = 6 ja D = 4
                if (rida == tahed[i]) {
                    rea_nr = i;
                }
                if (veerg == tahed[i]) {
                    veeru_nr = i;
                }
            }
            selected.insert(selected.begin(),rea_nr);
            selected.insert(selected.begin() + 1, veeru_nr);

            cout << selected[0] << ' ' << selected[1] << endl;
            return selected;
        }
    };

#endif //GRID_H
