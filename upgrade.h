#ifndef UPGRADE_H
#define UPGRADE_H

#include "robot.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>

using namespace std;

class jumpBot : public GenericRobot {
private:
    int jumps;

public:
    jumpBot(string name, const vector<vector<char>> &field);
    void jump(int newX, int newY, const vector<vector<char>> &field, ofstream& outfile);
    void think(vector<vector<char>> &field, ofstream& outfile) override;
    void setPosition(int x, int y) override;
};

class thirtyShotBot : public GenericRobot {
public:
    thirtyShotBot(string name, const vector<vector<char>> &field);
    void think(vector<vector<char>>& field, ofstream& outfile) override;
    void setPosition(int x, int y) override;

};

class scoutBot : public GenericRobot {
private:
    int scanUses;

public:
    scoutBot(string name, const vector<vector<char>>& field);
    void think(vector<vector<char>>& field, ofstream& outfile) override;
    void setPosition(int x, int y) override;
};

class trackBot : public GenericRobot {
private:
    int trackers;
    vector<pair<string, pair<int, int>>> trackedEnemies;

public:
    trackBot(string name, const vector<vector<char>>& field);
    void think(vector<vector<char>>& field, ofstream& outfile) override;
    void setPosition(int x, int y) override;
};


#endif
