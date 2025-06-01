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

vector<RobotSpawn> robSpawn;
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

void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot, vector<RobotSpawn>& robSpawn, int maxSteps, vector<GenericRobot*>& robots) {
    robots.clear();
    robSpawn.clear();
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

        if (x < 0 || x >= field.size() || y < 0 || y >= field[0].size() || field[x][y] != '.') {
            log(cout, outfile, rName + " position invalid or occupied.");
            continue;
        }

        GenericRobot* newRobot = new GenericRobot(rName, x, y);

        RobotSpawn data;
        data.robot = newRobot;
        data.spawned = (i < numRobot - 3); // Spawn all except last 3
        data.spawnTurn = data.spawned ? 0 : (rand() % (maxSteps - 3)) + 3;

        if (data.spawned) {
            field[x][y] = rName[0];
            robots.push_back(newRobot);

            activeRobots.push_back(newRobot);
            positionToRobot[{x, y}] = newRobot;
            robotRespawnCount[newRobot] = 0;

            log(cout, outfile, rName + " placed at (" + to_string(x) + "," + to_string(y) + ")");
        } else {
            log(cout, outfile, rName + " will spawn at (" + to_string(x) + "," + to_string(y) + ") on turn " + to_string(data.spawnTurn+1));
        }

        robSpawn.push_back(data);
    }
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

    log(cout, outfile, robotToRespawn->getRobotName() + " respawned at (" + to_string(x) + "," + to_string(y) + ")");
}

void simulation(ofstream& outfile, vector<vector<char>>& field, int steps, vector<RobotSpawn>& robSpawn, vector<GenericRobot*>& robots){
    auto replaceAllReferences = [&](GenericRobot* oldBot, GenericRobot* newBot) {
        for (auto& robot : robots) if (robot == oldBot) robot = newBot;
        for (auto& bot : activeRobots) if (bot == oldBot) bot = newBot;
        for (auto& bot : revertNextTurn) if (bot == oldBot) bot = newBot;
        for (auto& [pos, bot] : positionToRobot) if (bot == oldBot) bot = newBot;

      /*   for (auto& data : robSpawn) {
            if (!data.spawned && steps + 1 == data.spawnTurn) {
                auto [x, y] = data.robot->getPosition();
                if (field[x][y] == '.') {
                    field[x][y] = data.robot->getRobotName()[0];
                    data.spawned = true;

                    robots.push_back(data.robot);
                    activeRobots.push_back(data.robot);
                    positionToRobot[{x, y}] = data.robot;
                    robotRespawnCount[data.robot] = 0;

                    log(cout, outfile, data.robot->getRobotName() + " spawned at (" + to_string(x) + "," + to_string(y) + ")");
                } else {
                    log(cout, outfile, data.robot->getRobotName() + " failed to spawn (spot occupied)");
                }
            }
        } */

        if (robotRespawnCount.count(oldBot)) {
            robotRespawnCount[newBot] = robotRespawnCount[oldBot];
            robotRespawnCount.erase(oldBot);
        }

        queue<GenericRobot*> tempQueue;
        while (!respawnQueue.empty()) {
            GenericRobot* r = respawnQueue.front(); respawnQueue.pop();
            tempQueue.push(r == oldBot ? newBot : r);
        }
        respawnQueue = tempQueue;
    };

    for (int round = 0; round < steps; ++round) {
        log(cout, outfile, "\nTurn " + to_string(round + 1) + "/" + to_string(steps));
        
        for (auto& data : robSpawn) {
            if (!data.spawned && data.spawnTurn == round) {
                auto[x,y] = data.robot->getPosition();
                if (field[x][y] == '.') {
                    field[x][y] = data.robot->getRobotName()[0];
                    data.spawned = true;

                    robots.push_back(data.robot);
                    activeRobots.push_back(data.robot);
                    positionToRobot[{x, y}] = data.robot;
                    robotRespawnCount[data.robot] = 0;

                    log(cout, outfile, data.robot->getRobotName() + " spawned at (" + to_string(x) + "," + to_string(y) + ")");
                } else {
                    bool spawnedElsewhere = false;
                    for (int i = 0; i < field.size(); i++) {
                        for (int j = 0; j < field[0].size(); j++) {
                            if (field[i][j] == '.') {
                                // move robot to new position
                                data.robot->setPosition(i, j);
                                field[i][j] = data.robot->getRobotName()[0];
                                spawnedElsewhere = true;
            
                                log(cout, outfile, data.robot->getRobotName() + " spawned at (" + to_string(i) + "," + to_string(j) + ") instead (original spot occupied)");
            
                                // update data
                                data.spawned = true;
                                robots.push_back(data.robot);
                                activeRobots.push_back(data.robot);
                                positionToRobot[{i, j}] = data.robot;
                                robotRespawnCount[data.robot] = 0;
            
                                break;
                            }
                        }
                        if (spawnedElsewhere) break;
                    }
                    if (!spawnedElsewhere) {
                        log(cout, outfile, data.robot->getRobotName() + " failed to spawn (no free spot found)");
                    }
                }
            }
        }
        // Upgrade replacements
        for (auto& [oldBot, newBot] : replaceNextTurn) {
            replaceAllReferences(oldBot, newBot);
            delete oldBot;
        }
        replaceNextTurn.clear();

        // Robot actions
        for (auto& robot : robots) {
            if (robot && robot->getAliveStatus()) {
                robot->think(field, robots, outfile);
            }
        }

        // Revert upgrades
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

            replaceAllReferences(upgraded, reverted);
            field[x][y] = reverted->getRobotName()[0];  // Update field character

            revertCleanup.push_back(upgraded);
            log(cout, outfile, name + " reverted back to GenericRobot.");
        }
        revertNextTurn.clear();
        for (auto* r : revertCleanup) delete r;

        // Handle respawns
        processRespawn(field, outfile);

        // Display field
        for (const auto& row : field) {
            string line(row.begin(), row.end());
            log(cout, outfile, line);
        }

        // Early end if only one robot is left
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

    // Final result
    int aliveCount = 0;
    GenericRobot* winner = nullptr;
    for (auto& r : robots) {
        if (r && r->getAliveStatus()) {
            aliveCount++;
            winner = r;
        }
    }

    log(cout, outfile, "\n=== Simulation Ended ===");
    if (aliveCount == 1)
        log(cout, outfile, "Winner: " + winner->getRobotName() + " the " + winner->getRobotType());
    else
        log(cout, outfile, "No clear winner — it's a draw!");
}

