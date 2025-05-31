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

void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot, vector<RobotSpawn>&robSpawn,int maxSteps,vector<GenericRobot*>& robot)
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
        if(x<0 || x>= field.size() || y<0 || y>= field[0].size() || field[x][y] != '.')
        {
            cout<<rName<<" position is out of bound"<<endl;
            outfile<<rName<<" position is out of bound"<<endl;
            continue;
        }

        GenericRobot* newRobot = new GenericRobot(rName,x,y);

        RobotSpawn data;
        data.robot = newRobot;
        data.spawned = (i<5); //spawn first 5
        data.spawnTurn = (i<5) ? 0 : (rand() % (maxSteps -3 +1)) +3; //random turn between 3 to maxSteps
        
        if(data.spawned)
        {
            field[x][y] = rName[0];
            cout<<rName<<" placed at ("<<x<<","<<y<<")"<<endl;
            outfile<<rName<<" placed at ("<<x<<","<<y<<")"<<endl;    
        }
        else
        {
            cout<<rName<<" placed at ("<<x<<", "<<y<<")"<<endl;
            outfile<<rName<<" placed at ("<<x<<", "<<y<<")"<<endl;
        }
        robSpawn.push_back(data);
    }
}

void simulation(ofstream& outfile, vector<vector<char>>& field, int steps, vector<RobotSpawn>& robSpawn,vector<GenericRobot*>& robots)
{
    // Loop through each simulation turn, up to the maximum number of steps
    for(int round = 0; round < steps; ++round)
    {
        cout << endl;
        outfile << endl;

        // Display the turn number
        string turn = "Turn " + to_string(round + 1) + "/" + to_string(steps);
        log(cout, outfile, turn);

        for(auto& data : robSpawn)
        {
            if(!data.spawned && round+1 == data.spawnTurn)
            {
                auto pos = data.robot->getPosition();
                int x = pos.first;
                int y = pos.second;

                if(field[x][y] == '.')
                {
                    field[x][y] = data.robot->getrobotname()[0];//first letter of the robot name
                    data.spawned = true;
                    robots.push_back(data.robot);
                    log(cout,outfile, data.robot->getrobotname()+" spawned at "+ to_string(x)+","+to_string(y));
                }
                else
                {
                    log(cout,outfile, data.robot->getrobotname() +"failed to spawn: position occupied");
                }
            }

            if(data.spawned && data.robot->getAliveStatus())
           {
            data.robot->think(field,robots,outfile);
           } 

            
        }
        // Let each robot take its turn (think and act) if it's still alive
     /*    for(auto& robot : robots) {
            if (robot->getAliveStatus()) {
                robot->think(field, robots, outfile);
            }
        } */

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
     /*  if (aliveCount <= 1) {
            break;
        } */

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

