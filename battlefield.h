/**********|**********|**********|
Program: battlefield.h
Course: Object Oriented Programming and Data Structures
Trimester: 2510
Name: ANIQAH NABILAH BINTI AZHAR | JASMYNE YAP | 
NUR ALEEZ DANIA BINTI MOHD SHAHRUL AZMAN | WAN HANANI IMAN BINTI WAN MOHD AZIDI @ SAPAWI
ID: 242UC244LQ | 242UC244PT | 242UC244QB | 242UC244CK
Lecture Section: TC3L
Tutorial Section: T11L
Email: aniqah.nabilah.azhar@student.mmu.edu.my | jasmyne.yap@student.mmu.edu.my |
nur.aleez.dania@student.mmu.edu.my | wan.hanani.iman@student.mmu.edu.my
Phone: 011-6204 6219 | 011-6346 4323 | 019-7109905 | 019-966 0664
**********|**********|**********/


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