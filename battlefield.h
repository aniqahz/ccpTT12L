#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include "robot.h"
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
#include<queue>
#include<algorithm>
#include <map>

using namespace std;

void displayField(const vector<vector<char>>& field);
void log(ostream& terminal, ofstream& file, const string& output);
bool config(ifstream& infile, int& row, int& col, int& steps);
void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot);
void simulation(ofstream& outfile, vector<vector<char>>& field, int steps);

extern map<pair<int, int>, GenericRobot*> positionToRobot;
extern vector<GenericRobot*> activeRobots;
extern queue<GenericRobot*> respawnQueue;
extern vector<GenericRobot*> revertNextTurn;
void processRespawn(vector<vector<char>>& field, ofstream& outfile);


#endif
