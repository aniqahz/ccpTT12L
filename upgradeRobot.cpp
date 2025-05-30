#include "upgradeRobot.h"
#include "robot.h"
#include <cstdlib>

// HideBot definitions
HideBot::HideBot(string robotName, int x, int y) : GenericRobot(robotName, x, y)
{
    robotType = "HideBot";
}

void HideBot::startHide()
{
    if (hideChance > 0)
    {
        botHidden = true;
        hideChance--;
        cout << robotName << " is now hidden somewhere!" << endl;
    }
    else
    {
        cout << robotName << " doesn't have any hide chances left." << endl;
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

// LongShotBot definitions
LongShotBot::LongShotBot(string robotName, int x, int y) : GenericRobot(robotName, x, y)
{
    robotType = "LongShotBot";
}

void LongShotBot::fire(int targetPointX, int targetPointY, const vector<vector<char>>& field)
{
    int distance = abs(targetPointX - PosX) + abs(targetPointY - PosY);

    if (distance <= maxDistance && !(targetPointX == PosX && targetPointY == PosY))
    {
        cout << robotName << " is firing in a long shot at (" << targetPointX << ", " << targetPointY << ")." << endl;
    }
    else
    {
        cout << robotName << " failed to reach the target point!" << endl;
    }
}

// SemiAutoBot definitions
SemiAutoBot::SemiAutoBot(string robotName, int x, int y) : GenericRobot(robotName, x, y)
{
    robotType = "SemiAutoBot";
}

void SemiAutoBot::fire(int distanceX, int distanceY, const vector<vector<char>>& field)
{
    if (PosX == distanceX && PosY == distanceY)
    {
        cout << robotName << " cannot fire at itself" << endl;
        return;
    }

    if (shells <= 0)
    {
        cout << robotName << " has no more shells left!" << endl;
        return;
    }

    shells--;
    cout << robotName << " fires three shots at (" << distanceX << "," << distanceY << ")" << endl;

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
