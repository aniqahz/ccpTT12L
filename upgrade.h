#ifndef UPGRADE_H
#define UPGRADE_H

#include "robot.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class jumpBot : public GenericRobot {
private:
    int jumps;
public:
    jumpBot(string name, int x, int y);
    void jump(int newX, int newY, const vector<vector<char>>& field, ofstream& outfile);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;
};

class thirtyShotBot : public GenericRobot {
public:
    thirtyShotBot(string name, int x, int y);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;
};

class scoutBot : public GenericRobot {
private:
    int scanUses;
public:
    scoutBot(string name, int x, int y);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;
};

class trackBot : public GenericRobot {
private:
    int trackers;
    vector<pair<string, pair<int, int>>> trackedEnemies;
public:
    trackBot(string name, int x, int y);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void revert() override;
};

#endif
