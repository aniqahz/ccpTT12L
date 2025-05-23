 //base class of robot
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
    baseRobot(int x, int y) :PosX(x), PosY(y), isAlive(true), remainingLives(3) {}
    

    pair<int,int> getPosition() const {return {PosX,PosY};}

      void setPosition(int x,int y)
      {PosX=x;PosY=y; }

     bool getAliveStatus() const { return isAlive; }
     string getRobotType() const { return robotType; }
    
  
 };

 //MOVING ROBOT-------------------------------------------------------------
 class movingRobot : virtual public baseRobot {
  public:
   movingRobot(int x,int y): baseRobot(x,y) {}
    virtual void move()=0;
 };

 //SHOOTING ROBOT----------------------------------------------------
 class shootingRobot : virtual public baseRobot {
   shootingRobot(int x,int y): baseRobot(x,y) {}
   virtual void fire(int x,int y)=0;
 };

 //SEEING ROBOT----------------------------------------------------------
 class lookRobot : virtual public baseRobot {
  public:
  lookRobot(int x,int y): baseRobot(x,y) {}
  virtual void look(int dx, int dy, const vector<vector<char>>&field)=0;
 };

//THINKING ROBOT--------------------------------------------------------
 class thinkingRobot : virtual public baseRobot {
  public:
  thinkingRobot(int x,int y): baseRobot(x,y) {}
  virtual void think()=0; 
 };

 //GENERIC ROBOT----------------------------------------------------
class GenericRobot : public movingRobot, public shootingRobot, public lookRobot, public thinkingRobot {
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
    : baseRobot(x, y), movingRobot(x, y), shootingRobot(x, y), lookRobot(x, y), thinkingRobot(x, y);
        baseRobot(x, y),
        movingRobot(x, y),
        shootingRobot(x, y),
        lookRobot(x, y),
        thinkingRobot(x, y),
        name(robotName),
        shells(10), // max 10 shells
        lives(3), // max 3 lives
        maxLives(3),
        upgradesUsed(0),
        hasMovingUpgrade(false),
        hasShootingUpgrade(false),
        hasSeeingUpgrade(false) {
    };


    void look(int dx,int dy, const vector<vector<char>>&field) override {
      int newX = PosX + dx;
    int newY = PosY + dy; //(robotsPositionX + x, robotsPositionY + y)

    //to look around :
    for (int dx=-1; dx<=1; dx++){
      for (int dy=-1;dy<=1;dy++){
        newX = PosX + dx;
        newY = PosY + dy;
      }
    }

    if (newX>= 0 && newX<field.size()&& newY>=0 && newY<field[0].size()){
      if (field[newX][newY] == '.'){
        cout<<"Coast is clear!"<<endl;
      }

      else{
        cout<<"Enemy detected!"<<endl;
      }
    }
    else{
      cout<<"Drifting away out of bounds..."<<endl;
    }}

    
    void think() override { //decides what action to take
    int action=rand()%4; 
    switch(action){
    case 0:
    cout<<"Moving to a new position..."<<endl;
    int dx=rand()%3-1; // -1,0,1
    int dy=rand()%3-1; // -1,0,1
    PosX+=dx;
    PosY+=dy;
    
    break;
    case 1: //fire
    cout<<"Shooting an enemy!..."<<endl;
    fire()
    break;
    case 2: //look
    cout<<"Looking around for enemies..."<<endl;
    break;
    case 3: //move
    cout<<"Moving..."<<endl;
    break;
    case 4:
    cout<<"Staying still..."<<endl;
    break;
    }};

    void fire(int x, int y) override{
      
      if (shells>0){
        shells--;
        cout<<"Firing at position ("<<x<<","<<y<<")"<<endl;}
        else{
        cout<<"Out of shells!"<<endl;
        }
      }

    }
};
       


//JUMP BOT-------------------------
/*class jumpBot : public movingRobot, public thinkingRobot{
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
}; */ 
