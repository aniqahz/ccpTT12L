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

void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot, vector<GenericRobot*>& robots)
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
            cout<<rName<<" position is out of bound"<<endl;
            outfile<<rName<<" position is out of bound"<<endl;
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

        //spawnRobots(robots, spawnTurn, round+1,field, robNames);

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