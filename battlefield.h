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

void displayField(const vector<vector<char>>& field);
void log(ostream& terminal, ofstream& file, const string& output);
bool config(ifstream& infile, int& row, int& col, int& maxSteps);//, vector<RobotX>& robots);
void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot, vector<RobotSpawn>&robSpawn,int maxSteps,vector<GenericRobot*>& robot);
void simulation(ofstream& outfile, vector<vector<char>>& field, int steps, vector<RobotSpawn>& robSpawn,vector<GenericRobot*>& robots);

#endif