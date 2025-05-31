#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<cstdio>
#include<ctime>    // for time()
#include<cstdlib>  // for rand()
#include<chrono>
#include<thread>
#include<sstream>
#include "robot.h" 
#include<random>

using namespace std;
struct RobotSpawn{
    GenericRobot* robot;
    int spawnTurn; //determine which step to spawn
    bool spawned; //check if it had spawned
};

//void initializeField(vector<vector<char>>& field, int col, int row);
void displayField(const vector<vector<char>>& field);
void log(ostream& terminal, ofstream& file, const string& output);
bool config(ifstream& infile, int& row, int& col, int& maxSteps);//, vector<RobotX>& robots);
void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot, vector<RobotSpawn>&robSpawn,int maxSteps,vector<GenericRobot*>& robot);
void simulation(ofstream& outfile, vector<vector<char>>& field, int steps, vector<RobotSpawn>& robSpawn,vector<GenericRobot*>& robots);

/*int getTurn(int minSteps, int maxSteps);
void spawnRobots(vector<RobotX>& robots, const vector<int>& spawnTurn, int currentSteps, vector<vector<char>>& field, const vector<string>& robNames);
void setRobots(vector<RobotX>& robots, vector<int>& spawnTurn, int maxSteps, const vector<string>& robNames);
void simulateTurn(vector<RobotX>& robots, const vector<int>& spawnTurn, int maxSteps,vector<vector<char>>& field, vector<string>& robNames);
*/
#endif