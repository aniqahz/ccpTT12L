 //base class of robot
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <vector>
  #include <utility> //for pair
 #include "robot.h"
 #include <fstream>
 using namespace std;


 //BASE ROBOT---------------------------------------------------------------
baseRobot::baseRobot(int x, int y) : PosX(x), PosY(y), isAlive(true), remainingLives(3) {}
pair<int,int> baseRobot:: getPosition() const {return {PosX,PosY};}
void baseRobot:: setPosition(int x,int y){PosX=x;PosY=y; }
bool baseRobot:: getAliveStatus() const { return isAlive; }
string baseRobot:: getRobotType() const { return robotType; }
void baseRobot ::takeDamage(){
  if(remainingLives>0){
    remainingLives--;
    cout<< name<< "took damage! Remaining lives:"<<remainingLives;
  }
}

 //MOVING ROBOT-------------------------------------------------------------
movingRobot::movingRobot(int x,int y): baseRobot(x,y) {}

 //SHOOTING ROBOT----------------------------------------------------
 shootingRobot::shootingRobot(int x,int y): baseRobot(x,y) {}

 //SEEING ROBOT----------------------------------------------------------
lookRobot::lookRobot(int x,int y): baseRobot(x,y) {}

//THINKING ROBOT--------------------------------------------------------
 thinkingRobot::thinkingRobot(int x,int y): baseRobot(x,y) {}

 //GENERIC ROBOT----------------------------------------------------
GenericRobot::GenericRobot(string rName, int x, int y): 
        baseRobot(x, y),
        movingRobot(x, y),
        shootingRobot(x, y),
        lookRobot(x, y),
        thinkingRobot(x, y),
       
        shells(10), // max 10 shells
        lives(3), // max 3 lives
        maxLives(3),
        upgradesUsed(0),
        hasMovingUpgrade(false),
        hasShootingUpgrade(false),
        hasSeeingUpgrade(false) 
    { 
        name = rName; //set the robot name
        robotType = "GenericRobot"; //set the robot type
        isAlive = true; //alive status
    };
   
    //THINK MECHANICS++++++++++++++++++++++++++++++++++++++++++
  void GenericRobot::think( vector<vector<char>>&field, vector<GenericRobot*>& robots){
    cout<<name<<" is thinking..."<<endl;
    int action=rand()%5; 
    int dx = rand() % 3 - 1; //randomly choose direction to fire (generates a number from 0-2 then substracts 1)
    int dy = rand() % 3 - 1; //randomly choose direction to fire
    switch(action){

    case 0:
   cout<<name<<" is firing...!"<<endl;
    fire(dx,dy, field, robots); //fire in a random direction
    break;
    case 1: 
     cout<<name<<" is looking around for enemies..."<<endl;
    look(PosX, PosY, field); //look at current position
    break;
     case 2: //look
    cout<<name<<" is moving..."<<endl;
    move(dx, dy, field); //move in a random direction
    break;
    case 3:
    cout<<name<<" is staying still..."<<endl;
    break;
    };cout<<endl;};
    
  
//LOOKING MECHANICS+++++++++++++++++++++++++++++
    void GenericRobot::look(int dx,int dy,  vector<vector<char>>&field)  {
    int newX,newY ;

    //to look around :
    for (int dx=-1; dx<=1; dx++){
      for (int dy=-1;dy<=1;dy++){
        newX = PosX + dx;
        newY = PosY + dy;
      }};
    
    if (newX>= 0 && newX<field.size() && newY>=0 && newY<field[0].size()){
      if (field[newX][newY] == '.'){
        cout<<name<<" : Coast is clear!"<<endl;
      }

      else{
        cout<<name<<": Enemy detected!"<<endl;
      }
    }
    else{
      newX=PosX;
      newY=PosY; //if out of bounds, stay put
      cout<<name<<" : Drifting away out of bounds...Staying put"<<endl;
    }};

  //FIRE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
    void GenericRobot::fire(int dx, int dy,  vector<vector<char>>&field,  vector<GenericRobot*>& robots) {
int targetX, targetY;
      if (PosX==0 && PosY==0 || PosX==dx && PosY==dy){ //prevents suicide wow #mentalawareness month
        cout<<name<<"Cannot fire from the origin!"<<endl;
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
          for (auto& robot: robots){
            if(robot !=this && robot-> getAliveStatus() ){
              auto [x,y] = robot -> getPosition();
              if ( x== targetX && y== targetY){
                cout<<name<<" : Shot" << robot->getrobotname()<< " at "<< targetX<<","<<targetY<<")! KaBaBoom!!"<<endl;
                robot-> takeDamage();
                return;
              }
            }
            else{
              cout<<name<<" : Missed at ("<< targetX<<","<<targetY<<")!"<<endl;
            }
          };
        }
        else{
          cout<<name<<": No enemy in range!"<<endl;
        }
      }
      else{
        cout<<name<<": Out of shells! Self-destructing now X___X"<<endl;
      };
    };

    

    //MOVE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
    void GenericRobot::move(int dx, int dy,  vector<vector<char>>& field)  {
      int newX = PosX + dx;
      int newY = PosY + dy;

      if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) { 
        field[PosX][PosY] = '.';
        PosX = newX;
        PosY = newY;
        field[PosX][PosY] = name[0];
        cout <<name<< " : Moved to position (" << PosX << "," << PosY << ")" << endl;
      }
      else {
        cout <<name<< " : Cannot move out of bounds!" << endl;
      }
    } 


//JUMP BOT-------------------------
jumpBot::jumpBot(string name,  vector<vector<char>>&field): GenericRobot(name,0,0), jumps(3){
  robotType="JumpBot";
  hasMovingUpgrade=true;//set the robot type
    
  };

  void jumpBot:: jump(int newX, int newY,  vector<vector<char>>& field) {
   if (jumps>0){
    cout<<name<<" upgraded! to jumpBot!"<<endl;
    if (newX>=0 && newX<field.size()&& newY>=0 && newY<field[0].size()){
      PosX=newX;
      PosY=newY;
      jumps--;
      cout<<name<< "(jumpbot) spawned at ("<<PosX<<","<<PosY<<")"<<endl;
      outfile<<name<< "(jumpbot) spawned at ("<<PosX<<","<<PosY<<")"<<endl;
    }
    else{
      cout<<name<<" Out of bounds! Cannot jump!"<<endl;
    };
   };};

     void jumpBot:: think( vector<vector<char>>& field, vector<GenericRobot*>& robots)  {
    if (jumps >0){
      int newX = rand() % field.size();
      int newY = rand() % field[0].size();
      jump(newX,newY, field);
    }
    else{
      cout<<"JumpBot has no jumps left!"<<endl;
      GenericRobot::think(field, robots); //fallback to generic thinking
    };

  };
