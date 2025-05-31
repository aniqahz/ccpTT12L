/**********|**********|**********|
Program: upgrade.h
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
