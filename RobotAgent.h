#pragma once
#include <string>
#include "GameBoard.h"

using std::string;

class RobotAgent
{
public:
    virtual ~RobotAgent() = default;

    virtual string getAgentName() = 0;
    virtual string getAgentCreator() = 0;
    virtual void getAgentMove(RobotMoveRequest &rmr, GameBoard::SROW *srs, GameBoard::ROW *lrs) = 0;
    virtual void setRobotColor(Color c) = 0;
    virtual RobotAgent *clone() = 0;
};