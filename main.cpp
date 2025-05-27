#include "battlefield.h"
#include "robot.h"
#include<iostream>

using namespace std;

int main()
{
    //srand(static_cast<unsigned int>(time(nullptr)));
    srand(time(0));

    int row = 0, col = 0, maxSteps = 0, numRobot = 0;

    ifstream infile("Robot.txt");
    ofstream outfile("output.txt");
    if (!infile || !outfile)
    {
        cout << "Error opening file" << endl;
        return 1;
    }

    if(!config(infile, row,col,maxSteps))
    {
        cout<<"invalid config"<<endl;
        return 1;
    }

    vector<vector<char>> field(row, vector<char>(col, '.'));
    vector<RobotX> robots;
    vector<int> spawnTurn;

    //setRobots(robots, spawnTurn);

    string line;
    getline(infile, line); // robots
    sscanf(line.c_str(), "robots: %d", &numRobot);

    robotPos(infile, outfile, field, numRobot, robots);
    displayField(field);
    setRobots(robots, spawnTurn, maxSteps);
    simulation(outfile, field, robots, spawnTurn, maxSteps);

    infile.close();
    outfile.close();

    return 0;
}
