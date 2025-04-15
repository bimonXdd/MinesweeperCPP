#include <iostream>
#include <vector>
using namespace std;
#ifndef GRID_H
#define GRID_H

struct grid {
    int width, height;
    vector<vector<int>> data;
    // populates the game grid with just zeros
    void init() {
        for (int i = 0; i < height; i++) {
            vector<int> row(width, 0);
            data.push_back(row);
        }
    }

    /*
    * Prints the current grid
    */
    void display() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cout << data[i][j] << " ";
            }
            cout << endl;
        }
    }
};

#endif //GRID_H
