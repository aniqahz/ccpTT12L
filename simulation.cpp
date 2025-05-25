#include "battlefield.h"

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

    vector<vector<char>> field(row, vector<char>(col, '.'));

    int numRobot;
    string line;
    
    //getline(infile, line); // M by N
    //sscanf(line.c_str(), "M by N : %d %d", &row, &col);

    getline(infile, line); // robots
    sscanf(line.c_str(), "robots: %d", &numRobot);

    robotPos(infile,outfile,field, numRobot);
    simulation(outfile, field, steps);

    infile.close();
    outfile.close();

    return 0;
}
