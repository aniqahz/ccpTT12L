#include "upgrade.h"
#include "battlefield.h"

// ---------------- jumpBot ----------------

jumpBot::jumpBot(string name, const vector<vector<char>> &field)
    : GenericRobot(name, 0, 0), jumps(3) {
    robotType = "JumpBot";
}

void jumpBot::setPosition(int x, int y) {
    PosX = x;
    PosY = y;
}

void jumpBot::jump(int newX, int newY, const vector<vector<char>> &field, ofstream& outfile) {
    logStatus(outfile);
    if (jumps > 0) {
        if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) {
            PosX = newX;
            PosY = newY;
            jumps--;
            log(cout,outfile, "Jumped to (" + to_string(PosX) + "," + to_string(PosY) + ")" );
        } else {
            outfile << "Out of bounds! Cannot jump!" << endl;
        }
    } else {
        outfile << "No jumps left!" << endl;
    }
}

void jumpBot::think(vector<vector<char>> &field, ofstream& outfile) {
    if (jumps > 0) {
        int newX = rand() % field.size();
        int newY = rand() % field[0].size();
        jump(newX, newY, field, outfile);
    } else {
        outfile << "JumpBot has no jumps left. Reverting to default behavior." << endl;
        GenericRobot::think(field, outfile);
    }
}

// ---------------- thirtyShotBot ----------------
thirtyShotBot::thirtyShotBot(string name, const vector<vector<char>>& field)
    : GenericRobot(name, 0, 0){
    robotType = "ThirtyShotBot";
    shells = 30;
}

void thirtyShotBot::setPosition(int x, int y){
    PosX = x;
    PosY = y;
}

void thirtyShotBot::think(vector<vector<char>>& field, ofstream& outfile) {
    outfile << name << " has 30 shells and is thinking..." << endl;
    outfile << name << " (ThirtyShotBot) has " << shells << " shells." << endl;
    logStatus(outfile);
    GenericRobot::think(field, outfile);
}

// ---------------- scoutBot ----------------
scoutBot::scoutBot(string name, const vector<vector<char>>& field)
    : GenericRobot(name, 0, 0), scanUses(3) {
    robotType = "ScoutBot";
}

void scoutBot::setPosition(int x, int y) {
    PosX = x;
    PosY = y;
}

void scoutBot::think(vector<vector<char>>& field, ofstream& outfile) {
    logStatus(outfile);
    outfile << name << " has " << scanUses << " scout scan(s) remaining." << endl;

    if (scanUses > 0) {
        outfile << name << " uses full-field scan (ScoutBot ability)!" << endl;
        for (const auto& row : field) {
            string line(row.begin(), row.end());
            outfile << line << endl;
        }
        scanUses--;
    } else {
        outfile << name << " has no scans left. Thinking normally." << endl;
    }

    GenericRobot::think(field, outfile);
}

// ---------------- trackBot ----------------
trackBot::trackBot(string name, const vector<vector<char>>& field)
    : GenericRobot(name, 0, 0), trackers(3) {
    robotType = "TrackBot";
}

void trackBot::setPosition(int x, int y) {
    PosX = x;
    PosY = y;
}

void trackBot::think(vector<vector<char>>& field, ofstream& outfile) {
    logStatus(outfile);
    outfile << name << " has " << trackers << " tracker(s) remaining." << endl;

    if (trackers > 0) {
        for (GenericRobot* other : activeRobots) {
            if (other == this || !other->getAliveStatus()) continue;
            auto [x, y] = other->getPosition();

            // Can see 1-tile radius
            if (abs(PosX - x) <= 1 && abs(PosY - y) <= 1) {
                trackers--;
                trackedEnemies.push_back({ other->getName(), {x, y} });
                outfile << name << " has tracked " << other->getName() << " at (" << x << "," << y << ")" << endl;
                break;
            }
        }
    }

    else {outfile << name << " has no trackers left." << endl;}

    // Always log known tracked enemy positions
    for (auto& [trackedName, pos] : trackedEnemies) {
        outfile << name << " sees tracked " << trackedName << " at (" << pos.first << "," << pos.second << ")" << endl;
    }

    GenericRobot::think(field, outfile);
}

