#include "robot.h"
#include "battlefield.h"
#include "upgrade.h"

// ---------------- baseRobot ----------------

baseRobot::baseRobot() : PosX(0), PosY(0), isAlive(true), remainingLives(3) {}

baseRobot::baseRobot(int x, int y) : PosX(x), PosY(y), isAlive(true), remainingLives(3) {}

pair<int, int> baseRobot::getPosition() const { return {PosX, PosY}; }

void baseRobot::setPosition(int x, int y) { PosX = x; PosY = y; }

bool baseRobot::getAliveStatus() const { return isAlive; }

string baseRobot::getRobotType() const { return robotType; }

void baseRobot::loseLife(){
    remainingLives--;
    isAlive = false;
}

int baseRobot::getRemainingLives() const {
    return remainingLives;
}

void baseRobot::markDead(){
    isAlive = false;
}

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

void GenericRobot::think(vector<vector<char>>& field, ofstream& outfile) {
    int action = rand() % 5;
    int dx = rand() % 3 - 1;
    int dy = rand() % 3 - 1;

    switch (action) {
        case 0:
            log(cout, outfile, name + " is idle.");
            break;
        case 1:
            log(cout, outfile, name + " attempts to fire.");
            fire(dx, dy, field, outfile);
            break;
        case 2:
            log(cout, outfile, name + " looks around.");
            look(dx, dy, field, outfile);
            break;
        case 3:
            log(cout, outfile, name + " attempts to move.");
            move(dx, dy, field, outfile);
            break;
        case 4:
            log(cout, outfile, name + " is staying still.");
            break;
    }
}


void GenericRobot::look(int dx, int dy, const vector<vector<char>> &field, ofstream& outfile) {
    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j) {
            int newX = PosX + i;
            int newY = PosY + j;
            if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) {
                if (field[newX][newY] != '.')
                    log(cout, outfile, "Enemy detected!" );
            } else {
                log(cout, outfile, "Out of bounds..." );
            }
        }
}

bool GenericRobot::fire(int dx, int dy, vector<vector<char>>& field, ofstream& outfile) {
    bool killed = false;

    // Fire logic
    for (int i = -1; i <= 1; ++i)
        for (int j = -1; j <= 1; ++j) {
            int targetX = PosX + i;
            int targetY = PosY + j;

            if (shells > 0) {
                shells--;

                if (targetX == PosX && targetY == PosY) {
                continue;}// Don't allow self-fire

                int probability = rand() % 100;
                if (field[targetX][targetY] != '.' && (targetX != PosX || targetY != PosY)) {
                    // Found a target
                    int probability = rand() % 100;
                    if (probability >= 70) {
                        // 💥 Successful kill
                        auto pos = make_pair(targetX, targetY);
                        auto it = positionToRobot.find(pos);
                        if (it != positionToRobot.end()) {
                            GenericRobot* victim = it->second;

                            // Mark victim as dead
                            victim->markDead();       // sets isAlive = false
                            victim->loseLife();       // subtracts 1 life

                            string logMsg = name + " killed " + victim->getName() + "!";
                            cout << logMsg << endl;
                            outfile << logMsg << endl;

                            // Remove from field and map
                            field[targetX][targetY] = '.';
                            positionToRobot.erase(pos);

                            // If the victim has lives, clone and queue for respawn
                            if (victim->getRemainingLives() > 0) {
                                GenericRobot* clone = new GenericRobot(victim->getName(), 0, 0);
                                respawnQueue.push(clone);
                            }

                            return true; // upgrade trigger
                        }
                    }
                }

            }
        }

    return killed;
}


void GenericRobot::move(int dx, int dy, vector<vector<char>> &field, ofstream& outfile) {
    int newX = PosX + dx;
    int newY = PosY + dy;

    if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size() && field[newX][newY] == '.') {
        // Clear old position in field & map
        field[PosX][PosY] = '.';
        positionToRobot.erase({PosX, PosY});

        // Move
        PosX = newX;
        PosY = newY;
        field[newX][newY] = name[0];
        positionToRobot[{newX, newY}] = this;

        log(cout, outfile, "Moved to (" + to_string(PosX) + "," + to_string(PosY) + ")");
    } else {
        log(cout, outfile, "Cannot move out of bounds!");
    }

}

void GenericRobot::reset(){
    shells = 10;
    isAlive = true;
    PosX = 0;
    PosY = 0;
}

void GenericRobot::awardUpgrade(vector<GenericRobot*>& activeRobots, vector<vector<char>>& field, ofstream& outfile) {
    if (upgradesUsed >= 3) {
        cout << name << " has maxed out all upgrades." << endl;
        return;
    }

    vector<string> available;
    if (!hasMovingUpgrade) available.push_back("Moving");
    if (!hasShootingUpgrade) available.push_back("Shooting");
    if (!hasSeeingUpgrade) available.push_back("Seeing");

    if (available.empty()) return;

    string choice = available[rand() % available.size()];
    cout << name << " received upgrade: " << choice << endl;

    GenericRobot* upgraded = nullptr;

    if (choice == "Moving") {
        vector<string> movingUpgrades = {"Jump", "Hide"};
        string selected = movingUpgrades[rand() % movingUpgrades.size()];

        outfile << name << " received Moving upgrade: " << selected << endl;
        cout    << name << " received Moving upgrade: " << selected << endl;

        if (selected == "Jump")
            upgraded = new jumpBot(name, field);
        //else if (selected == "Hide")
        //    upgraded = new hideBot(name, field);
        
        if (upgraded) {
            upgraded->setPosition(PosX, PosY);
            upgraded->hasMovingUpgrade = true;
        }
    }

    else if (choice == "Shooting") {
        outfile << name << " would have received a Shooting upgrade." << endl;
        cout << name << " would have received a Shooting upgrade." << endl;
        return;
    }
    else if (choice == "Seeing") {
        outfile << name << " would have received a Seeing upgrade." << endl;
        cout << name << " would have received a Seeing upgrade." << endl;
        return;
    }

    if (upgraded) {
        upgraded->hasShootingUpgrade = hasShootingUpgrade;
        upgraded->hasSeeingUpgrade = hasSeeingUpgrade;
        upgraded->upgradesUsed = upgradesUsed + 1;

        auto it = std::find(activeRobots.begin(), activeRobots.end(), this);
        if (it != activeRobots.end()) {
            *it = upgraded;
        }

        revertNextTurn.push_back(upgraded);
        delete this;
    }
}

void GenericRobot::logStatus(ofstream& out) const {
    out << name << " [" << robotType << "]"
        << " at (" << PosX << "," << PosY << ") | "
        << "Shells: " << shells << " | "
        << "Lives: " << remainingLives << " | "
        << "Upgrades Used: " << upgradesUsed << endl;
}

