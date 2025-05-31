#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
using namespace std;

// Base class
class baseRobot {
protected:
    string robotName;
    string robotType;
    int PosX, PosY;
    bool isAlive;
    int remainingLives;

public:
    baseRobot();
    baseRobot(int x, int y);
    string getName() const { return robotName; }
    pair<int, int> getPosition() const;
    virtual void setPosition(int x, int y);
    bool getAliveStatus() const;
    string getRobotType() const;
    void loseLife();
    int getRemainingLives() const;
    void markDead();
    virtual void reset() = 0;
};

// Abstract capability classes
class movingRobot : virtual public baseRobot {
public:
    movingRobot(int x, int y);
    virtual void move(int dx, int dy, vector<vector<char>> &field, ofstream& outfile) = 0;
};

class shootingRobot : virtual public baseRobot {
public:
    shootingRobot(int x, int y);
    virtual bool fire(int x, int y, vector<vector<char>>& field, ofstream& outfile) = 0;  };

class lookRobot : virtual public baseRobot {
public:
    lookRobot(int x, int y);
    virtual void look(int dx, int dy, const vector<vector<char>> &field, ofstream& outfile) = 0;
};

class thinkingRobot : virtual public baseRobot {
public:
    thinkingRobot(int x, int y);
    virtual void think(vector<vector<char>> &field, ofstream& outfile) = 0;
};

// Generic robot
class GenericRobot : public movingRobot, public shootingRobot, public lookRobot, public thinkingRobot {
protected:
    string name;
    int shells;
    int lives;
    int maxLives;
    int upgradesUsed;
    bool hasMovingUpgrade, hasShootingUpgrade, hasSeeingUpgrade;

public:
    GenericRobot(string robotName, int x, int y);

    void think(vector<vector<char>> &field, ofstream& outfile) override;
    void look(int dx, int dy, const vector<vector<char>> &field, ofstream& outfile) override;
    bool fire(int dx, int dy, vector<vector<char>> &field, ofstream& outfile) override;
    void move(int dx, int dy, vector<vector<char>> &field, ofstream& outfile) override;
    void reset();
    void awardUpgrade(std::vector<GenericRobot*>& activeRobots, std::vector<std::vector<char>>& field, ofstream& outfile);
    void logStatus(ofstream& out) const;
};

#endif
