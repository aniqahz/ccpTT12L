/**********|**********|**********|
Program: main.cpp
Course: Object Oriented Programming and Data Structures
Trimester: 2510
Name: ANIQAH NABILAH BINTI AZHAR | JASMYNE YAP | 
NUR ALEEZ DANIA BINTI MOHD SHAHRUL AZMAN | WAN HANANI IMAN BINTI WAN MOHD AZIDI @ SAPAWI
ID: 242UC244LQ | 242UC244PT | 242UC244QB | 242UC244CK
Lecture Section: TC3L
Tutorial Section: T11L
Email: aniqah.nabilah.azhar@student.mmu.edu.my | jasmyne.yap@student.mmu.edu.my |
nur.aleez.dania@student.mmu.edu.my | wan.hanani.iman@student.mmu.edu.my
Phone: 011-6204 6219 | 011-6346 4323 | 019-7109905 | 019-966 0664
**********|**********|**********/

#include "robot.h"
#include "upgrade.h"
#include "battlefield.h"

// JUMPBOT-----------------------------------------------------------------------------------
JumpBot::JumpBot(string name, int x, int y)
    : GenericRobot(name, x, y), jumps(3) {
    robotType = "JumpBot";
    hasMovingUpgrade = true;
    upgradeActive = true; // Activate the upgrade
}

void JumpBot::jump(int newX, int newY, const vector<vector<char>>& field, ofstream& outfile) {
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

void JumpBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    if (jumps > 0) {
        int newX = rand() % field.size();
        int newY = rand() % field[0].size();
        jump(newX, newY, field, outfile);
    } else {
        GenericRobot::think(field, robots, outfile);
    }
}

void JumpBot::revert() {
    this->resetToGeneric();
    upgradeActive = false;
    jumps = 3;  // Reset jumps for next use

}

// HIDEBOT------------------------------------------------------------------
HideBot::HideBot(string name, int x, int y)
    : GenericRobot(name, x, y), hideChances(2), botHidden(false) {
    robotType = "HideBot";
    hasMovingUpgrade = true;
    upgradeActive = true; // Activate the upgrade
}

void HideBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    if (hideChances > 0 && !botHidden) {
        botHidden = true;
        hideChances--;
        log(cout, outfile, name + " is now hidden somewhere......!");
    } else if (botHidden) {
        log(cout, outfile, name + " remains hidden.");
        botHidden = false;  // ends hiding after one turn
    } else {
        log(cout, outfile, name + " has no hide chances left.");
    }

    GenericRobot::think(field, robots, outfile);
}

void HideBot::revert(){
    this->resetToGeneric();
    upgradeActive = false;
    botHidden = false;
    hideChances = 2;
}

// LONGSHOTBOT-------------------------------------------------------------------
LongShotBot::LongShotBot(string name, int x, int y) : GenericRobot(name, x, y)
{
    robotType = "LongShotBot";
    hasShootingUpgrade = true;
    maxDistance = 3; // Set the maximum distance for long shots
    shells = 10; // Set initial shells for LongShotBot
    upgradeActive = true; // Activate the upgrade
};

void LongShotBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    int targetX = PosX + (rand() % 7 - 3); // random offset within 3 tiles
    int targetY = PosY + (rand() % 7 - 3);

    int distance = abs(targetX - PosX) + abs(targetY - PosY);

    if (distance <= maxDistance && !(targetX == PosX && targetY == PosY)) {
        log(cout, outfile, name + " fires long shot at (" + to_string(targetX) + ", " + to_string(targetY) + ")");
    } else {
        log(cout, outfile, name + " failed to fire: out of range.");
    }

    GenericRobot::think(field, robots, outfile);
}

void LongShotBot::revert() {
    this->resetToGeneric();
    upgradeActive = false;
    maxDistance = 3;
    shells = 10; // Reset shells for LongShotBot

}

// SEMIAUTOBOT---------------------------------------------------------------------
SemiAutoBot::SemiAutoBot(string name, int x, int y) : GenericRobot(name, x, y)
{
    robotType = "SemiAutoBot";
    hasShootingUpgrade = true;
    shells = 10; // Set initial shells for SemiAutoBot
    upgradeActive = true; // Activate the upgrade
    lastaction = NONE; // Initialize last action to NONE
};

void SemiAutoBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    if (shells <= 0) {
        log(cout, outfile, name + " has no more shells!");
    } else {
        log(cout, outfile, name + " fires three quick shots.");
        for (int i = 1; i <= 3; ++i) {
            int hitChance = rand() % 100;
            if (hitChance < 70) {
                log(cout, outfile, " Shot " + to_string(i) + ": Hit the target!");
            } else {
                log(cout, outfile, " Shot " + to_string(i) + ": Missed!");
            }
        }
        shells--;
    }

    GenericRobot::think(field, robots, outfile);
}

void SemiAutoBot::revert() {
    this->resetToGeneric();
    upgradeActive = false;
    shells = 10;

}

// THIRTYSHOTBOT-------------------------------------------------------------
ThirtyShotBot::ThirtyShotBot(string name, int x, int y)
    : GenericRobot(name, x, y) {
    robotType = "ThirtyShotBot";
    hasShootingUpgrade = true;
    setShells(30);
    upgradeActive = true; // Activate the upgrade
}

void ThirtyShotBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    log(cout, outfile, name + " (ThirtyShotBot) has " + to_string(getShells()) + " shells.");
    GenericRobot::think(field, robots, outfile);
}

void ThirtyShotBot::revert() {
    this->resetToGeneric();
    upgradeActive = false;
    setShells(30); // Reset shells to 30

}

// SCOUTBOT-------------------------------------------------------------------
ScoutBot::ScoutBot(string name, int x, int y)
    : GenericRobot(name, x, y), scanUses(3) {
    robotType = "ScoutBot";
    hasSeeingUpgrade = true;
    upgradeActive = true; // Activate the upgrade
}

void ScoutBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
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

void ScoutBot::revert() {
    this->resetToGeneric();
    upgradeActive = false;
    scanUses =3;
}

// TRACKBOT-----------------------------------------------------------------------------
TrackBot::TrackBot(string name, int x, int y)
    : GenericRobot(name, x, y), trackers(3) {
    robotType = "TrackBot";
    hasSeeingUpgrade = true;
    trackedEnemies.clear();
    upgradeActive = true;
}

void TrackBot::think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
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

void TrackBot::revert() {
    this->resetToGeneric();
    upgradeActive = false;
    trackers= 3;
    trackedEnemies.clear();
}
