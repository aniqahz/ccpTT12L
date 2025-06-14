#include "battlefield.h"
#include "robot.h"
#include<iostream>
#include <vector>
using namespace std;

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    int row = 0, col = 0, steps = 0, numRobot = 0;

    ifstream infile("Robot.txt");
    ofstream outfile("output.txt");
    if (!infile || !outfile)
    {
        cout << "Error opening file" << endl;
        return 1;
    }

    if(!config(infile, row,col,steps))
    {
        cout<<"invalid config"<<endl;
        return 1;
    }

    vector<vector<char>> field(row, vector<char>(col, '.'));
    vector<GenericRobot*> robots; //storing robots in a vector

    string line;
    getline(infile, line); // robots
    sscanf(line.c_str(), "robots: %d", &numRobot);
    robotPos(infile, outfile, field, numRobot,robots);
    displayField(field);

    simulation(outfile, field, steps, robots);

      // Clean up robot memory
    for (auto r : robots)
        delete r;
    infile.close();
    outfile.close();

    return 0;
}