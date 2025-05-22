 //base class of robot
 #include <string>
 #include <cstdlib>
 using namespace std;

 //BASE ROBOT---------------------------------------------------------------
 class baseRobot{
    protected:
    //position, type,name, capabilities, one action per turn
    string robotName;
    string robotType; //type of bot (generic, etc...)
    int PosX,PosY; //position of bot
    bool isAlive; 
    int remainingLives; 
   
    public:
    baseRobot(int x, int y) :
     PosX(x), PosY(y), isAlive(true), remainingLives(3) {}
    virtual void think()=0; //decides what action to take
    virtual void move()=0; //moves robot
    virtual void fire(int x,int y)=0; //fires at a target
    virtual void look(int x,int y)=0; // reveals a nearby area


    pair<int,int> getPosition() const {
      return {PosX,PosY};}

      void setPosition(int x,int y)
      {PosX=x;
      PosY=y; }

     bool getAliveStatus() const { return isAlive; }
     string getRobotType() const { return robotType; }
    
  
 };

 //MOVING ROBOT-------------------------------------------------------------
 class movingRobot : virtual public baseRobot {
   public:
   movingRobot(int x,int y): baseRobot(x,y) {}
     
 };

 //SHOOTING ROBOT----------------------------------------------------
 class shootingRobot : virtual public baseRobot {
   shootingRobot(int x,int y): baseRobot(x,y) {}
 };

 //SEEING ROBOT----------------------------------------------------------
 class seeingRobot : virtual public baseRobot {
   public: 
  seeingRobot(int x,int y): baseRobot(x,y) {}
 };

//THINKING ROBOT--------------------------------------------------------
 class thinkingRobot : virtual public baseRobot {
   public :
  thinkingRobot(int x,int y): baseRobot(x,y) {}
 };

 //GENERIC ROBOT----------------------------------------------------
class GenericRobot : public movingRobot, public shootingRobot, public seeingRobot, public thinkingRobot {
private:
    string name;
    int shells; //max 10 per match
    int lives;  //max 3 respawns
    int maxLives = 3;
    int upgradesUsed;
    bool hasMovingUpgrade;
    bool hasShootingUpgrade;
    bool hasSeeingUpgrade;

public:
    GenericRobot(string robotName, int x, int y)
    : baseRobot(x, y), movingRobot(x, y), shootingRobot(x, y), seeingRobot(x, y), thinkingRobot(x, y);
        name = robotName;
        PosX = x;
        PosY = y;
        isAlive = true;
        shells = 10;
        lives = maxLives;
        upgradesUsed = 0;
        hasMovingUpgrade = false;
        hasShootingUpgrade = false;
        hasSeeingUpgrade = false;
         bool hasUpgradedInCategory[3] = {false, false, false}; // 0: Moving, 1: Shooting, 2: Seeing
        robotType = "GenericRobot";
    };


//JUMP BOT-------------------------
class jumpBot : public movingRobot, public thinkingRobot{
   private:
   string name;
   int maxX, maxY; //maximum space you can jump (depends on battlefield)
   bool upgraded;

   public:
    jumpBot(string robotName, int id, int battlefieldWidth, int battlefieldHeight) {
      name = robotName;
      PosX = rand() % battlefieldWidth;
      PosY = rand() % battlefieldHeight;
      isAlive = true;
      robotType = "JumpBot";
      maxX = battlefieldWidth;
      maxY = battlefieldHeight;
      upgraded = true; // Already upgraded from GenericRobot
    }
};
