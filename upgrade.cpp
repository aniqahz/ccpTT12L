/**********|**********|**********|
Program: upgrade.cpp
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
