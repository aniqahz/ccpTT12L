 //base class of robot
#ifndef robot_h //if robot_h is not defined
#define robot_h //define it to prevent multiple inclusions
#endif //end
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <vector>
 using namespace std;

 //BASE ROBOT---------------------------------------------------------------
 class baseRobot{
    protected:
    //position, type,name, capabilities, one action per turn
    string robotName;
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
  virtual void move(int dx, int dy, const vector<vector<char>>& field)=0;
 };

 //SHOOTING ROBOT----------------------------------------------------
 class shootingRobot : virtual public baseRobot {
  public :
   shootingRobot(int x,int y);
   virtual void fire(int x,int y, const vector<vector<char>>&field)=0;
 };

 //SEEING ROBOT----------------------------------------------------------
 class lookRobot : virtual public baseRobot {
  public:
  lookRobot(int x,int y);
  virtual void look(int dx, int dy, const vector<vector<char>>&field)=0;
 };

//THINKING ROBOT--------------------------------------------------------
 class thinkingRobot : virtual public baseRobot {
  public:
  thinkingRobot(int x,int y);
  virtual void think(const vector<vector<char>>& field)=0; 
 };

 //GENERIC ROBOT----------------------------------------------------
class GenericRobot : public movingRobot, public shootingRobot, public lookRobot, public thinkingRobot {
protected:
    string name;
    int shells; //max 10 per match
    int lives;  //max 3 respawns
    int maxLives = 3;
    int upgradesUsed;
    bool hasMovingUpgrade;
    bool hasShootingUpgrade;
    bool hasSeeingUpgrade;

public:
//getter and setter methods

    GenericRobot(string robotName, int x, int y);
    void think(const vector<vector<char>>& field) override; //decides what action to take
    void look(int dx,int dy, const vector<vector<char>>&field) override ;
    void fire(int dx, int dy, const vector<vector<char>>&field) override;
    void move(int dx, int dy, const vector<vector<char>>& field) override ;
};

//JUMP BOT-------------------------
class jumpBot : public  GenericRobot {
  private :
  int jumps; //max 3 jumps/match


  public : 
  jumpBot(string name, const vector<vector<char>>&field);

 void jump(int newX, int newY, const vector<vector<char>>& field);
 void think(const vector<vector<char>>& field) override;
    
};

