#include "upgrade.h"

// ---------------- jumpBot ----------------

jumpBot::jumpBot(string name, const vector<vector<char>> &field)
    : GenericRobot(name, 0, 0), jumps(3) {
    robotType = "JumpBot";
}

void jumpBot::jump(int newX, int newY, const vector<vector<char>> &field) {
    if (jumps > 0) {
        if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) {
            PosX = newX;
            PosY = newY;
            jumps--;
            cout << "Jumped to (" << PosX << "," << PosY << ")" << endl;
        } else {
            cout << "Out of bounds! Cannot jump!" << endl;
        }
    }
}

void jumpBot::think(const vector<vector<char>> &field) {
    if (jumps > 0) {
        int newX = rand() % field.size();
        int newY = rand() % field[0].size();
        jump(newX, newY, field);
    } else {
        GenericRobot::think(field);
    }
}