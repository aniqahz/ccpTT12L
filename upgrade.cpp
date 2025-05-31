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
#include "upgrade.h"
#include "robot.h"

#include <iostream>
#include <fstream>
#include <string>
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
        log(cout, outfile, name + " is now hidden somewhere......!");
    }
    else
    {
        log(cout,outfile, name + " doesn't have any hide chances left.");
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
        log(cout, outfile, name + " is firing in a long shot at (" + to_string(PosX) + ", " + to_string(PosY) + ")." << endl;
    }
    else
    {
        log(cout, outfile, name + " failed to reach the target point!");
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
        log(cout, outfile, name + " cannot fire at itself");
        return;
    }

    if (shells <= 0)
    {
        log(cout, outfile, name + " has no more shells left ! OH NOOO! ");
        return;
    }

    shells--;
    log(cout, outfile, name + " fires three shots at (" + to_string(PosX) +"," + to_string(PosY) + ")" );

    for (int i = 1; i <= 3; i++)
    {
        int hitChance = rand() % 100;
        if (hitChance < 70)
        {
            log(cout, outfile , " Shot " + i + ": Hit the target!" );
        }
        else
        {
            log(cout, outfile," Shot " + i +": Miss the target!" );
        }
    }
}
