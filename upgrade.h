/**********|**********|**********|
Program: main.cpp
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

#ifndef UPGRADE_H
#define UPGRADE_H

#include "robot.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// JUMPBOT(moving upgrade)-------------------------------------------------
class JumpBot : public GenericRobot {
private:
    int jumps;
public:
    JumpBot(string name, int x, int y);
    void jump(int newX, int newY, vector<vector<char>>& field, ofstream& outfile);    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;
};

// HIDEBOT(moving upgrade)-------------------------------------------------
class HideBot : public GenericRobot
{
    private:
        int hideChances;
        bool botHidden = false;

    public:
        HideBot(string name, int x, int y);
        void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
        void revert() override;
};

// LONGSHOTBOT(shooting upgrade)----------------------------------------------
class LongShotBot : public GenericRobot
{
private:
    int maxDistance = 3;
public:
    LongShotBot(string name, int x, int y);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;

};

//SEMIAUTOBOT(shooting upgrade)-------------------------------------------------
class SemiAutoBot : public GenericRobot
{
public:
    SemiAutoBot(string name, int x, int y);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;
};

//THIRTYSHOTBOT(shooting upgrade)-------------------------------------------------
class ThirtyShotBot : public GenericRobot {
public:
    ThirtyShotBot(string name, int x, int y);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;
};

//SCOUTBOT(seeing upgrade)-------------------------------------------------
class ScoutBot : public GenericRobot {
private:
    int scanUses;
public:
    ScoutBot(string name, int x, int y);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;
};

//TRACKBOT(seeing upgrade)-------------------------------------------------
class TrackBot : public GenericRobot {
private:
    int trackers;
    vector<pair<string, pair<int, int>>> trackedEnemies;
public:
    TrackBot(string name, int x, int y);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;
};

#endif