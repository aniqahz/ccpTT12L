//UPGRADE ROBOT
#include <iostream>
#include <string>
#include "upgrade.h"
#include "robot.h"
#include <cstdlib>
using namespace std;

// HIDEBOT------------------------------------------------------------------
HideBot::HideBot(string name, int x, int y) : GenericRobot(name, x, y)
{
    robotType = "HideBot";
    hasMovingUpgrade = true; // to set the robot type
};

void HideBot::startHide()
{
    if (hideChances > 0)
    {
        botHidden = true;
        hideChances--;
        cout << name << " is now hidden somewhere!" << endl;
    }
    else
    {
        cout << name << " doesn't have any hide chances left." << endl;
    }
}

bool HideBot::botCurrentlyHide() const
{
    return botHidden;
}

void HideBot::endTurn()
{
    botHidden = false;
}

// LONGSHOTBOT-------------------------------------------------------------------
LongShotBot::LongShotBot(string name, int x, int y) : GenericRobot(name, x, y)
{
    robotType = "LongShotBot";
    hasShootingUpgrade = true;
};

void LongShotBot::fire(int targetPointX, int targetPointY, const vector<vector<char>>& field)
{
    int distance = abs(targetPointX - PosX) + abs(targetPointY - PosY);

    if (distance <= maxDistance && !(targetPointX == PosX && targetPointY == PosY))
    {
        cout << name << " is firing in a long shot at (" << targetPointX << ", " << targetPointY << ")." << endl;
    }
    else
    {
        cout << name << " failed to reach the target point!" << endl;
    }
}

// SEMIAUTOBOT---------------------------------------------------------------------
SemiAutoBot::SemiAutoBot(string name, int x, int y) : GenericRobot(name, x, y)
{
    robotType = "SemiAutoBot";
    hasShootingUpgrade = true;
};

void SemiAutoBot::fire(int distanceX, int distanceY, const vector<vector<char>>& field)
{
    if (PosX == distanceX && PosY == distanceY)
    {
        cout << name << " cannot fire at itself" << endl;
        return;
    }

    if (shells <= 0)
    {
        cout << name << " has no more shells left!" << endl;
        return;
    }

    shells--;
    cout << name << " fires three shots at (" << distanceX << "," << distanceY << ")" << endl;

    for (int i = 1; i <= 3; i++)
    {
        int hitChance = rand() % 100;
        if (hitChance < 70)
        {
            cout << " Shot " << i << ": Hit the target!" << endl;
        }
        else
        {
            cout << " Shot " << i << ": Miss the target!" << endl;
        }
    }
}
