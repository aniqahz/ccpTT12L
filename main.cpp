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

#include "battlefield.h"
#include "robot.h"
#include "upgrade.h"

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