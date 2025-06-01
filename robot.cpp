//base class of robot
#include "robot.h" 
#include "battlefield.h"
#include "upgrade.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility> //for pair
#include <fstream>
 
 using namespace std;

//BASE ROBOT---------------------------------------------------------------
baseRobot::baseRobot(int x, int y) : PosX(x), PosY(y), isAlive(true), remainingLives(3) {}
pair<int,int> baseRobot:: getPosition() const {return {PosX,PosY};}
void baseRobot:: setPosition(int x,int y){PosX=x;PosY=y; }
bool baseRobot:: getAliveStatus() const { return isAlive; }
string baseRobot:: getRobotType() const { return robotType; }

void baseRobot::takeDamage(ofstream& outfile) {
    if (remainingLives > 0) {
        remainingLives--;
        if (remainingLives <= 0) {
            isAlive = false;
        }
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
    if (!getAliveStatus()) return;
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
    } 
    while (dx == 0 && dy == 0); //prevent standing still

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
    log(cout, outfile, "");
};
    

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

//FIRE MECHANICS+++++++++++++++++++++++++++++
void GenericRobot::fire(int dx, int dy, vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    // Check if robot has shells left
    if (shells <= 0) {
        log(cout, outfile, name + ": Out of shells! Self-destructing now X___X");
        isAlive = false;

        // Remove from battlefield
        field[PosX][PosY] = '.';
        log(cout, outfile, name + " has been removed from the battlefield due to self-destruction.");

        return;
    }

    // Prevent firing at self or invalid direction
    if ((dx == 0 && dy == 0) || abs(dx) > 1 || abs(dy) > 1) {
        log(cout, outfile, name + " tried to fire at an invalid direction.");
        return;
    }

    // Compute target location based on direction
    int targetX = PosX + dx;
    int targetY = PosY + dy;

    // Boundary check: target must be within field
    if (targetX < 0 || targetX >= field.size() || targetY < 0 || targetY >= field[0].size()) {
        log(cout, outfile, name + ": Target out of bounds.");
        return;
    }

    // Use up one shell
    shells--;

    bool hit = false; // flag to track if any enemy is hit

    // Loop through all robots to check if any is at target location
    for (GenericRobot* robot : robots) {
        if (robot != this && robot->getAliveStatus()) {
            auto [rx, ry] = robot->getPosition();

            // Check if this robot is at the target position
            if (rx == targetX && ry == targetY) {
                int probability = rand() % 100;

                if (probability < 70) {
                    // Successful hit
                    robot->takeDamage(outfile);

                        log(cout, outfile, name + " fired at (" + to_string(rx) + "," + to_string(ry) + ") — HIT " + robot->getrobotname() + 
                        "! Remaining lives: " + to_string(robot->getRemainingLives()));

                    if (!robot->getAliveStatus()) {
                        log(cout, outfile, robot->getrobotname() + " has been destroyed and removed from the battlefield.");
                        auto [deadX, deadY] = robot->getPosition();
                        field[deadX][deadY] = '.'; // clear battlefield
                    }
                } 
                
                else { // Missed
                    log(cout, outfile, name + " MISSED " + robot->getrobotname() + " at (" + to_string(rx) + "," + to_string(ry) + ")");
                }

                hit = true;
                break; // Stop checking — only one robot per spot
            }
        }
    }

    // If no robot found in the spot
    if (!hit) {
        log(cout, outfile, name + " fired at (" + to_string(targetX) + "," + to_string(targetY) + ") but no robot was there.");
    }
}
    
//MOVE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
void GenericRobot::move(int dx, int dy, vector<vector<char>>& field, ofstream& outfile) {
    // If dx and dy are both 0, robot stays still
    if (dx == 0 && dy == 0) {
        log(cout, outfile, name + " decides to stay still.");
        return;
    }

    // Compute new position
    int newX = PosX + dx;
    int newY = PosY + dy;

    // Bounds check
    if (newX >= 0 && newX < field.size() && newY >= 0 && newY < field[0].size()) {
        if (field[newX][newY] == '.') {
            // Move is valid — update the field and robot's position
            field[PosX][PosY] = '.';         // Clear old position
            PosX = newX;                     // Update X
            PosY = newY;                     // Update Y
            field[PosX][PosY] = name[0];     // Mark new position with robot's symbol

            log(cout, outfile, name + " moved to (" + to_string(PosX) + "," + to_string(PosY) + ")");
        } else {
            // Can't move — spot is already occupied
            log(cout, outfile, name + " cannot move — spot is occupied.");
        }
    } else {
        // Can't move — out of bounds
        log(cout, outfile, name + " cannot move — out of bounds.");
    }
}


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