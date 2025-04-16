#include <iostream>
#include <vector>
#include <array>
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
                srand(time(0)+j);
                int rng = rand()%101;
                cout<<rng<<"\n";

                if (rng < 20) {
                    currentBombCount++;
                    row.at(j).bomb = true;
                }
            }
        }
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
        for (int i = 0; i < 10; i++) {
            if (rida == tahed[i]) {
                selected.insert(selected.begin(), i);
            }
            if (veerg == tahed[i]) {
                selected.insert(selected.begin()+1, i);
            }
        }
        cout << selected[0] << ' ' << selected[1] << endl;
        return selected;
    }
};

#endif //GRID_H
