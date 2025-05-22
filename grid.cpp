#include "grid.h"
#include <iostream>
#include <random>
#include <vector>
using namespace std;

/**
 * Avab vastavalt valitud alale ümbrust
 * @param location
 */
void grid::reveal(vector<int>& location) {
    int x = location[0];
    int y = location[1];

    // Kas positsioon on kaardil sees?
    if (x < 0 || x >= data.size() || y < 0 || y >= data[x].size()) {
        cout << "Valitud ala ei ole olemas!" << endl;
        return;
    }

    cell& current = data[x][y];

    // Ära ava juba avatud ruutu uuesti
    if (!current.hidden) return;

    current.hidden = false;

    // Kui on numberruut, siis ära levita edasi
    if (current.value > 0 || current.mine) return;

    // Suunad: 8 suunda (x, y) ümber
    const vector<pair<int, int>> directions = {
        {-1, -1}, {-1, 0}, {-1, 1}, // ülemine rida
        {0, -1},{0, 1},                    // vasak ja parem
        {1, -1},  {1, 0}, {1, 1}    //alumine rida
    };

    for (auto [dx, dy] : directions) {
        int nx = x + dx;
        int ny = y + dy;

        // Kontrolli, et naaber asub sees
        if (nx >= 0 && nx < data.size() &&
            ny >= 0 && ny < data[nx].size()) {

            cell& neighbor = data[nx][ny];

            // Kui pole miin ja peidus
            if (!neighbor.mine && neighbor.hidden) {
                if (neighbor.value == 0) {
                    vector<int> next = {nx, ny};
                    reveal(next); // rekursioon
                }
                neighbor.hidden = false;
            }
            }
    }
}
/**
 * Nummerdab pommi ümbritseva ala
 * @param x
 * @param y
 */
void grid::populate_bomb_area(int x, int y) {
    if (y-1 >= 0) {            //left of grid check
        data[x][y-1].value++; //left
    }
    if (y+1 < data[x].size()) {
        data[x][y+1].value++; //right
    }
    if (x+1 < data.size()) {                //bottom of grid check
        data[x+1][y].value++; //down
        if (y + 1 < data[x + 1].size()) {
            data[x + 1][y + 1].value++;// down left
        }
        if (y - 1 >= 0) {
            data[x + 1][y - 1].value++;// down right
        }
    }
    if (x-1 >= 0) {                         //top of grid check
        data[x-1][y].value++; //up
        if (y + 1 < data[x - 1].size()) {
            data[x - 1][y + 1].value++;// up left
        }
        if (y - 1 >= 0) {
            data[x - 1][y - 1].value++; // up right
        }
    }
}

/**
 * Käib läbi kõik pommid ja kasutab populate_bomb_area meetodit, et koik ümbrused nummerdada.
 */
void grid::number_bombs() {
    for (int x = 0; x < data.size(); x++) {
        for (int y = 0; y < data[x].size(); y++) {
            if (data[x][y].mine) {
                this->populate_bomb_area(x, y);
                if (data[x][y].giga_mine) {             //giga_mines show up as 2 mines on one tile but work the same.
                    this->populate_bomb_area(x, y);
                }
            }
        }
    }
};

/**
 * Loob mängu ala valitud pommide arvuga.
 * @param bombCount
 */
void grid::init(int bombCount) {
    int currentBombCount{0};
    for (int i = 0; i < height; i++) {
        vector<cell> row(width);
        data.push_back(row);
    }
    for (vector<cell>& row : data) {
        for (int j = 0; j < row.size(); j++) {
            if ( bombCount <= currentBombCount) {
                break;
            }
            if (random_number()) {
                currentBombCount++;
                row.at(j).mine = true;
                if (random_number()) {
                    row.at(j).giga_mine = true;
                }
            }

            if (j == row.size() -1 && bombCount != 10) {
                j = 0;
            }
        }
    }
}

/**
 * Genereerib suvalise arvu vahemikus 1- 100 ning kui arv on alla 20ne siis returnib true
 * @return boolean
 */
bool grid::random_number() {
    std::random_device seed_gen;
    std::mt19937 generator(seed_gen());
    std::uniform_int_distribution<> distribution(1, 100);
    int number = distribution(generator);
    if (number < 20){
        return true;
    }
    return false;
}

/**
 * Prindib hetkese mänguvälja ekraanile/konsooli
 */
void grid::display() const {
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
vector<int> grid::locator(char &rida, char &veerg) {
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
bool grid::onAMine(int x, int y) const {
    return data[x][y].mine;
}

/**
 * Püstitab/eemaldab lipu antud kohas
 * @param x
 * @param y
 */
void grid::plantAFlag(int x, int y) {
    data[x][y].flag = !data[x][y].flag;
}

/**
 * Käib läbi kõik cellid ja kontrollib, kas mäng on järsku võidetud.
 * @return boolean
 */
bool grid::gameWon() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (data[i][j].hidden) { // Kõik ruudud peavad avatud olema et võita (mis ei ole miinid), ehk kui leidub peidus olevaid numbri ruute, siis kohe false
                if (!data[i][j].mine) {
                    return false;
                }
            }
            if (data[i][j].mine && !data[i][j].flag) { // miin ja pole märgitud, siis ei ole võitnud veel
                return false;
            }
        }
    }
    return true;
}
