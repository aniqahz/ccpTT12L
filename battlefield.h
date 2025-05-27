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
#include<random>

using namespace std;
struct RobotX {
    int id;
    int x,y;
    bool active;
};

void displayField(const vector<vector<char>>& field);
void log(ostream& terminal, ofstream& file, const string& output);
bool config(ifstream& infile, int& row, int& col, int& steps);
void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot);
void simulation(ofstream& outfile, vector<vector<char>>& field, vector<RobotX>& robots, const vector<int>& spawnTurn, int maxSteps);

int getTurn(int minSteps, int maxSteps);
void spawnRobots(vector<RobotX>& robots, const vector<int>& spawnTurn, int currentSteps);
void setRobots(vector<RobotX>& robots, vector<int>& spawnTurn);
void simulateTurn(vector<RobotX>& robots, const vector<int>& spawnTurn, int maxSteps);

#endif
