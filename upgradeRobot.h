#ifndef UPGRADE_ROBOT_H
#define UPGRADE_ROBOT_H

#include <iostream>
#include <vector>
#include "robot.h"
using namespace std;

class HideBot : public GenericRobot
{
private:
    int hideChance = 3;
    bool botHidden = false;

public:
    HideBot(string robotName, int x, int y);
    void startHide();
    bool botCurrentlyHide() const;
    void endTurn();
};

class LongShotBot : public GenericRobot
{
private:
    int maxDistance = 3;

public:
    LongShotBot(string robotName, int x, int y);
    void fire(int targetPointX, int targetPointY, const vector<vector<char>>& field);
};

class SemiAutoBot : public GenericRobot
{
protected:
    int shells = 3;  // assuming a starting value

public:
    SemiAutoBot(string robotName, int x, int y);
    void fire(int distanceX, int distanceY, const vector<vector<char>>& field);
};

#endif
