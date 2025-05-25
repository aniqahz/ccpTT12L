#include "battlefield.h"

using namespace std;

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
    if(!getline(infile,line))
    {
        return false;
        sscanf(line.c_str(), "M by N : %d %d", &row, &col);
    }

    if(!getline(infile, line))
    {
        return false;
        sscanf(line.c_str(), "steps: %d", &steps);
    }

    return true;
}

void robotPos(ifstream& infile, ofstream& outfile,vector<vector<char>>& field, int numRobot)
{
    string line;
    for(int i=0; i<numRobot; ++i)
    {
        string rType, rName;
        int x=-1, y=-1;

        if(!getline(infile,line))
        {
            break;
        }

        char type[10], name[10];
        rType = type;
        rName = name;

        size_t pos = line.find(rName);
        string after = line.substr(pos + rName.length());
        after.erase(0,after.find_first_not_of(' '));

        if (after == "random random")
        {
            do
            {
                x = rand() % field.size();
                y= rand() % field[0].size();

            }
            while (field[x][y] != '.');}
        else
        {
            int nx, ny;
            int read = sscanf(after.c_str(), "%d %d", &nx, &ny);
            if(read !=2)
            {
                cout<<"invalid position for "<<rName<<endl;
                outfile<<"invalid position for "<<rName<<endl;
                continue;
            }

            x=nx;
            y=ny;

            if(x<0 || x>= field.size() || y<0 || y>= field[0].size())
            {
                cout<<"the position is out of bond"<<endl;
                outfile<<"the position is out of bond"<<endl;
                continue;
            }

            if(field[x][y] != '.')
            {
                cout<<"position is oocupied"<<endl;
                outfile<<"position is oocupied"<<endl;   
                continue;         
            }
        }
        char sym = rName[0];
        field[x][y] = sym;

        cout<<rName<<", "<<sym<<" placed at "<<x<<","<<y<<endl;
        outfile<<rName<<", "<<sym<<" placed at "<<x<<","<<y<<endl;
    }
}

void simulation(ofstream& outfile, vector<vector<char>>& field, int steps)
{
    for(int round=0; round<steps; ++round)
    {
        string turn = "Turn" + to_string(round+1) + "/" + to_string(steps);
        log(cout, outfile, turn);

        for(const auto& row : field)
        {
            string Str(row.begin(), row.end());
            log(cout, outfile, Str);
        }
        this_thread::sleep_for(chrono::milliseconds(500)); //pause before next round/step
    }
}