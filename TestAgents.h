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

class AntiRandom : public RobotAgent
{
private:
    Color robotColor;
    string agentName;
    string agentCreator;
    int moveCounter;
    Direction preferredDirection;

public:
    AntiRandom();

    string getAgentName() override;
    string getAgentCreator() override;
    void getAgentMove(RobotMoveRequest &rmr, GameBoard::SROW *srs, GameBoard::ROW *lrs) override;
    void setRobotColor(Color c) override;
    RobotAgent *clone() override;
};

class TrackerAgent : public RobotAgent
{
private:
    Color robotColor;
    Color opponentColor;
    string agentName;
    string agentCreator;
    int turnsSinceLastScan;
    Location lastKnownOpponentLocation;
    bool hasOpponentLocation;

public:
    TrackerAgent();

    string getAgentName() override;
    string getAgentCreator() override;
    void getAgentMove(RobotMoveRequest &rmr, GameBoard::SROW *srs, GameBoard::ROW *lrs) override;
    void setRobotColor(Color c) override;
    RobotAgent *clone() override;
};