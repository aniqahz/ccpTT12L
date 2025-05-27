#ifndef UPGRADE_H
#define UPGRADE_H
 
 #include "robot.h"
 #include <iostream>
 #include <string>
 #include <cstdlib>
 #include <vector>
 using namespace std;

// JumpBot
class jumpBot : public GenericRobot {
private:
    int jumps;

public:
    jumpBot(string name, const vector<vector<char>> &field);
    void jump(int newX, int newY, const vector<vector<char>> &field);
    void think(const vector<vector<char>> &field) override;
};

#endif