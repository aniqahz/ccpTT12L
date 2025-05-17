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

void robotPos(vector<vector<char>>& field, int x, int y, char sym)
{
    if(x >= 0 && x < field.size() && y>= 0 && y< field[0].size())
        field[x][y]=sym;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    int row=0, col=0, steps=0, numRobot=0;
    string line;
    
    ifstream infile("Robot.txt");
    if (!infile)
    {
        cout << "Error" <<endl;
        return 1;
    }

    getline(infile,line); //set the battlefield dimension width and height
    sscanf(line.c_str(), "M by N : %d %d", &row, &col);
    //cout<<"rows: "<<row << ", col: "<< col <<endl; 

    vector<vector<char>> field(row, vector<char>(col,'.'));

    getline(infile,line);
    sscanf(line.c_str(), "steps: %d", &steps);// pass steps value to steps var
    //cout<<"steps: "<<steps<<endl;

    getline(infile,line);
    sscanf(line.c_str(),"robots: %d", &numRobot); //pass robot position to numRobot var
    //cout<<"read robots.. "<<endl;

    for(int i=0; i<numRobot; ++i) //identify robot type and its  position
    {
        if(!getline(infile,line))
        {
            cout<<"failed to read"<< i+1<<endl;
            break;
        }
        cout<<"robot line "<<i+1<< ": "<<line<<endl;

        string rType, rName;
        
        int x=-1, y=-1;
        getline(infile,line);

        char type[10], name[10];
        int ret = sscanf(line.c_str(),"%s %s random random", type, name);
        cout<<"return: "<<ret<<endl;

        rType = type;
        rName = name;

        size_t pos = line.find(rName);
        string after = line.substr(pos+rName.length());
        after.erase(0,after.find_first_not_of(' '));

        if(after=="random random") //for random posiitoned robot 
        {
            cout<<rName<<" is positioned"<<endl;
            do
            {
                x=rand() % row;
                y = rand() % col;
            } while (field[x][y] != '.');
        }
        else //for fixed position robot
        {
            int nx,ny;
            int read = sscanf(line.c_str(), "%*s %*s %d %d",&nx, &ny );
            if (read==2)
            {
                x=nx;
                y=ny;
            }
            else
            {
                cout<<"invalid format"<< rName<<endl;
                continue;
            }

            //validating coordinates
            if(x<0 || x>=row || y<0 || y>=col)
            {
                cout<<"error for "<<rName<<endl;
                continue;
            }
            if(field[x][y] != '.')
            {
                cout<<x<<" "<<y<<" is occupied<<"<<endl;
                continue;
            }
        
        }
        /*else
        {
            char type[10], name[10]; //temp var to store robot name and type
            sscanf(line.c_str(), "%*s %s %d %d",type, name, &x, &y);
            rType=type;
            rName=name;
        }

        //error handling
        if(x>=0 && x<row && y>=0 && y<col)
        {
            if(field[x][y] !='.')
            {
                cout<<"Warning: "<<x<<" "<<y<<" is occupied";
                continue;
            }
            else
            {
                cout<<"invalid fixed position for "<<rName;
                continue;
            }
        }*/

        char sym = rName[0];
        cout<<rName<< sym<<" is placed at "<<x<<" "<<y<<endl;
        robotPos(field, x, y, sym);
    }

    infile.close();

    //cout<< row << col <<endl;
    displayField(field);

    return 0;
}