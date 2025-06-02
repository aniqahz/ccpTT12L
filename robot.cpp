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
void baseRobot::setAliveStatus(bool status) {isAlive = status;}
string baseRobot:: getRobotType() const { return robotType; }

//Damage MECHANICS++++++++++++++++++++++++++++++++++++++++++++
void baseRobot::takeDamage(vector<vector<char>>& field, ofstream& outfile) {
    if (!isAlive) return;

    loseLife();
    markDead();

    // Clear from battlefield
    field[PosX][PosY] = '.';
    positionToRobot.erase({PosX, PosY});

    if (remainingLives > 0) {
        log(cout, outfile, getRobotName() + " was HIT and DESTROYED! Remaining lives: " + to_string(remainingLives));
        log(cout, outfile, getRobotName() + " has been destroyed and removed from the battlefield.");

        GenericRobot* derived = dynamic_cast<GenericRobot*>(this);
        if (derived && !derived->getIsQueuedForRespawn()) {
            respawnQueue.push(derived);
            derived->setIsQueuedForRespawn(true);
            log(cout, outfile, getRobotName() + " is added to respawn queue.");
        }
    } else {
        log(cout, outfile, getRobotName() + " was HIT and DESTROYED! Remaining lives: 0");
        log(cout, outfile, getRobotName() + " is permanently dead.");
    }
}

void baseRobot::loseLife() {
    if (remainingLives > 0) {
        remainingLives--;
        if (remainingLives == 0) {
            isAlive = false;
        }
    }
}

void baseRobot::markDead(){
    isAlive = false;
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
    upgradesUsed(0),
    hasMovingUpgrade(false),
    hasShootingUpgrade(false),
    hasSeeingUpgrade(false),
    isQueuedForRespawn(false),
    upgradeActive(false)

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
        fire(field, robots,outfile); //fire in a random direction
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
void GenericRobot::fire(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) {
    if (shells <= 0) {
        log(cout, outfile, name + ": Out of shells! Self-destructing now X___X");

        loseLife();
        markDead();

        field[PosX][PosY] = '.';
        positionToRobot.erase({PosX, PosY});

        log(cout, outfile, name + " has self-destructed due to running out of shells.");
        log(cout, outfile, name + " remaining lives: " + to_string(remainingLives));

        if (remainingLives > 0 && !getIsQueuedForRespawn()) {
            respawnQueue.push(this);
            setIsQueuedForRespawn(true);
            log(cout, outfile, name + " is added to respawn queue.");
        } else if (remainingLives == 0) {
            log(cout, outfile, name + " has no lives left and is permanently dead.");
        }

        return;
    }

    shells--;

    int dx = rand() % 3 - 1;
    int dy = rand() % 3 - 1;

    if (dx == 0 && dy == 0) {
        log(cout, outfile, name + " tried to fire at an invalid direction.");
        return;
    }

    int targetX = PosX + dx;
    int targetY = PosY + dy;

    if (targetX < 0 || targetX >= field.size() || targetY < 0 || targetY >= field[0].size()) {
        log(cout, outfile, name + ": Target out of bounds.");
        return;
    }

    bool hit = false;
    for (GenericRobot* robot : robots) {
        if (!robot) continue;  // Defensive: skip null or deleted robots
        if (robot == this) continue;     // Don't shoot yourself
        if (!robot->getAliveStatus()) continue;  // Only alive robots

        auto [rx, ry] = robot->getPosition();
        if (rx == targetX && ry == targetY) {
            int chance = rand() % 100;
            if (chance < 70) {
                robot->takeDamage(field, outfile);

                if (!robot->getAliveStatus()) {
                    field[rx][ry] = '.';
                    positionToRobot.erase({rx, ry});
                }

                awardUpgrade(robots, field, outfile);
            } else {
                log(cout, outfile, name + " MISSED " + robot->getRobotName() + " at (" + to_string(rx) + "," + to_string(ry) + ")");
            }

            hit = true;
            break;
        }
    }

    if (!hit) {
        log(cout, outfile, name + " fired at (" + to_string(targetX) + "," + to_string(targetY) + ") but no robot was there.");
    }
}


//MOVE MECHANICS++++++++++++++++++++++++++++++++++++++++++++
void GenericRobot::move(int dx, int dy, vector<vector<char>>& field, ofstream& outfile){
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

//Reset after Respawn---------------------------------------------------------
void GenericRobot::resetToGeneric() {
    shells = 10;
    isAlive = true;
    isQueuedForRespawn = false;
    upgradesUsed = 0;
    hasMovingUpgrade = false;
    hasShootingUpgrade = false;
    hasSeeingUpgrade = false;
    upgradeActive = false;
}

//Upgrade------------------------------------------------------------------
void GenericRobot::awardUpgrade(vector<GenericRobot*>& activeRobots, vector<vector<char>>& field, ofstream& outfile) {
    if (upgradesUsed >= 3 || upgradeActive) return;
    
    if (upgradesUsed >= 3) {
        log(cout, outfile, name + " has maxed out all upgrades.");
        return;
    }

    GenericRobot* upgraded = nullptr;
    vector<string> available;

    if (!hasMovingUpgrade) available.push_back("Moving");
    if (!hasShootingUpgrade) available.push_back("Shooting");
    if (!hasSeeingUpgrade) available.push_back("Seeing");

    if (available.empty()) return;

    string category = available[rand() % available.size()];
    log(cout, outfile, name + " is choosing an upgrade from: " + category);

    int x = PosX;
    int y = PosY;

    if (category == "Moving") {
        vector<string> choices = {"Jump", "Hide"};
        string selected = choices[rand() % choices.size()];
        log(cout, outfile, name + " received Moving upgrade: " + selected);

        if (selected == "Jump") upgraded = new JumpBot(name, x, y);
        else if (selected == "Hide") upgraded = new HideBot(name, x, y);
        upgraded->hasMovingUpgrade = true;

    } else if (category == "Shooting") {
        vector<string> choices = {"LongShot","SemiAuto","ThirtyShot"};
        string selected = choices[rand() % choices.size()];
        log(cout, outfile, name + " received Shooting upgrade: " + selected);

        if (selected == "LongShot") upgraded = new LongShotBot(name, x, y);
        else if (selected == "SemiAuto") upgraded = new SemiAutoBot(name, x, y);
        else if (selected == "ThirtyShot") upgraded = new ThirtyShotBot(name, x, y);
        upgraded->hasShootingUpgrade = true;

    } else if (category == "Seeing") {
        vector<string> choices = {"Scout", "Track"};
        string selected = choices[rand() % choices.size()];
        log(cout, outfile, name + " received Seeing upgrade: " + selected);

        if (selected == "Scout") upgraded = new ScoutBot(name, x, y);
        else if (selected == "Track") upgraded = new TrackBot(name, x, y);
        upgraded->hasSeeingUpgrade = true;
    }

    if (upgraded) {
        upgraded->setPosition(PosX, PosY);
        upgraded->setShells(shells);
        upgraded->upgradesUsed = upgradesUsed + 1;
        upgraded->setIsQueuedForRespawn(isQueuedForRespawn);
        upgraded->setAliveStatus(isAlive);

        // Preserve other upgrades if any
        if (!upgraded->hasMovingUpgrade) upgraded->hasMovingUpgrade = hasMovingUpgrade;
        if (!upgraded->hasShootingUpgrade) upgraded->hasShootingUpgrade = hasShootingUpgrade;
        if (!upgraded->hasSeeingUpgrade) upgraded->hasSeeingUpgrade = hasSeeingUpgrade;

        upgraded->upgradeActive = true;

        field[PosX][PosY] = name[0];
        positionToRobot[{PosX, PosY}] = upgraded;

        revertNextTurn.push_back(upgraded);
        replaceNextTurn.push_back({this, upgraded});

        log(cout, outfile, name + " will be upgraded to " + upgraded->getRobotType() + " next turn.");
    }
}

