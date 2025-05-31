#ifndef upgrade_h
#define upgrade_h
#include <iostream>
#include <vector>
#include <string>
#include "robot.h"
using namespace std;

// HIDEBOT(moving upgrade)-------------------------------------------------
class HideBot : public GenericRobot
{
    private:
        int hideChances;
        bool botHidden = false;

    public:
        HideBot(string name, int x, int y);
        void startHide();
        bool botCurrentlyHide() const;
        void endTurn();
        void think(vector<vector<char>>& field);
};

// LONGSHOTBOT(shooting upgrade)----------------------------------------------
class LongShotBot : public GenericRobot
{
private:
    int maxDistance = 3;

public:
    LongShotBot(string name, int x, int y);
    void fire(int targetPointX, int targetPointY, const vector<vector<char>>& field);
};

//SEMIAUTOBOT(shooting upgrade)-------------------------------------------------
class SemiAutoBot : public GenericRobot
{
protected:
    int shells = 3;  // assuming a starting value

public:
    SemiAutoBot(string name, int x, int y);
    void fire(int dx, int dy, const vector<vector<char>>& field);
};

#endif
