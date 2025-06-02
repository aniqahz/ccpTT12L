#include "battlefield.h"
#include "robot.h"
#include "upgrade.h"
#include <iostream>
#include <vector>
#include <set>  // NEW: To collect unique robot pointers
using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int row = 0, col = 0, steps = 0, numRobot = 0;

    ifstream infile("Robot.txt");
    ofstream outfile("output.txt");
    if (!infile || !outfile) {
        cout << "Error opening file" << endl;
        return 1;
    }

    if (!config(infile, row, col, steps)) {
        cout << "Invalid config" << endl;
        return 1;
    }

    vector<vector<char>> field(row, vector<char>(col, '.'));
    vector<GenericRobot*> robots;
    vector<RobotSpawn> robSpawn;

    string line;
    getline(infile, line); // robots
    sscanf(line.c_str(), "robots: %d", &numRobot);
    robotPos(infile, outfile, field, numRobot, robSpawn, steps, robots);
    displayField(field);

    simulation(outfile, field, steps, robSpawn, robots);

    // --- Clean up allocated robots safely ---
    set<GenericRobot*> robotsToDelete;
    for (auto r : robots)
        if (r) robotsToDelete.insert(r);

    for (auto& data : robSpawn)
        if (data.robot) robotsToDelete.insert(data.robot);

    for (auto r : robotsToDelete)
        delete r;

    infile.close();
    outfile.close();

    return 0;
}
