#include <iostream>
#include "robot.h"
using namespace std;

//HideBot - moving (can hide three times in match, when the robot hides it cannot be hit by other robots)
class HideBot : public GenericRobot
{
    private :
        int hideChance = 3;
        bool botHidden = false;

    public :
        HideBot(string name, int x, int y) : GenericRobot(name, x, y)
        {}

        void startHide() 
        {
            if ( hideChance > 0 ) 
            {
                botHidden = true;
                hideChance--;
                cout << robotName << " is starting to hide !" << endl;
            }
            else
            {
                cout << robotName << "does not have any hide chances left." << endl;
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

};

//SemiAutoBot (each shell the robot fires is now considered as three consecutive shots into one location
//and each shot has a 70% probability to hit and destroy another robot)
class SemiAutoBot : public GenericRobot
{

};
