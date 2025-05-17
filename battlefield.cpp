#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstdio> //sscanf = word parsing

using namespace std;

void displayField(const vector<vector<char>>& field)
{
    for(const auto& row : field)
    {
        for(char cell : row)
            cout << cell;
    
    cout<<endl;
    }
}

int main()
{
    int row, col, steps, numRobot;
    //char* input;
    string line;
    
    ifstream infile("Robot.txt");
    if (!infile)
    {
        cout << "Error" <<endl;
        return 1;
    }

    getline(infile,line);
    sscanf(line.c_str(), "M by N : %d %d", &row, &col);
    //cout<<"rows: "<<row << ", col: "<< col <<endl; 

    infile.close();

    vector<vector<char>> field(row, vector<char>(col,'.'));

    //cout<< row << col <<endl;
    displayField(field);

    return 0;
}