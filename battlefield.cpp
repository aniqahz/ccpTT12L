/**********|**********|**********|
Program: battlefield.cpp
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

#include<vector>

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

void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot, vector<GenericRobot*>& robots)
{
    string line;
    for(int i=0; i<numRobot; ++i)
    {
        string rType, rName, posX, posY;
        int x=-1, y=-1;

        if(!getline(infile,line))
        {
            cout<<"error reading"<<endl;
            break;
        }

        stringstream ss(line);
        ss>> rType>>rName>> posX>>posY;

        if (posX == "random" && posY == "random")
        {
            if(field.empty() || field[0].empty())
            {
                cout<<"field size not initialised"<<endl;
                return;
            }

            do
            {
                x = rand() % field.size();
                y= rand() % field[0].size();

            }
            while (field[x][y] != '.');
        }
        else
        {
            try
            {
                x = stoi(posX);
                y = stoi(posY);
            }
            catch(...) //catch all types
            {
                cout<<"invalid format for "<<rName<<endl;
                outfile<<"invalid format for "<<rName<<endl;
                continue;
            }
            
        }
        if(x<0 || x>= field.size() || y<0 || y>= field[0].size())
        {
            cout<<rName<<" position is out of bound"<<endl;
            outfile<<rName<<" position is out of bound"<<endl;
            continue;
        }

        if(field[x][y] != '.')
        {
            cout<<"position is oocupied"<<endl;
            outfile<<"position is oocupied"<<endl;   
            continue;         
        }

        char sym = rName[0];
        field[x][y] = sym;

        GenericRobot* newRobot = new GenericRobot(rName, x, y);
        robots.push_back(newRobot);

        cout<<rName<<", "<<sym<<" placed at ("<<x<<","<<y<<")"<<endl;
        outfile<<rName<<", "<<sym<<" placed at ("<<x<<","<<y<<")"<<endl;
        //robot instance
    }
}

void simulation(ofstream& outfile, vector<vector<char>>& field, int steps, vector<GenericRobot*>& robots)
{
    // Loop through each simulation turn, up to the maximum number of steps
    for(int round = 0; round < steps; ++round)
    {
        cout << endl;
        outfile << endl;

        // Display the turn number
        string turn = "Turn " + to_string(round + 1) + "/" + to_string(steps);
        log(cout, outfile, turn);

        // Let each robot take its turn (think and act) if it's still alive
        for(auto& robot : robots) {
            if (robot->getAliveStatus()) {
                robot->think(field, robots, outfile);
            }
        }

        // Display the updated battlefield after this turn
        for(const auto& row : field) {
            string str(row.begin(), row.end());
            log(cout, outfile, str);
        }

        // Count how many robots are still alive after the turn
        int aliveCount = 0;
        GenericRobot* lastAlive = nullptr; // Keep track of the last robot alive (in case it's the winner)
        for (auto& r : robots) {
            if (r->getAliveStatus()) {
                aliveCount++;
                lastAlive = r;
            }
        }

        // End simulation early if only one robot is left alive
        if (aliveCount <= 1) {
            break;
        }

        // Optional delay to simulate time between turns (500 milliseconds)
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    // Final result after simulation ends (either due to steps limit or one robot left)
    int aliveCount = 0;
    GenericRobot* winner = nullptr;
    for (auto& r : robots) {
        if (r->getAliveStatus()) {
            aliveCount++;
            winner = r;
        }
    }

    log(cout, outfile, "\n=== Simulation Ended ===");

    // Declare winner or a draw
    if (aliveCount == 1) {
        // Ensure getRobotType() is defined in GenericRobot or subclass
        log(cout, outfile, "🏆 Winner: " + winner->getrobotname() + " the " + winner->getRobotType());
    } else {
        log(cout, outfile, "🤝 No clear winner — it's a draw!");
    }
}