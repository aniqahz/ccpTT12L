#include "upgrade.h"
#include "battlefield.h"

// JUMP BOT
jumpBot::jumpBot(string name, int x, int y)
    : GenericRobot(name, x, y), jumps(3) {
    robotType = "JumpBot";
}

void jumpBot::jump(int newX, int newY, const vector<vector<char>>& field, ofstream& outfile) {
    if (jumps > 0) {
        if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) {
            setPosition(newX, newY);
            jumps--;
            log(cout, outfile, name + " jumped to (" + to_string(PosX) + "," + to_string(PosY) + ")");
        } else {
            log(cout, outfile, name + " tried to jump out of bounds.");
        }
    } else {
        log(cout, outfile, name + " has no jumps left.");
    }
}

void jumpBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    if (jumps > 0) {
        int newX = rand() % field.size();
        int newY = rand() % field[0].size();
        jump(newX, newY, field, outfile);
    } else {
        GenericRobot::think(field, robots, outfile);
    }
}

void jumpBot::revert() {
    this->reset();
}

// THIRTY SHOT BOT
thirtyShotBot::thirtyShotBot(string name, int x, int y)
    : GenericRobot(name, x, y) {
    robotType = "ThirtyShotBot";
    setShells(30);
}

void thirtyShotBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    log(cout, outfile, name + " (ThirtyShotBot) has " + to_string(getShells()) + " shells.");
    GenericRobot::think(field, robots, outfile);
}

void thirtyShotBot::revert() {
    this->reset();
}

// SCOUT BOT
scoutBot::scoutBot(string name, int x, int y)
    : GenericRobot(name, x, y), scanUses(3) {
    robotType = "ScoutBot";
}

void scoutBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    if (scanUses > 0) {
        log(cout, outfile, name + " uses full-field scan (ScoutBot).");
        for (const auto& row : field) {
            string line(row.begin(), row.end());
            log(cout, outfile, line);
        }
        scanUses--;
    } else {
        log(cout, outfile, name + " has no scans left.");
    }
    GenericRobot::think(field, robots, outfile);
}

void scoutBot::revert() {
    this->reset();
}

// TRACK BOT
trackBot::trackBot(string name, int x, int y)
    : GenericRobot(name, x, y), trackers(3) {
    robotType = "TrackBot";
}

void trackBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    if (trackers > 0) {
        for (GenericRobot* other : robots) {
            if (other && other != this && other->getAliveStatus()) {
                auto [x, y] = other->getPosition();
                if (abs(PosX - x) <= 1 && abs(PosY - y) <= 1) {
                    trackers--;
                    trackedEnemies.push_back({ other->getRobotName(), {x, y} });
                    log(cout, outfile, name + " tracked " + other->getRobotName() + " at (" + to_string(x) + "," + to_string(y) + ")");
                    break;
                }
            }
        }
    }

    for (const auto& [trackedName, pos] : trackedEnemies) {
        log(cout, outfile, name + " sees tracked " + trackedName + " at (" + to_string(pos.first) + "," + to_string(pos.second) + ")");
    }

    GenericRobot::think(field, robots, outfile);
}

void trackBot::revert() {
    this->reset();
}
