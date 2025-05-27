#ifndef ROBOT_H
#define ROBOT_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
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
    pair<int, int> getPosition() const;
    void setPosition(int x, int y);
    bool getAliveStatus() const;
    string getRobotType() const;
};

// Abstract capability classes
class movingRobot : virtual public baseRobot {
public:
    movingRobot(int x, int y);
    virtual void move(int dx, int dy, const vector<vector<char>> &field) = 0;
};

class shootingRobot : virtual public baseRobot {
public:
    shootingRobot(int x, int y);
    virtual void fire(int x, int y, const vector<vector<char>> &field) = 0;
};

class lookRobot : virtual public baseRobot {
public:
    lookRobot(int x, int y);
    virtual void look(int dx, int dy, const vector<vector<char>> &field) = 0;
};

class thinkingRobot : virtual public baseRobot {
public:
    thinkingRobot(int x, int y);
    virtual void think(const vector<vector<char>> &field) = 0;
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

    void think(const vector<vector<char>> &field) override;
    void look(int dx, int dy, const vector<vector<char>> &field) override;
    void fire(int dx, int dy, const vector<vector<char>> &field) override;
    void move(int dx, int dy, const vector<vector<char>> &field) override;
};

#endif
