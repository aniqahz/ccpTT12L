 //base class of robot
#ifndef robot_h //if robot_h is not defined
#define robot_h //define it to prevent multiple inclusions
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <vector>
 #include <utility> //for pair
 using namespace std;

 //BASE ROBOT---------------------------------------------------------------
 class baseRobot{
    protected:
    //position, type,name, capabilities, one action per turn
    
    string robotType; //type of bot (generic, etc...)
    int PosX,PosY; //position of bot currently
    bool isAlive; 
    int remainingLives; 
   
    public:
    baseRobot(){};
    baseRobot(int x, int y) ;
    virtual ~baseRobot(){}
    pair<int,int> getPosition() const;
    void setPosition(int x,int y);
     bool getAliveStatus() const;
     string getRobotType() const;
     
     
 };

 //MOVING ROBOT-------------------------------------------------------------
 class movingRobot : virtual public baseRobot {
  public:
   movingRobot(int x,int y);
  virtual void move(int dx, int dy,  vector<vector<char>>& field)=0;
 };

 //SHOOTING ROBOT----------------------------------------------------
 class shootingRobot : virtual public baseRobot {
  public :
   shootingRobot(int x,int y);
   virtual void fire(int x,int y,  vector<vector<char>>&field)=0;
 };

 //SEEING ROBOT----------------------------------------------------------
 class lookRobot : virtual public baseRobot {
  public:
  lookRobot(int x,int y);
  virtual void look(int dx, int dy,  vector<vector<char>>&field)=0;
 };

//THINKING ROBOT--------------------------------------------------------
 class thinkingRobot : virtual public baseRobot {
  public:
  thinkingRobot(int x,int y);
  virtual void think( vector<vector<char>>& field)=0; 
 };

// Generic robot
class GenericRobot : public movingRobot, public shootingRobot, public lookRobot, public thinkingRobot {
protected:
    string name;
    int shells;
    int lives;
    int maxLives;
    bool hasMovingUpgrade = false;
    bool hasShootingUpgrade = false;
    bool hasSeeingUpgrade = false;
    int upgradesUsed = 0;

public:
//getter and setter methods

    GenericRobot(string rName, int x, int y);
    void think( vector<vector<char>>& field) override; //decides what action to take
    void look(int dx,int dy,  vector<vector<char>>&field) override ;
    void fire(int dx, int dy,  vector<vector<char>>&field) override;
    void move(int dx, int dy,  vector<vector<char>>& field) override ;
    bool applyUpgrade(UpgradeArea area) {
    if (upgradesUsed >= 3) {
        cout << name << ": Max upgrades reached!" << endl;
        return false;
    }

    if ((area == UpgradeArea::MOVING && hasMovingUpgrade) ||
        (area == UpgradeArea::SHOOTING && hasShootingUpgrade) ||
        (area == UpgradeArea::SEEING && hasSeeingUpgrade)) {
        cout << name << ": Upgrade area already chosen!" << endl;
        return false;
    }

    // Apply upgrade based on area, e.g.
    if (area == UpgradeArea::MOVING) {
        hasMovingUpgrade = true;
        // You might instantiate a new derived class like jumpBot here
        // or enable special abilities in this instance
    } else if (area == UpgradeArea::SHOOTING) {
        hasShootingUpgrade = true;
    } else if (area == UpgradeArea::SEEING) {
        hasSeeingUpgrade = true;
    }

    upgradesUsed++;
    cout << name << ": Upgrade applied in area " << (int)area << endl;
    return true;
};

//JUMP BOT-------------------------
class jumpBot : public  GenericRobot {
  private :
  int jumps; //max 3 jumps/match


  public : 
  jumpBot(string name,  vector<vector<char>>&field);

 void jump(int newX, int newY,  vector<vector<char>>& field);
 void think(vector<vector<char>>& field) override;
    
};
#endif //end