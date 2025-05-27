 //base class of robot
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <vector>
 #include "robot.h"
 using namespace std;

 //BASE ROBOT---------------------------------------------------------------
baseRobot::baseRobot(int x, int y) : PosX(x), PosY(y), isAlive(true), remainingLives(3) {}
pair<int,int> baseRobot:: getPosition() const {return {PosX,PosY};}
void baseRobot:: setPosition(int x,int y){PosX=x;PosY=y; }
bool baseRobot:: getAliveStatus() const { return isAlive; }
string baseRobot:: getRobotType() const { return robotType; }
 //MOVING ROBOT-------------------------------------------------------------
movingRobot::movingRobot(int x,int y): baseRobot(x,y) {}

 //SHOOTING ROBOT----------------------------------------------------
 shootingRobot::shootingRobot(int x,int y): baseRobot(x,y) {}

 //SEEING ROBOT----------------------------------------------------------
lookRobot::lookRobot(int x,int y): baseRobot(x,y) {}

//THINKING ROBOT--------------------------------------------------------
 thinkingRobot::thinkingRobot(int x,int y): baseRobot(x,y) {}

 //GENERIC ROBOT----------------------------------------------------
GenericRobot::GenericRobot(string robotName, int x, int y): 
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
        hasSeeingUpgrade(false) 
    { 
        robotType = "GenericRobot"; //set the robot type
        isAlive = true; //alive status
    };
   
    //THINK MECHANICS++++++++++++++++++++++++++++++++++++++++++
  void GenericRobot::think(const vector<vector<char>>&field){
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
    };};
  
//LOOKING MECHANICS+++++++++++++++++++++++++++++
    void GenericRobot::look(int dx,int dy, const vector<vector<char>>&field)  {
    int newX,newY ;

    //to look around :
    for (int dx=-1; dx<=1; dx++){
      for (int dy=-1;dy<=1;dy++){
        newX = PosX + dx;
        newY = PosY + dy;
      }};
    
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
    }};
  //FIRE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
    void GenericRobot::fire(int dx, int dy, const vector<vector<char>>&field) {
int targetX, targetY;
      if (PosX==0 && PosY==0 || PosX==dx && PosY==dy){ //prevents suicide wow #mentalawareness month
        cout<<"Cannot fire from the origin!"<<endl;
        return;
      }

      for (int dx=-1; dx<=1; dx++){
        for (int dy=-1;dy<=1;dy++){
         targetX= PosX+dx;
         targetY = PosY+dy;};};
      
      if (shells>0){ //if bot has more bullets
        shells--; 
        int probability = rand()%100;
        if (targetX>= 0 && targetX<field.size()&& targetY>=0 && targetY<field[0].size()){
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
      };

    

    //MOVE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
    void GenericRobot::move(int dx, int dy, const vector<vector<char>>& field)  {
      int newX = PosX + dx;
      int newY = PosY + dy;

      if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) { 
        PosX = newX;
        PosY = newY;
        cout << "Moved to position (" << PosX << "," << PosY << ")" << endl;
      }
      else {
        cout << "Cannot move out of bounds!" << endl;
      }
    } 


//JUMP BOT-------------------------
jumpBot::jumpBot(string name, const vector<vector<char>>&field): GenericRobot(name,0,0), jumps(3){
  robotType="JumpBot";
  hasMovingUpgrade=true;//set the robot type
    
  };

  void jumpBot:: jump(int newX, int newY, const vector<vector<char>>& field) {
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

     void jumpBot:: think(const vector<vector<char>>& field)  {
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

