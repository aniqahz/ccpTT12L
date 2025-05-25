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

void robotPos(vector<vector<char>>& field, int x, int y, char sym)
{
    if(x >= 0 && x < field.size() && y >= 0 && y < field[0].size())
        field[x][y] = sym;
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