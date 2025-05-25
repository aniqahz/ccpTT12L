#include <iostream>
#include <vector> //dynamic 2D array
#include <string>
#include <fstream> //read from file and write output to file
#include <cstdio>   // for sscanf
#include <cstdlib>  // for random position generator (rand)
#include <ctime>    // for time
#include <thread>   // for sleep
#include <chrono>

using namespace std;

void displayField(const vector<vector<char>>& field) //2D vector
{
    for (const auto& row : field)
    {
        for (char cell : row)
            cout << cell;
        cout << endl;
    }
}

void log(ostream& terminal, ofstream& file, const string& output)
{
    terminal << output <<endl;
    file << output <<endl;
}

void robotPos(vector<vector<char>>& field, int x, int y, char sym)
{
    if (x >= 0 && x < field.size() && y >= 0 && y < field[0].size())
        field[x][y] = sym;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    int row = 0, col = 0, steps = 0, numRobot = 0;
    string line;

    ifstream infile("Robot.txt");
    ofstream outfile("output.txt");
    if (!infile || !outfile)
    {
        cout << "Error opening file" << endl;
        return 1;
    }

    getline(infile, line); // M by N
    sscanf(line.c_str(), "M by N : %d %d", &row, &col);
    cout << "rows: " << row << ", col: " << col << endl;

    vector<vector<char>> field(row, vector<char>(col, '.'));

    getline(infile, line); // steps
    sscanf(line.c_str(), "steps: %d", &steps);

    getline(infile, line); // robots
    sscanf(line.c_str(), "robots: %d", &numRobot);

    for (int i = 0; i < numRobot; ++i)
    {
        string rType, rName;
        int x = -1, y = -1;
        getline(infile, line);

        cout << "robot line " << i + 1 << ": " << line << endl;

        char type[20], name[20];
        int ret = sscanf(line.c_str(), "%s %s", type, name);
        if (ret != 2) {
            cout << "Invalid robot format, skipping line" << endl;
            continue;
        }

        rType = type;
        rName = name;

        size_t pos = line.find(rName);
        string after = line.substr(pos + rName.length());
        after.erase(0, after.find_first_not_of(' '));

        if (after == "random random")
        {
            do {
                x = rand() % row;
                y = rand() % col;
            } while (field[x][y] != '.');
        }
        else
        {
            int nx, ny;
            int read = sscanf(after.c_str(), "%d %d", &nx, &ny);
            if (read == 2)
            {
                x = nx;
                y = ny;
            }
            else
            {
                cout << "Invalid position for " << rName << endl;
                continue;
            }

            if (x < 0 || x >= row || y < 0 || y >= col)
            {
                cout << "Error: position out of bounds for " << rName << endl;
                continue;
            }

            if (field[x][y] != '.')
            {
                cout << "Position (" << x << "," << y << ") is already occupied" << endl;
                continue;
            }
        }

        char sym = rName[0];
        cout << rName << " (" << sym << ") is placed at (" << x << "," << y << ")" << endl;
        robotPos(field, x, y, sym);
    }

    infile.close();
    outfile.close();

    // === Simulation Loop ===
    for (int step = 0; step < steps; ++step)
    {
        cout << "\nTurn " << step + 1 << "/" << steps << endl;
        displayField(field);

        // Placeholder: Robot actions would be added here in the future

        // Optional: delay for better visualization
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    return 0;
}
