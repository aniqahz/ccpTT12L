#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include<iostream>
#include<vector>
#include<string>
#include<fstream>

using namespace std;

void displayField(const vector<vector<char>>& field);
void log(ostream& terminal, ofstream& file, const string& output);
bool config(ifstream& infile, int& row, int& col, int& steps);
void placeRobot(ifstream& infile, ofstream& outfile, vector<vector<char>>& field);
void simulation(ofstream& outfile, vector<vector<char>>& field, int steps);

#endif
