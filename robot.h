#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>

using namespace std;

enum actionType { NONE, FIRE, LOOK, MOVE, STAY };

class GenericRobot; // forward declaration for circular dependency

// --- BASE ROBOT ------------------------------------------------------------
class baseRobot {
protected:
    string name;
    string robotType;
    int PosX, PosY;
    bool isAlive;
    int remainingLives;

public:
    baseRobot() = default;
    baseRobot(int x, int y);
    virtual ~baseRobot() {}

    pair<int, int> getPosition() const;
    virtual void setPosition(int x, int y);
    bool getAliveStatus() const;
    void setAliveStatus(bool status);
    string getRobotType() const;
    string getRobotName() const { return name; }
    int getRemainingLives() const { return remainingLives; }

    virtual void takeDamage(vector<vector<char>>& field, ofstream& outfile);
    void loseLife();
    void markDead();
    virtual void resetToGeneric() = 0;
};

// --- MOVING ROBOT ----------------------------------------------------------
class movingRobot : virtual public baseRobot {
public:
    movingRobot(int x, int y);
    virtual void move(int dx, int dy, vector<vector<char>>& field, ofstream& outfile) = 0;
};

// --- SHOOTING ROBOT --------------------------------------------------------
class shootingRobot : virtual public baseRobot {
public:
    shootingRobot(int x, int y);
    virtual void fire(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) = 0;
};

// --- SEEING ROBOT ----------------------------------------------------------
class lookRobot : virtual public baseRobot {
public:
    lookRobot(int x, int y);
    virtual void look(int dx, int dy, vector<vector<char>>& field, ofstream& outfile) = 0;
};

// --- THINKING ROBOT --------------------------------------------------------
class thinkingRobot : virtual public baseRobot {
public:
    thinkingRobot(int x, int y);
    virtual void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) = 0;
    virtual ~thinkingRobot() {}
};

// --- GENERIC ROBOT ---------------------------------------------------------
class GenericRobot : public movingRobot, public shootingRobot, public lookRobot, public thinkingRobot {
protected:
    int shells;
    int upgradesUsed;
    bool hasMovingUpgrade;
    bool hasShootingUpgrade;
    bool hasSeeingUpgrade;
    bool isQueuedForRespawn;
    bool upgradeActive;

public:
    actionType lastaction = NONE;

    GenericRobot(string rName, int x, int y);

    // Core actions
    void think(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void look(int dx, int dy, vector<vector<char>>& field, ofstream& outfile) override;
    void fire(vector<vector<char>>& field, vector<GenericRobot*>& robots, ofstream& outfile) override;
    void move(int dx, int dy, vector<vector<char>>& field, ofstream& outfile) override;
    void resetToGeneric() override;

    // Upgrade system
    void awardUpgrade(vector<GenericRobot*>& activeRobots, vector<vector<char>>& field, ofstream& outfile);
    virtual void revert() {} // For temporary upgrades

    // Accessors
    bool getIsQueuedForRespawn() const { return isQueuedForRespawn; }
    void setIsQueuedForRespawn(bool status) { isQueuedForRespawn = status; }

    int getShells() const { return shells; }
    void setShells(int s) { shells = s; }
};

#endif