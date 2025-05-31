 //base class of robot
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <vector>
  #include <utility> //for pair
 #include "robot.h"
 #include <fstream>
 #include "battlefield.h"
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
  void GenericRobot::think( vector<vector<char>>&field, vector<GenericRobot*>& robots, ofstream& outfile){
    log(cout, outfile, name + " is thinking...");
    int action;
    do {
    action = rand() % 4 +1; // 1 = move, 2 = fire, 3 = look, 4 = stay (to avoid NONE action)
    } 
    while (action == lastaction); //to avoid getting same action as before
   
    int dx, dy;
    do {
    dx = rand() % 3 - 1;
    dy = rand() % 3 - 1;
    } while (dx == 0 && dy == 0); //prevent standing still

    switch(action){

    case 1: //fire
   log(cout, outfile, name + " is firing...!");
    fire(dx,dy, field, robots,outfile); //fire in a random direction
   lastaction = FIRE;
    break;
    case 2: //look
    log(cout, outfile, name + " is looking around for enemies...");
    look(PosX, PosY, field,outfile); //look at current position
    lastaction = LOOK;
    break;
     case 3: //move
    log(cout, outfile, name + " is moving...");
    move(dx, dy, field, outfile); //move in a random direction
   lastaction = MOVE;
    break;
    case 4: //stay still
    log(cout, outfile, name + " is staying still...");
    lastaction = STAY;
    break;
    };
    log(cout, outfile, "");};
    
  
//LOOKING MECHANICS+++++++++++++++++++++++++++++
    void GenericRobot::look(int dx,int dy,  vector<vector<char>>&field, ofstream& outfile) { 
    int newX,newY ;

    //to look around :
    for (int dx=-1; dx<=1; dx++){
      for (int dy=-1;dy<=1;dy++){
        newX = PosX + dx;
        newY = PosY + dy;
      }};
    
    if (newX>= 0 && newX<field.size() && newY>=0 && newY<field[0].size()){
      if (field[newX][newY] == '.'){
       log(cout, outfile, name + " : Coast is clear!");
      }

      else{
         log(cout, outfile, name + " : Enemy spotted nearby!");
      }
    }
    else{
      newX=PosX;
      newY=PosY; //if out of bounds, stay put
      cout<<name<<" : Drifting away out of bounds...Staying put"<<endl;
    }};

  //FIRE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
   void GenericRobot::fire(int dx, int dy, vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    // Prevent shooting from origin or into self
    if ((PosX == 0 && PosY == 0) || (dx == 0 && dy == 0)) {
        log(cout, outfile, name + " cannot fire from the origin or at itself!");
        return;
    }

    // Calculate target coordinates
    int targetX = PosX + dx;
    int targetY = PosY + dy;

    if (shells <= 0) {
        log(cout, outfile, name + ": Out of shells! Self-destructing now X___X");
        isAlive = false;
        return;
    }

    // Check if target is within field bounds
    if (targetX < 0 || targetX >= field.size() || targetY < 0 || targetY >= field[0].size()) {
        log(cout, outfile, name + ": Target out of bounds.");
        return;
    }

    shells--; // Use a shell
    int probability = rand() % 100;

    // Simulate hit with probability (e.g., 70% chance to hit)
    bool hit = false;
    for (auto& robot : robots) {
        if (robot != this && robot->getAliveStatus()) {
            auto [x, y] = robot->getPosition();
            if (x == targetX && y == targetY) {
                if (probability < 70) {
                    robot->takeDamage();
                    log(cout, outfile, name + " : Hit " + robot->getrobotname() + " at (" + to_string(targetX) + "," + to_string(targetY) + ")!");
                    hit = true;
                } else {
                    log(cout, outfile, name + " : Missed at (" + to_string(targetX) + "," + to_string(targetY) + ")!");
                }
                break;
            }
        }
    }

    if (!hit) {
        log(cout, outfile, name + " : No enemy in range at (" + to_string(targetX) + "," + to_string(targetY) + ")!");
    }
}
    

    //MOVE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
    void GenericRobot::move(int dx, int dy,  vector<vector<char>>& field, ofstream& outfile)  {
      int newX = PosX + dx;
      int newY = PosY + dy;

      if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) { 
        field[PosX][PosY] = '.';
        PosX = newX;
        PosY = newY;
        field[PosX][PosY] = name[0];
        log(cout, outfile, name + " : Moved to position (" + to_string(PosX) + "," + to_string(PosY) + ")");      }
      else {
        log(cout, outfile, name + " : Cannot move out of bounds!");
    
      }
    };


//JUMP BOT-------------------------
jumpBot::jumpBot(string name,  vector<vector<char>>&field, ofstream& outfile): GenericRobot(name,0,0), jumps(3){
  robotType="JumpBot";
  hasMovingUpgrade=true;//set the robot type
    
  };

  void jumpBot:: jump(int newX, int newY,  vector<vector<char>>& field, ofstream& outfile) {
   if (jumps>0){
    cout<<name<<" upgraded! to jumpBot!"<<endl;
    if (newX>=0 && newX<field.size()&& newY>=0 && newY<field[0].size()){
      PosX=newX;
      PosY=newY;
      jumps--;
      log(cout, outfile, name + " jumped to position (" + to_string(PosX) + "," + to_string(PosY) + ")");
    }
    else{
      cout<<name<<" Out of bounds! Cannot jump!"<<endl;
    };
   };
  };

     void jumpBot:: think( vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile)  {
    if (jumps >0){
      int newX = rand() % field.size();
      int newY = rand() % field[0].size();
      jump(newX,newY, field, outfile);
    }
    else{
      log(cout, outfile, name + " has no jumps left! Falling back to generic thinking.");
      GenericRobot::think(field, robots, outfile); //fallback to generic thinking
    };

  };
