#include "battlefield.h"
#include "robot.h"
#include "upgrade.h"

#include <vector>
#include <queue>
#include <map>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;

vector<GenericRobot*> activeRobots;
queue<GenericRobot*> respawnQueue;
vector<GenericRobot*> revertNextTurn;
map<pair<int, int>, GenericRobot*> positionToRobot;
map<GenericRobot*, int> robotRespawnCount;
vector<pair<GenericRobot*, GenericRobot*>> replaceNextTurn;

void displayField(const vector<vector<char>>& field) {
    for (const auto& row : field) {
        for (char cell : row) cout << cell;
        cout << endl;
    }
}

void log(ostream& terminal, ofstream& file, const string& output) {
    terminal << output << endl;
    file << output << endl;
}

bool config(ifstream& infile, int& row, int& col, int& steps) {
    string line;
    if (getline(infile, line)) {
        sscanf(line.c_str(), "M by N : %d %d", &row, &col);
    } else return false;

    if (getline(infile, line)) {
        sscanf(line.c_str(), "steps: %d", &steps);
    } else return false;

    return true;
}

void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot, vector<GenericRobot*>& robots) {
    string line;
    for (int i = 0; i < numRobot; ++i) {
        string rType, rName, posX, posY;
        int x = -1, y = -1;

        if (!getline(infile, line)) break;
        stringstream ss(line);
        ss >> rType >> rName >> posX >> posY;

        if (posX == "random" && posY == "random") {
            do {
                x = rand() % field.size();
                y = rand() % field[0].size();
            } while (field[x][y] != '.');
        } else {
            try {
                x = stoi(posX);
                y = stoi(posY);
            } catch (...) {
                log(cout, outfile, "Invalid format for " + rName);
                continue;
            }
        }

        if (x < 0 || x >= field.size() || y < 0 || y >= field[0].size()) {
            log(cout, outfile, rName + " position is out of bounds.");
            continue;
        }

        if (field[x][y] != '.') {
            log(cout, outfile, rName + " position is occupied.");
            continue;
        }

        field[x][y] = rName[0];
        GenericRobot* newRobot = new GenericRobot(rName, x, y);
        robots.push_back(newRobot);
        activeRobots.push_back(newRobot);
        robotRespawnCount[newRobot] = 0;

        log(cout, outfile, rName + ", " + rName[0] + " placed at (" + to_string(x) + "," + to_string(y) + ")");
    }
}

void simulation(ofstream& outfile, vector<vector<char>>& field, int steps, vector<GenericRobot*>& robots) {
    for (int round = 0; round < steps; ++round) {
        log(cout, outfile, "\nTurn " + to_string(round + 1) + "/" + to_string(steps));

        for (auto& [oldBot, newBot] : replaceNextTurn) {
            for (auto& robot : robots) if (robot == oldBot) robot = newBot;
            for (auto& bot : revertNextTurn) if (bot == oldBot) bot = newBot;
            for (auto& [pos, bot] : positionToRobot) if (bot == oldBot) bot = newBot;
            delete oldBot;
        }

        for (auto& [oldBot, newBot] : replaceNextTurn) {
            for (auto& robot : robots) if (robot == oldBot) robot = newBot;
            for (auto& bot : revertNextTurn) if (bot == oldBot) bot = newBot;
            for (auto& [pos, bot] : positionToRobot) if (bot == oldBot) bot = newBot;
            // Set all oldBot pointers to nullptr to avoid dangling pointers
            for (auto& robot : robots) if (robot == oldBot) robot = nullptr;
            for (auto& bot : activeRobots) if (bot == oldBot) bot = nullptr;
            delete oldBot;
        }

        replaceNextTurn.clear();

        for (auto& robot : robots) {
            if (!robot) continue; // skip if robot is nullptr
            if (robot && robot->getAliveStatus()) {
                robot->think(field, robots, outfile);
            }
        }

        vector<GenericRobot*> revertCleanup;
        for (auto* upgraded : revertNextTurn) {
            if (!upgraded) continue;

            string name = upgraded->getRobotName();
            auto [x, y] = upgraded->getPosition();

            GenericRobot* reverted = new GenericRobot(name, x, y);
            reverted->setAliveStatus(upgraded->getAliveStatus());
            reverted->setShells(upgraded->getShells());
            reverted->setPosition(x, y);
            reverted->reset();

            auto it = find(activeRobots.begin(), activeRobots.end(), upgraded);
            if (it != activeRobots.end()) *it = reverted;

            auto itAll = find(robots.begin(), robots.end(), upgraded);
            if (itAll != robots.end()) *itAll = reverted;

            for (auto& [posKey, bot] : positionToRobot)
                if (bot == upgraded) bot = reverted;

            revertCleanup.push_back(upgraded);
            log(cout, outfile, name + " reverted back to GenericRobot.");
        }
        revertNextTurn.clear();
        for (auto* r : revertCleanup) delete r;

        processRespawn(field, outfile);

        for (const auto& row : field) {
            string line(row.begin(), row.end());
            log(cout, outfile, line);
        }

        int aliveCount = 0;
        GenericRobot* lastAlive = nullptr;
        for (auto& r : robots) {
            if (r && r->getAliveStatus()) {
                aliveCount++;
                lastAlive = r;
            }
        }

        if (aliveCount <= 1) break;

        this_thread::sleep_for(chrono::milliseconds(500));
    }

    int aliveCount = 0;
    GenericRobot* winner = nullptr;
    for (auto& r : robots) {
        if (r && r->getAliveStatus()) {
            aliveCount++;
            winner = r;
        }
    }

    log(cout, outfile, "\n=== Simulation Ended ===");

    if (aliveCount == 1) log(cout, outfile, "Winner: " + winner->getRobotName() + " the " + winner->getRobotType());
    else log(cout, outfile, "No clear winner — it's a draw!");
}

void processRespawn(vector<vector<char>>& field, ofstream& outfile) {
    if (respawnQueue.empty()) return;

    GenericRobot* robotToRespawn = respawnQueue.front();
    respawnQueue.pop();

    int rows = field.size(), cols = field[0].size(), x, y, attempts = 0;
    const int maxAttempts = 100;

    do {
        x = rand() % rows;
        y = rand() % cols;
        attempts++;
    } while (field[x][y] != '.' && attempts < maxAttempts);

    if (attempts >= maxAttempts) {
        log(cout, outfile, "No empty position found to respawn robot.");
        return;
    }

    robotToRespawn->reset();
    robotToRespawn->setPosition(x, y);
    robotToRespawn->setAliveStatus(true);
    robotToRespawn->setIsQueuedForRespawn(false);

    field[x][y] = robotToRespawn->getRobotName()[0];
    positionToRobot[{x, y}] = robotToRespawn;

    log(cout, outfile, robotToRespawn->getRobotName() + " respawned at (" + to_string(x) + ", " + to_string(y) + ")");
}
