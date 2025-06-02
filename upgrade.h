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
    void jump(int newX, int newY, const vector<vector<char>>& field, ofstream& outfile);
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
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