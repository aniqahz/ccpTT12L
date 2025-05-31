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

void robotPos(ifstream& infile, ofstream& outfile,vector<vector<char>>& field, int numRobot)
{
    string line;
    for(int i=0; i<numRobot; i++)
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
        //robNames.push_back(rName);
        
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

        char sym = rName[0];
        //bool active= (i<5);
        //if (active)
        field[x][y] = sym;
        
       /* RobotX r;
        r.id = i+1;
        r.x = x;
        r.y = y;
        r.active= active;
        robots.push_back(r);*/

        cout<<rName<<", "<<sym<<" placed at "<<x<<","<<y<<endl;
        outfile<<rName<<", "<<sym<<" placed at "<<x<<","<<y<<endl;
    }
}

void simulation(ofstream& outfile, vector<vector<char>>& field, int maxSteps) //, vector<string>& robNames)
{
    for(int round=0; round<maxSteps; ++round)
    {
        string turn = "Turn " + to_string(round+1) + "/" + to_string(maxSteps);
        log(cout, outfile, turn);

        //spawnRobots(robots, spawnTurn, round+1,field, robNames);

        for(const auto& row : field)
        {
            string Str(row.begin(), row.end());
            log(cout, outfile, Str);
        }
      //displayField(field);
        this_thread::sleep_for(chrono::milliseconds(500)); //pause before next round/step
    }
}

/*int getTurn(int minSteps, int maxSteps)
{
    //srand(time(0));
    return minSteps + (rand()%(maxSteps - minSteps+1));
}

void setRobots(vector<RobotX>& robots, vector<int>& spawnTurn, int maxSteps, const vector<string>& robNames)
{
   //int totRob = robots.size();
   int initialRob = 5;
   int newRob = 3;
   //int dynRob = totRob - initialRob;
   //robots.resize(totRob);

    for(int i = 0; i< newRob; ++i) //spawnturn
    {
        RobotX r;
        r.id = initialRob + i + 1;
        r.active = false;
        r.x = -1;
        r.y = -1;
        
        robots.push_back(r);

        int turn = getTurn(3, maxSteps);
        spawnTurn.push_back(turn);
        cout<<"Robot "<<i<<" will spawn at steps: "<<turn<<endl;

        if((initialRob +i)<robNames.size())
            cout<<robNames[initialRob+i]<<", "<<r.id<<" spawn at turn "<<turn<<endl;
        else
            cout<<"out of range"<<endl;
    }
}

void simulateTurn(vector<RobotX>& robots, const vector<int>& spawnTurn, int maxSteps,vector<vector<char>>& field, vector<string>& robNames)
{
    for(int i=0; i<= maxSteps; ++i)
    {
        cout<<"Round "<<i<<endl;
        spawnRobots(robots, spawnTurn, i, field, robNames);    
    }
}

void spawnRobots(vector<RobotX>& robots, const vector<int>& spawnTurn, int currentSteps, vector<vector<char>>& field, const vector<string>& robNames)
{
    int startIndex = robots.size() - spawnTurn.size();
    for(int i=0; i< spawnTurn.size(); ++i)
    {
        if(currentSteps == spawnTurn[i])
        {
            int index = startIndex + i;

            if(index >= robots.size() || index >= robNames.size())
            {
                cout<<"index out of bound for robot "<< index<<endl;
                continue;
            }

            robots[index].active = true;
            cout<<"Robot "<<robots[index].id <<", "<<robNames[index]<<" has spawned."<<endl; 

            int x = robots[index].x;
            int y = robots[index].y;

            if(x ==-1 || y==-1)
            {
                do{
                    x = rand() % field.size();
                    y = rand() % field[0].size();
                } while (field[x][y] != '.');

                robots[index].x = x;
                robots[index].y = y;
            }
            if(field[x][y]=='.')
            {
                char symbol = robNames[index][0];
                field[x][y] = symbol;
                cout<<"robot "<<robNames[index]<<" placed at "<<x<<","<<y<<endl;
            }   
            else
            {
                cout<<"position is occupied "<< i+5<<endl;
            }
        }
    }
}*/