#include <iostream>
#include "robot.h"
#include <cstdlib>
#include <vector>
using namespace std;

//HideBot - moving (can hide three times in match, when the robot hides it cannot be hit by other robots)
class HideBot : public GenericRobot
{
    private :
        int hideChance = 3;
        bool botHidden = false;

    public :
        HideBot(string robotName, int x, int y) : GenericRobot(robotName, x, y)
        {   
            robotType ="HideBot"; 
        }

        void startHide() 
        {
            if ( hideChance > 0 ) 
            {
                botHidden = true;
                hideChance--;
                cout << robotName << " is now hidden somewhere !" << endl;
            }
            else
            {
                cout << robotName << "doesn't have any hide chances left." << endl;
            }
        }

        bool botCurrentlyHide() const
        {
            return botHidden;
        }

        void endTurn() {
            botHidden = false;
        }
};

//LongShotBot (can fire up to three unit distance away from its location)
class LongShotBot : public GenericRobot
{
    private :
        int maxDistance = 3;

    public :
        LongShotBot(string robotName , int x, int y) : GenericRobot(robotName, x, y)
        {
            robotType = "LongShotBot" ;
        }

        void fire(int targetPointX, int targetPointY, const vector<vector<char>>& field) override
        {
            int distance = abs(targetPointX - PosX) + abs(targetPointY - PosY); // x + y 

            if ( distance <= maxDistance && !(targetPointX == PosX && targetPointY == PosY)) //x + y <=3
            {
                cout << robotName << "is firing in a long shot at (" << targetPointX << ", " << targetPointY  << " ). " << endl;
            }

            else
            {
                cout << robotName << "failed to reach the target point ! " << endl;
            }
        }
};

//SemiAutoBot (each shell the robot fires is now considered as three consecutive shots into one location
//and each shot has a 70% probability to hit and destroy another robot)
class SemiAutoBot : public GenericRobot
{
    protected:
         int shells;
    public :
        SemiAutoBot(string robotName, int x, int y) : GenericRobot(robotName, x, y)
        {
            robotType = "SemiAutoBot";
        }
        
        void fire(int distanceX, int distanceY, const vector<vector<char>>&field) override
        {
            if (PosX == distanceX && PosY == distanceY)
            {
                cout << robotName << "cannot fire at itself" << endl;
                return ;
            }

            if (shells <=0) 
            {
                cout << robotName << "has no more shells left !" << endl;
                return;
            }

            shells--;   
            cout << robotName << "fires three shots at (" << distanceX << "," << distanceY  << ")" << endl;

            for (int i = 1; i <= 3; i++)
            {
                int hitChance = rand() % 100;
                if (hitChance < 70)
                {
                    cout << " Shot " << i << ": Hit the target !" << endl;
                }
                else
                {
                    cout << " Shot " << i << ": Miss the target !" << endl;
                }
            }
        }
};
