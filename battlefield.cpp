#include<vector>
#include "battlefield.h"
#include "robot.h"
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
            cout<<rName<<"the position is out of bound"<<endl;
            outfile<<rName<<"the position is out of bound"<<endl;
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
    for(int round=0; round<steps; ++round)
    {
        cout<<endl;
        outfile<<endl;
        string turn = "Turn " + to_string(round+1) + "/" + to_string(steps);
        log(cout, outfile, turn);

        //to think and act
        for(auto &robot: robots){
            robot->think(field, robots, outfile);
        }

        for(const auto& row : field)
        {
            string Str(row.begin(), row.end());
            log(cout, outfile, Str);
        }
        this_thread::sleep_for(chrono::milliseconds(500)); //pause before next round/step
    }
}