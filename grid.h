#ifndef GRID_H
#define GRID_H

#pragma once
#include <vector>
using namespace std;

struct cell {
    int value;
    bool hidden{true};
    bool mine{false};
    bool giga_mine{false};  //Like 2 regular mines on top of each other. Meant to disorient and confuse the player.
    bool flag{false};
};

struct grid {
    int width, height;
    vector<vector<cell>> data;
    bool reveal_all = false; //If game ends shows all bombs
    int hidden;

    void reveal(vector<int>& location);
    void populate_bomb_area(int x, int y);
    void number_bombs();
    void init(int bombCount);
    bool random_number();
    void display() const;
    vector<int> locator(char &rida, char &veerg);
    bool onAMine(int x, int y) const;
    void plantAFlag(int x, int y);
    bool gameWon();
};

#endif //GRID_H
