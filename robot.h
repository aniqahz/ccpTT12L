/**********|**********|**********|
Program: robot.h
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


//base class of robot
#ifndef robot_h //if robot_h is not defined
#define robot_h //define it to prevent multiple inclusions

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility> //for pair
using namespace std;

enum actionType{NONE, FIRE, LOOK, MOVE, STAY};
class GenericRobot; //forward declaration

//BASE ROBOT---------------------------------------------------------------
class baseRobot{
    protected:
        //position, type,name, capabilities, one action per turn
        string name;
        string robotType; //type of bot (generic, etc...)
        int PosX,PosY; //position of bot currently
        bool isAlive; 
        int remainingLives; 
   
    public:
        baseRobot(){};
        baseRobot(int x, int y) ;
        virtual ~baseRobot() {}
        pair<int,int> getPosition() const;
        void setPosition(int x,int y);
        bool getAliveStatus() const;
        string getRobotType() const;
        virtual void takeDamage(ofstream& outfile);    
        string getrobotname() const { return name; }
        int getRemainingLives() const { return remainingLives; }
     
     
};

//MOVING ROBOT-------------------------------------------------------------
class movingRobot : virtual public baseRobot {
    public:
        movingRobot(int x,int y);
        virtual void move(int dx, int dy,  vector<vector<char>>& field, ofstream& outfile)=0;
};

//SHOOTING ROBOT----------------------------------------------------
class shootingRobot : virtual public baseRobot {
    public :
        shootingRobot(int x,int y);
        virtual void fire(int dx, int dy,  vector<vector<char>>&field,  vector<GenericRobot*>& robots, ofstream& outfile)=0;
};

//SEEING ROBOT----------------------------------------------------------
class lookRobot : virtual public baseRobot {
    public:
        lookRobot(int x,int y);
        virtual void look(int dx, int dy,  vector<vector<char>>&field, ofstream& outfile)=0;
};

//THINKING ROBOT--------------------------------------------------------
class thinkingRobot : virtual public baseRobot {
    public:
        thinkingRobot(int x,int y);
        virtual void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) = 0;
        virtual ~thinkingRobot() {}
};

// Generic robot
class GenericRobot : public movingRobot, public shootingRobot, public lookRobot, public thinkingRobot {
    protected:
        int shells; //max 10 per match
        int lives;  //max 3 respawns
        int maxLives = 3;
        int upgradesUsed;
        bool hasMovingUpgrade;
        bool hasShootingUpgrade;
        bool hasSeeingUpgrade;
    
    public:
    //getter and setter methods
        actionType lastaction = NONE; //initial action 
        GenericRobot(string rName, int x, int y);
        void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile ) override; //decides what action to take
        void look(int dx,int dy,  vector<vector<char>>&field, ofstream& outfile) override ;
        void fire(int dx, int dy,  vector<vector<char>>&field,  vector<GenericRobot*>& robots, ofstream& outfile) override;
        void move(int dx, int dy,  vector<vector<char>>& field, ofstream& outfile) override ;
};

//JUMP BOT-------------------------
class jumpBot : public  GenericRobot {
    private :
        int jumps; //max 3 jumps/match


    public : 
        jumpBot(string name,  vector<vector<char>>&field, ofstream& outfile);

    void jump(int newX, int newY,  vector<vector<char>>& field, ofstream& outfile);
    void think(vector<vector<char>>& field,vector<GenericRobot*>& robots, ofstream& outfile) override;
    
};

#endif //end