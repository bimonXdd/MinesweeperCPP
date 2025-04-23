#include <iostream>
#include <vector>
#include <random>
using namespace std;
#ifndef GRID_H
#define GRID_H

struct cell {
    int value;
    bool hidden{true};
    bool mine{false};
    bool flag{false};
};

struct grid {
    int width, height;
    vector<vector<cell>> data;
    bool reveal_all = false; //If game ends shows all bombs
    // populates the game grid with just zeros
    void reveal(vector<int>& location) {
        int x = location[0]; // -up / +down
        int y = location[1]; // -left / +right
        //selection inside grid check
        if (x >= 0 && x < data.size() && y >= 0 && y < data[x].size()) {
            data[x][y].hidden = false;
        } else {
            cout << "Valitud ala ei ole olemas!" << endl;
            return;
        }

        cell& right_from_selected = data[x][y+1];
        cell& left_from_selected = data[x][y-1];
        cell& down_from_selected = data[x+1][y];
        cell& up_from_selected = data[x-1][y];
        cell& up_right_from_selected = data[x-1][y+1];
        cell& up_left_from_selected = data[x-1][y-1];
        cell& down_right_from_selected = data[x+1][y+1];
        cell& down_left_from_selected = data[x+1][y-1];

        if (y-1 > 0) {              //left of grid check
            if (!left_from_selected.mine) left_from_selected.hidden = false;
        }
        if (y+1 < data[x].size()) {
            if (!right_from_selected.mine) right_from_selected.hidden = false; //right
        }
        if (x+1 < data.size()) {                //bottom of grid check
            if (!down_from_selected.mine) down_from_selected.hidden = false; //down

            //so tiles wouldn't be revealed between mines also numbers
            if (!(down_from_selected.mine && down_right_from_selected.mine)) {
                if (!down_right_from_selected.mine) down_right_from_selected.hidden = false; //down right
            }
            if (!(down_from_selected.mine && down_left_from_selected.mine)) {
                if (!down_left_from_selected.mine) down_left_from_selected.hidden = false; //down left
            }
        }
        if (x-1 >= 0) {                         //top of grid check
            if (!up_from_selected.mine) up_from_selected.hidden = false; //up

            //so tiles wouldn't be revealed between mines
            if (!(up_from_selected.mine && right_from_selected.mine)) {
                if (!up_right_from_selected.mine) up_right_from_selected.hidden = false; //up right
            }
            if (!(up_from_selected.mine && left_from_selected.mine)) {
                if (!up_left_from_selected.mine) up_left_from_selected.hidden = false; //up left
            }
        }


    }

    /**
     * Nummerdab pommi ümbritseva ala
     * @param x
     * @param y
     */
    void populate_bomb_area(int x, int y) {
        if (y-1 > 0) {              //left of grid check
            data[x][y-1].value++; //left
        }
        if (y+1 < data[x].size()) {
            data[x][y+1].value++; //right
        }
        if (x+1 < data.size()) {                //bottom of grid check
            data[x+1][y].value++; //down
            data[x+1][y+1].value++; //down right
            data[x+1][y-1].value++; //down left
        }
        if (x-1 >= 0) {                         //top of grid check
            data[x-1][y].value++; //up
            data[x-1][y+1].value++; //up right
            data[x-1][y-1].value++; //up left
        }
    }

    /**
     * Käib läbi kõik pommid ja kasutab populate_bomb_area meetodit, et koik ümbrused nummerdada.
     */
    void number_bombs() {
        for (int x = 0; x < data.size(); x++) {
            for (int y = 0; y < data[x].size(); y++) {
                if (data[x][y].mine) {
                    this->populate_bomb_area(x, y);
                }
            }
        }
    }


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
                if (suvaline_arv()) {
                    currentBombCount++;
                    row.at(j).mine = true;
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
    * Prindib hetkese mänguvälja
    */
    void display() const {
        char tahed[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
        cout << "  A B C D E F G H I J" << endl;
        for (int i = 0; i < height; i++) {
            cout << tahed[i] << ' ';
            for (int j = 0; j < width; j++) {
                if (data[i][j].hidden && data[i][j].mine && reveal_all) {
                    cout << "$ ";
                } else if (data[i][j].hidden && !data[i][j].flag) {
                    cout << "# ";
                } else if (data[i][j].hidden && data[i][j].flag) {
                    cout << "% ";
                }
                else{cout << data[i][j].value << " ";}
            }
            cout << endl;
        }
    }

    /**
     * Tagastab vastavalt tähtedele valitud indexi
     * @param rida
     * @param veerg
     * @return index
     */
    vector<int> lokaator(char &rida, char &veerg) {
        char tahed[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
        vector<int> selected(2);
        int rea_nr = 0, veeru_nr = 0;
        for (int i = 0; i < 10; i++) {
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

    /**
     * Check if selected location is a mine
     * @param x
     * @param y
     * @return boolean
     */
    bool onAMine(int x, int y) const {
            return data[x][y].mine;
        }

    /**
     * Püstitab/eemaldab lipu antud kohas
     * @param x
     * @param y
     */
    void plantAFlag(int x, int y) {
        data[x][y].flag = !data[x][y].flag;
    }
    };

#endif //GRID_H
