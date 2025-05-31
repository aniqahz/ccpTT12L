#include "battlefield.h"
#include "robot.h"
vector<GenericRobot*> activeRobots;
queue<GenericRobot*> respawnQueue;
vector<GenericRobot*> revertNextTurn;
map<pair<int, int>, GenericRobot*> positionToRobot;

void displayField(const vector<vector<char>>& field)
{
    for(const auto& row : field)
    {
        for(char cell : row)
            cout << cell;
        cout << endl;
    }
}

void log(ostream& terminal, ofstream& file, const string& output)
{
    terminal << output <<endl;
    file << output <<endl;
}

bool config(ifstream& infile, int& row, int& col, int& steps)
{
    string line;
    if(getline(infile,line))
    {
        sscanf(line.c_str(), "M by N : %d %d", &row, &col);
    }
    else 
        return false;


    if(getline(infile, line))
    {
        sscanf(line.c_str(), "steps: %d", &steps);
    }
    else
        return false;

    return true;
}

void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot)
{
    string line;
    for (int i = 0; i < numRobot; ++i)
    {
        string rType, rName, posX, posY;
        int x = -1, y = -1;

        if (!getline(infile, line)) {
            cout << "error reading" << endl;
            break;
        }

        stringstream ss(line);
        ss >> rType >> rName >> posX >> posY;

        if (posX == "random" && posY == "random") {
            if (field.empty() || field[0].empty()) {
                cout << "field size not initialised" << endl;
                return;
            }

            do {
                x = rand() % field.size();
                y = rand() % field[0].size();
            } while (field[x][y] != '.');
        } else {
            try {
                x = stoi(posX);
                y = stoi(posY);
            } catch (...) {
                cout << "invalid format for " << rName << endl;
                outfile << "invalid format for " << rName << endl;
                continue;
            }
        }

        if (x < 0 || x >= field.size() || y < 0 || y >= field[0].size()) {
            cout << "the position is out of bounds" << endl;
            outfile << "the position is out of bounds" << endl;
            continue;
        }

        if (field[x][y] != '.') {
            cout << "position is occupied" << endl;
            outfile << "position is occupied" << endl;
            continue;
        }

        // Create and place GenericRobot
        GenericRobot* bot = new GenericRobot(rName, x, y);
        bot->setPosition(x, y);
        field[x][y] = rName[0];

        activeRobots.push_back(bot);

        positionToRobot[{x, y}] = bot;

        log(cout, outfile, rName + " [GenericRobot] placed at (" + to_string(x) + "," + to_string(y) + ")");
    }
}


void simulation(ofstream& outfile, vector<vector<char>>& field, int steps)
{   
    for (int round = 0; round < steps; ++round)
    {
        // Log the round number
        string turn = "Turn " + to_string(round + 1) + "/" + to_string(steps);
        log(cout, outfile, turn);

        // Let each robot take its turn
        for (GenericRobot* bot : activeRobots) {
            if (bot->getAliveStatus()) {
                bot->think(field, outfile);  // Let the robot decide and act
            }
        }
           

        for(auto* upgraded : revertNextTurn){
            string name = upgraded->getName();
            pair<int, int> pos = upgraded->getPosition();

            GenericRobot* reverted = new GenericRobot(name, pos.first, pos.second);
            
            auto it = find(activeRobots.begin(), activeRobots.end(), upgraded);
            if (it != activeRobots.end()) {
                *it = reverted;
            }
            delete upgraded;
            log(cout, outfile, name + " reverted back to GenericRobot.");
        }
        revertNextTurn.clear();

        // Respawn one robot from the queue, if any
        processRespawn(field, outfile);

        // Display and log the field after the turn
        for (const auto& row : field){
            string Str(row.begin(), row.end());
            log(cout, outfile, Str);
        }

        // Optional: pause before next round
        this_thread::sleep_for(chrono::milliseconds(500));
    }
}


void processRespawn(vector<vector<char>>& field, ofstream& outfile){
    if (respawnQueue.empty()) return;

    GenericRobot* robot = respawnQueue.front();
    respawnQueue.pop();

    int newX, newY;
    do {
        newX = rand() % field.size();
        newY = rand() % field[0].size();
    } while (field[newX][newY] != '.');

    robot->reset();
    robot->setPosition(newX, newY);
    field[newX][newY] = robot->getName()[0];
    activeRobots.push_back(robot);

    positionToRobot[{newX, newY}] = robot;

    string msg = robot->getName() +" respawn at (" + to_string(newX) + "," + to_string(newY) + ")";
    log(cout, outfile, msg);
}