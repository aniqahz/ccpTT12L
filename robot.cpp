#include "robot.h"

// ---------------- baseRobot ----------------

baseRobot::baseRobot() : PosX(0), PosY(0), isAlive(true), remainingLives(3) {}

baseRobot::baseRobot(int x, int y) : PosX(x), PosY(y), isAlive(true), remainingLives(3) {}

pair<int, int> baseRobot::getPosition() const { return {PosX, PosY}; }

void baseRobot::setPosition(int x, int y) { PosX = x; PosY = y; }

bool baseRobot::getAliveStatus() const { return isAlive; }

string baseRobot::getRobotType() const { return robotType; }

// ---------------- Capabilities ----------------

movingRobot::movingRobot(int x, int y) : baseRobot(x, y) {}
shootingRobot::shootingRobot(int x, int y) : baseRobot(x, y) {}
lookRobot::lookRobot(int x, int y) : baseRobot(x, y) {}
thinkingRobot::thinkingRobot(int x, int y) : baseRobot(x, y) {}

// ---------------- GenericRobot ----------------

GenericRobot::GenericRobot(string robotName, int x, int y)
    : baseRobot(x, y), movingRobot(x, y), shootingRobot(x, y), lookRobot(x, y), thinkingRobot(x, y),
      name(robotName), shells(10), lives(3), maxLives(3), upgradesUsed(0),
      hasMovingUpgrade(false), hasShootingUpgrade(false), hasSeeingUpgrade(false) {
    robotType = "GenericRobot";
    isAlive = true;
    PosX = x;
    PosY = y;
}

void GenericRobot::think(const vector<vector<char>> &field) {
    int action = rand() % 5;
    int dx = rand() % 3 - 1;
    int dy = rand() % 3 - 1;
    switch (action) {
        case 0: cout << "Moving to a new position..." << endl; break;
        case 1: cout << "DECISION : FIRE!..." << endl; fire(dx, dy, field); break;
        case 2: cout << "Looking around for enemies..." << endl; look(PosX, PosY, field); break;
        case 3: cout << "Moving..." << endl; move(dx, dy, field); break;
        case 4: cout << "Staying still..." << endl; break;
    }
}

void GenericRobot::look(int dx, int dy, const vector<vector<char>> &field) {
    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j) {
            int newX = PosX + i;
            int newY = PosY + j;
            if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) {
                if (field[newX][newY] == '.')
                    cout << "Coast is clear!" << endl;
                else
                    cout << "Enemy detected!" << endl;
            } else {
                cout << "Out of bounds..." << endl;
            }
        }
}

void GenericRobot::fire(int dx, int dy, const vector<vector<char>> &field) {
    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j) {
            int targetX = PosX + i;
            int targetY = PosY + j;
            if (shells > 0) {
                shells--;
                int probability = rand() % 100;
                if (targetX >= 0 && targetX < field.size() && targetY >= 0 && targetY < field[0].size()) {
                    if (field[targetX][targetY] == '.') {
                        if (probability >= 70)
                            cout << "KaBoom at (" << targetX << "," << targetY << ")!" << endl;
                        else
                            cout << "Missed at (" << targetX << "," << targetY << ")!" << endl;
                    }
                }
            } else {
                cout << "Out of shells! Self-destructing!" << endl;
            }
        }
}

void GenericRobot::move(int dx, int dy, const vector<vector<char>> &field) {
    int newX = PosX + dx;
    int newY = PosY + dy;
    if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) {
        PosX = newX;
        PosY = newY;
        cout << "Moved to (" << PosX << "," << PosY << ")" << endl;
    } else {
        cout << "Cannot move out of bounds!" << endl;
    }
}

