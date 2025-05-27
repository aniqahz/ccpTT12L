#include "battlefield.h"

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

bool config(ifstream& infile, int& row, int& col, int& maxSteps)
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
        sscanf(line.c_str(), "steps: %d", &maxSteps);
    }
    else
        return false;

    return true;
}

void robotPos(ifstream& infile, ofstream& outfile,vector<vector<char>>& field, int numRobot, vector<RobotX>& robots)
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
        cout << "Loaded robot: " << rType << " " << rName << " Position: " << posX << "," << posY << endl;
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
            cout<<"the position is out of bond"<<endl;
            outfile<<"the position is out of bond"<<endl;
            continue;
        }

        if(field[x][y] != '.')
        {
            cout<<"position is occupied"<<endl;
            outfile<<"position is occupied"<<endl;   
            continue;         
        }

        RobotX r;
        r.id = robots.size() +1;
        r.x = x;
        r.y = y;
        r.active=true;
        robots.push_back(r);

        char sym = rName[0];
        field[x][y] = sym;

        cout<<rName<<", "<<sym<<" placed at "<<x<<","<<y<<endl;
        outfile<<rName<<", "<<sym<<" placed at "<<x<<","<<y<<endl;
    }
}

void simulation(ofstream& outfile, vector<vector<char>>& field, vector<RobotX>& robots, const vector<int>& spawnTurn, int maxSteps)
{
    for(int round=0; round<maxSteps; ++round)
    {
        string turn = "Turn " + to_string(round+1) + "/" + to_string(maxSteps);
        log(cout, outfile, turn);

        spawnRobots(robots, spawnTurn, round+1,field);

        for(const auto& row : field)
        {
            string Str(row.begin(), row.end());
            log(cout, outfile, Str);
        }
        displayField(field);
        this_thread::sleep_for(chrono::milliseconds(500)); //pause before next round/step
    }
}

int getTurn(int minSteps, int maxSteps)
{
    //srand(time(0));
    return minSteps + (rand()%(maxSteps - minSteps+1));
}

void setRobots(vector<RobotX>& robots, vector<int>& spawnTurn, int maxSteps)
{
   int initial = robots.size();
   int newRob = 3;

    /*for(int i=1; i<= 8; ++i) //id start from 1 to 8
        robots[i-1]={i,0,0,false}; //id,x,y,boolean value
    
    for(int i =1; i<5; ++i)
        robots[i].active = true;*/

    for(int i = 0; i<= newRob; ++i) //spawnturn
    {
        RobotX r;
        r.id = initial + i + 1;
        r.active = false;
        robots.push_back(r);

        int turn = getTurn(3, maxSteps);
        spawnTurn.push_back(turn);
        cout<<"Robot "<<i<<" will spawn at steps: "<<turn<<endl;
    }
}

void simulateTurn(vector<RobotX>& robots, const vector<int>& spawnTurn, int maxSteps,vector<vector<char>>& field)
{
    for(int i=0; i<= maxSteps; ++i)
    {
        cout<<"Round "<<i<<endl;
        spawnRobots(robots, spawnTurn, i, field);    
    }
}

void spawnRobots(vector<RobotX>& robots, const vector<int>& spawnTurn, int currentSteps, vector<vector<char>>& field)
{
    for(int i=0; i< spawnTurn.size(); ++i)
    {
        if(currentSteps == spawnTurn[i])
        {
            if(i + 5 <robots.size())
            {
                robots[i+5].active = true;
                cout<<"Robot "<<robots[i+5].id <<" has spawned."<<endl; 
                
                int x, y;
                do{
                    x = rand() % field.size();
                    y = rand() % field[0].size();
                } while (field[x][y] != '.');

                robots[i + 5].x = x;
                robots[i + 5].y = y;
                field[x][y] = robots[i+5].id +'0';

                cout<<"robot "<<robots[i+5].id<<" placed at "<<x<<","<<y<<endl;
            }
            else
            {
                cout<<"index out of bound for robot "<< i+5<<endl;
            }
        }
    }
}