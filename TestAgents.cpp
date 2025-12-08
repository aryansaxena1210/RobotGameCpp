#include "TestAgents.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

LazyAgent::LazyAgent()
    : robotColor(Color::Red), agentName("LazyAgent"), agentCreator("Student")
{
}

string LazyAgent::getAgentName()
{
    return agentName;
}

string LazyAgent::getAgentCreator()
{
    return agentCreator;
}

void LazyAgent::getAgentMove(RobotMoveRequest &rmr, GameBoard::SROW *srs, GameBoard::ROW *lrs)
{
    rmr.move = NoMove;
    rmr.fire = true;

    // 10% chance to request long range scan (optional - for variety)
    rmr.longrangescan = (std::rand() % 10 == 0);

    int turnChoice = std::rand() % 4;
    if (turnChoice == 0)
    {
        rmr.move = TurnLeft;
    }
    else if (turnChoice == 1)
    {
        rmr.move = TurnRight;
    }
}

void LazyAgent::setRobotColor(Color c)
{
    robotColor = c;
}

RobotAgent *LazyAgent::clone()
{
    LazyAgent *newAgent = new LazyAgent();
    newAgent->robotColor = this->robotColor;
    return newAgent;
}

// RandomAgent Implementation

RandomAgent::RandomAgent()
    : robotColor(Color::Blue), agentName("RandomAgent"), agentCreator("Student")
{
}

string RandomAgent::getAgentName()
{
    return agentName;
}

string RandomAgent::getAgentCreator()
{
    return agentCreator;
}

void RandomAgent::getAgentMove(RobotMoveRequest &rmr, GameBoard::SROW *srs, GameBoard::ROW *lrs)
{
    rmr.fire = false;
    rmr.longrangescan = false;

    // Build list of legal moves
    bool canMoveForward = false;
    bool canTurnLeft = false;
    bool canTurnRight = false;

    if (srs != nullptr)
    {
        // Check if forward square is legal (no rock, no wall, no robot)
        AgentSquare &forwardSquare = srs[0][1];
        canMoveForward = !forwardSquare.Rock->isPresent() &&
                         !forwardSquare.Wall->isPresent() &&
                         !forwardSquare.Robot->isPresent();

        canTurnLeft = true;
        canTurnRight = true;
    }

    // Count available moves
    int numMoves = (canMoveForward ? 1 : 0) + (canTurnLeft ? 1 : 0) + (canTurnRight ? 1 : 0);

    if (numMoves == 0)
    {
        // should never occur (fallback), as you can always turn
        rmr.move = NoMove;
        return;
    }

    // pick a random move
    int choice = std::rand() % numMoves;
    int counter = 0;

    if (canMoveForward)
    {
        if (counter == choice)
        {
            rmr.move = MoveForward;
            return;
        }
        counter++;
    }

    if (canTurnLeft)
    {
        if (counter == choice)
        {
            rmr.move = TurnLeft;
            return;
        }
        counter++;
    }

    if (canTurnRight)
    {
        if (counter == choice)
        {
            rmr.move = TurnRight;
            return;
        }
        counter++;
    }

    // Fallback (my execution should never reach here either)
    rmr.move = NoMove;
}

void RandomAgent::setRobotColor(Color c)
{
    robotColor = c;
}

RobotAgent *RandomAgent::clone()
{
    RandomAgent *newAgent = new RandomAgent();
    newAgent->robotColor = this->robotColor;
    return newAgent;
}