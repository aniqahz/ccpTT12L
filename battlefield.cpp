#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstdio>
#include<ctime>    // for time()
#include<cstdlib>  // for rand()

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

void robotPos(vector<vector<char>>& field, int x, int y, char sym)
{
    if(x >= 0 && x < field.size() && y >= 0 && y < field[0].size())
        field[x][y] = sym;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    int row = 0, col = 0, steps = 0, numRobot = 0;
    string line;

    ifstream infile("Robot.txt");
    if (!infile)
    {
        cout << "Error opening file." << endl;
        return 1;
    }

    getline(infile, line);
    sscanf(line.c_str(), "M by N : %d %d", &row, &col);
    //cout << "rows: " << row << ", col: " << col << endl;

    vector<vector<char>> field(row, vector<char>(col, '.'));

    getline(infile, line);
    sscanf(line.c_str(), "steps: %d", &steps);

    getline(infile, line);
    sscanf(line.c_str(), "robots: %d", &numRobot);

    for(int i = 0; i < numRobot; ++i)
    {
        string rType, rName;
        int x = -1, y = -1;

        getline(infile, line);
        //cout << "robot line " << i+1 << ": " << line << endl;

        size_t found = line.find("random random");
        if (found != string::npos)
        {
            // Handle random position
            char type[20], name[20];
            sscanf(line.c_str(), "%s %s", type, name);
            rType = type;
            rName = name;

            do {
                x = rand() % row;
                y = rand() % col;
            } while (field[x][y] != '.');
        }
        else
        {
            // Handle fixed position
            char type[20], name[20];
            int nx, ny;
            int matched = sscanf(line.c_str(), "%s %s %d %d", type, name, &nx, &ny);
            if (matched == 4)
            {
                rType = type;
                rName = name;
                x = nx;
                y = ny;

                if (x < 0 || x >= row || y < 0 || y >= col)
                {
                    cout << "Invalid fixed position for " << rName << endl;
                    continue;
                }
                if (field[x][y] != '.')
                {
                    cout << "Position (" << x << "," << y << ") is occupied" << endl;
                    continue;
                }
            }
            else
            {
                cout << "Invalid format for robot line: " << line << endl;
                continue;
            }
        }

        char sym = rName[0];
        //cout << rName << " is placed at (" << x << "," << y << ")" << endl;
        robotPos(field, x, y, sym);
    }

    infile.close();
    displayField(field);

    return 0;
}
