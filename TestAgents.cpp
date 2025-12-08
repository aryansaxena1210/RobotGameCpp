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

// AntiRandom agent implementation
AntiRandom::AntiRandom()
    : robotColor(Color::None),
      agentName("AntiRandomAgent"),
      agentCreator("Student"),
      moveCounter(0),
      preferredDirection(Direction::North)
{
}

string AntiRandom::getAgentName()
{
    return agentName;
}

string AntiRandom::getAgentCreator()
{
    return agentCreator;
}

void AntiRandom::getAgentMove(RobotMoveRequest &rmr, GameBoard::SROW *srs, GameBoard::ROW *lrs)
{
    rmr.fire = false;
    rmr.longrangescan = false;

    if (srs == nullptr)
    {
        rmr.move = NoMove;
        return;
    }

    moveCounter++;

    // Strategy: Move in sweeping pattern, turning when hitting obstacles
    // This covers territory efficiently and avoids getting stuck

    // Check forward square (srs[0][1] is forward since scan is robot-oriented)
    AgentSquare &forwardSquare = srs[0][1];
    bool canMoveForward = !forwardSquare.Rock->isPresent() &&
                          !forwardSquare.Wall->isPresent() &&
                          !forwardSquare.Robot->isPresent();

    // Check right square
    AgentSquare &rightSquare = srs[1][2];
    bool canMoveRight = !rightSquare.Rock->isPresent() &&
                        !rightSquare.Wall->isPresent() &&
                        !rightSquare.Robot->isPresent();

    // Check left square
    AgentSquare &leftSquare = srs[1][0];
    bool canMoveLeft = !leftSquare.Rock->isPresent() &&
                       !leftSquare.Wall->isPresent() &&
                       !leftSquare.Robot->isPresent();

    // Aggressive strategy: Keep moving forward, turn right when blocked
    // This creates a sweeping pattern that covers lots of territory
    if (canMoveForward)
    {
        rmr.move = MoveForward;

        // Occasionally fire if we've been moving straight for a while
        if (moveCounter % 15 == 0)
        {
            rmr.fire = true;
        }
    }
    else if (canMoveRight)
    {
        // Can't go forward, turn right
        rmr.move = TurnRight;
    }
    else if (canMoveLeft)
    {
        // Can't go forward or right, turn left
        rmr.move = TurnLeft;
    }
    else
    {
        // Stuck! Turn around
        rmr.move = TurnRight;
    }

    // Check if opponent robot is visible and in line of sight
    // Fire if we see them (extra aggressive)
    if (srs[0][1].Robot->isPresent())
    {
        // Robot directly in front
        rmr.fire = true;
    }
}

void AntiRandom::setRobotColor(Color c)
{
    robotColor = c;
}

RobotAgent *AntiRandom::clone()
{
    AntiRandom *newAgent = new AntiRandom();
    newAgent->robotColor = this->robotColor;
    newAgent->moveCounter = 0;
    return newAgent;
}

// Tracker Agent Implementation
TrackerAgent::TrackerAgent()
    : robotColor(Color::None),
      opponentColor(Color::None),
      agentName("TrackerAgent"),
      agentCreator("Student"),
      turnsSinceLastScan(0),
      lastKnownOpponentLocation(0, 0),
      hasOpponentLocation(false)
{
}

string TrackerAgent::getAgentName()
{
    return agentName;
}

string TrackerAgent::getAgentCreator()
{
    return agentCreator;
}

void TrackerAgent::getAgentMove(RobotMoveRequest &rmr, GameBoard::SROW *srs, GameBoard::ROW *lrs)
{
    rmr.fire = false;
    rmr.longrangescan = false;

    if (srs == nullptr)
    {
        rmr.move = NoMove;
        return;
    }

    turnsSinceLastScan++;

    // Determine opponent color
    if (opponentColor == Color::None)
    {
        opponentColor = (robotColor == Color::Red) ? Color::Blue : Color::Red;
    }

    // Strategy 1: Check if opponent is visible in short range scan and fire
    if (srs[0][1].Robot->isPresent())
    {
        // Opponent directly in front - FIRE!
        rmr.fire = true;
        rmr.move = NoMove;
        return;
    }

    // Strategy 2: Use long range scan every 20 turns to locate opponent
    if (turnsSinceLastScan >= 20)
    {
        rmr.longrangescan = true;
        turnsSinceLastScan = 0;
    }

    // Strategy 3: If we have long range scan data, find opponent location
    if (lrs != nullptr)
    {
        hasOpponentLocation = false;

        // Search entire board for opponent robot
        for (int i = 0; i < BOARD_SZ + 2; i++)
        {
            for (int j = 0; j < BOARD_SZ + 2; j++)
            {
                if (lrs[i][j].Robot->isPresent())
                {
                    // Found a robot - check if it's the opponent
                    // Note: We can't safely cast AgentSquare's Robot pointer
                    // So we just note that we found a robot location
                    hasOpponentLocation = true;
                    lastKnownOpponentLocation = Location(i, j);
                    // Don't break - keep looking in case there are both robots
                }
            }
        }
    }

    // Strategy 4: Simple movement with firing
    // Check what's around us and move intelligently
    AgentSquare &forwardSquare = srs[0][1];
    bool canMoveForward = !forwardSquare.Rock->isPresent() &&
                          !forwardSquare.Wall->isPresent() &&
                          !forwardSquare.Robot->isPresent();

    AgentSquare &rightSquare = srs[1][2];
    bool canMoveRight = !rightSquare.Rock->isPresent() &&
                        !rightSquare.Wall->isPresent() &&
                        !rightSquare.Robot->isPresent();

    AgentSquare &leftSquare = srs[1][0];
    bool canMoveLeft = !leftSquare.Rock->isPresent() &&
                       !leftSquare.Wall->isPresent() &&
                       !leftSquare.Robot->isPresent();

    // Movement strategy: Keep moving forward, turn when blocked
    if (canMoveForward)
    {
        rmr.move = MoveForward;

        // Fire occasionally while moving
        if (turnsSinceLastScan % 8 == 0)
        {
            rmr.fire = true;
        }
    }
    else if (canMoveRight)
    {
        rmr.move = TurnRight;
    }
    else if (canMoveLeft)
    {
        rmr.move = TurnLeft;
    }
    else
    {
        // Stuck - turn around
        rmr.move = TurnRight;
    }
}

void TrackerAgent::setRobotColor(Color c)
{
    robotColor = c;
}

RobotAgent *TrackerAgent::clone()
{
    TrackerAgent *newAgent = new TrackerAgent();
    newAgent->robotColor = this->robotColor;
    newAgent->opponentColor = this->opponentColor;
    return newAgent;
}
