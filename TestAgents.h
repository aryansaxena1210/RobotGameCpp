#pragma once
#include "RobotAgent.h"
#include "GameBoard.h"
#include <string>

using std::string;

class LazyAgent : public RobotAgent
{
private:
    Color robotColor;
    string agentName;
    string agentCreator;

public:
    LazyAgent();

    string getAgentName() override;
    string getAgentCreator() override;
    void getAgentMove(RobotMoveRequest &rmr, GameBoard::SROW *srs, GameBoard::ROW *lrs) override;
    void setRobotColor(Color c) override;
    RobotAgent *clone() override;
};

class RandomAgent : public RobotAgent
{
private:
    Color robotColor;
    string agentName;
    string agentCreator;

public:
    RandomAgent();

    string getAgentName() override;
    string getAgentCreator() override;
    void getAgentMove(RobotMoveRequest &rmr, GameBoard::SROW *srs, GameBoard::ROW *lrs) override;
    void setRobotColor(Color c) override;
    RobotAgent *clone() override;
};