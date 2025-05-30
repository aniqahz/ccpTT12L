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

using namespace std;

void displayField(const vector<vector<char>>& field);
void log(ostream& terminal, ofstream& file, const string& output);
bool config(ifstream& infile, int& row, int& col, int& steps);
void robotPos(ifstream& infile, ofstream& outfile, vector<vector<char>>& field, int numRobot,vector<GenericRobot*>& robots);
void simulation(ofstream& outfile, vector<vector<char>>& field, int steps, vector<GenericRobot*> &robots);

#endif