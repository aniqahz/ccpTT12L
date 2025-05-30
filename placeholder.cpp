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
    string rName;
    string robotType; //type of bot (generic, etc...)
    int PosX,PosY; //position of bot currently
    bool isAlive; 
    int remainingLives; 
   
    public:
    baseRobot(){};
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
    virtual void move(int dx, int dy, const vector<vector<char>>& field)=0;
 };

 //SHOOTING ROBOT----------------------------------------------------
 class shootingRobot : virtual public baseRobot {
  public :
   shootingRobot(int x,int y): baseRobot(x,y) {}
   virtual void fire(int x,int y, const vector<vector<char>>&field)=0;
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
  virtual void think(const vector<vector<char>>& field)=0; 
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
    GenericRobot(string rName, int x, int y): 
        baseRobot(x, y),
        movingRobot(x, y),
        shootingRobot(x, y),
        lookRobot(x, y),
        thinkingRobot(x, y),
        name(rName),
        shells(10), // max 10 shells
        lives(3), // max 3 lives
        maxLives(3),
        upgradesUsed(0),
        hasMovingUpgrade(false),
        hasShootingUpgrade(false),
        hasSeeingUpgrade(false) 
    { 
        robotType = "GenericRobot"; //set the robot type
        PosX = x; // initial position
        PosY = y; // initial position
        isAlive = true; //alive status
    };

    

    //THINK MECHANICS++++++++++++++++++++++++++++++++++++++++++
    void think(const vector<vector<char>>& field) override { //decides what action to take
    int action=rand()%5; 
    int dx = rand() % 3 - 1; //randomly choose direction to fire (generates a number from 0-2 then substracts 1)
    int dy = rand() % 3 - 1; //randomly choose direction to fire
    switch(action){

    case 0:
    cout<<"Moving to a new position..."<<endl;
    break;
    case 1: //fire
    cout<<"DECISION : FIRE!..."<<endl;
    fire(dx,dy, field);
    break;
    case 2: //look
    cout<<"Looking around for enemies..."<<endl;
    look(PosX, PosY, field); //look at current position
    break;
    case 3: //move
    cout<<"Moving..."<<endl;
    move(dx, dy, field); //move in a random direction
    break;
    case 4:
    cout<<"Staying still..."<<endl;
    break;
    }};

//LOOKING MECHANICS+++++++++++++++++++++++++++++
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

    if (newX>= 0 && newX<field.size() && newY>=0 && newY<field[0].size()){
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
  //FIRE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
    void fire(int dx, int dy, const vector<vector<char>>&field) override{

      if (PosX==0 && PosY==0 || PosX==dx && PosY==dy){ //prevents suicide wow #mentalawareness month
        cout<<"Cannot fire from the origin!"<<endl;
        return;
      }

      for (int dx=-1; dx<=1; dx++){
        for (int dy=-1;dy<=1;dy++){
        int targetX= PosX+dx;
        int targetY = PosY+dy;
      
      if (shells>0){ //if bot has more bullets
        shells--; 
        int probability = rand()%100;
        if (targetX>= 0 && targetY<field.size()&& targetY>=0 && targetY<field[0].size()){
          if (field[targetX][targetY] == '.'){
            if (probability>=70){
             cout<<"Firing at ("<< targetX<<","<<targetY<<")! KaBaBoom!!"<<endl;
            }
            else{
            cout<<"Missed at ("<< targetX<<","<<targetY<<")!"<<endl;
            }}
          else{
            cout<<"No enemy in range!";
          };
        }}
        
        else{
      cout<<"Out of shells! Self-destructing now X___X"<<endl;
        };
      }

    };};

    //MOVE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
    void move(int dx, int dy, const vector<vector<char>>& field) override {
      int newX=PosX+dx;
      int newY=PosY+dy;

      if (newX>=0 && newX<field.size()&&newY>=0 && newX<field[0].size()){ 
        PosX = newX;
        PosY = newY;
        cout<<"Moved to position ("<<PosX<<","<<PosY<<")"<<endl;
      }
      else{
        cout<<"Cannot move out of bounds!"<<endl;
      }
      
    }

  };
       


//JUMP BOT-------------------------
class jumpBot : public  GenericRobot {
  private :
  int jumps; //max 3 jumps/match


  public : 
  jumpBot(string name, const vector<vector<char>>&field ):GenericRobot(name, 0,0), jumps(3) {
    robotType = "JumpBot"; 
    bool hasMovingUpgrade=true;//set the robot type
    
  };

  void jump(int newX, int newY, const vector<vector<char>>& field) {
   if (jumps>0){
    if (newX>=0 && newX<field.size()&& newY>=0 && newY<field[0].size()){
      PosX=newX;
      PosY=newY;
      jumps--;
      cout<<"JumpBot spawned at ("<<PosX<<","<<PosY<<")"<<endl;
    }
    else{
      cout<<"Out of bounds! Cannot jump!"<<endl;
    };
   };};

     void think(const vector<vector<char>>& field) override {
    if (jumps >0){
      cout<<"JumpBot is thinking..."<<endl;
      int newX = rand() % field.size();
      int newY = rand() % field[0].size();
      jump(newX,newY, field);
    }
    else{
      cout<<"JumpBot has no jumps left!"<<endl;
      GenericRobot::think(field); //fallback to generic thinking
    };

  };
};

