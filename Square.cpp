#include "Square.h"

// Initialize static instances
NotPresent Square::NotPresentInstance;
RockContent Square::RockInstance;
FogContent Square::FogInstance;
WallContent Square::WallInstance;
RobotContent AgentSquare::agentRobotRed = RobotContent(Red, Direction{0, 0}, None);
RobotContent AgentSquare::agentRobotBlue = RobotContent(Blue, Direction{0, 0}, None);

// Square implementation
Square::Square() {}

// ObscuredSquare implementation
ObscuredSquare::ObscuredSquare() : Square()
{
    // All pointers remain as NotPresentInstance
}

// AgentSquare implementation
// NOTE - 'Square' does not have a Color::color (but it has Conent squareColor which can be of type SquareColorContent ), only AgentSquare and InternalSquare do
AgentSquare::AgentSquare(Color squareColor, bool rockPresent, bool fogPresent, bool robotPresent, Color robotColor)
    : Square(), agentSquareColor(squareColor)
{

    SquareColor = &agentSquareColor;

    if (rockPresent)
    {
        Rock = &RockInstance;
    }

    if (fogPresent)
    {
        Fog = &FogInstance;
    }

    // TODO what to do if robot is present
    // if (robotPresent)
    // {
    //     Robot = &NotPresentInstance;
    // }
    if (robotPresent)
    {
        // Create a minimal RobotContent with just the robot color
        // Agents can see that a robot is present and what color it is
        // But they don't get direction or paint color details
        if (robotColor == Red)
        {
            Robot = &agentRobotRed;
        }
        else
        {
            Robot = &agentRobotBlue;
        }
    }
}

AgentSquare::AgentSquare(InternalSquare s) : Square()
{
    // Copy only the information agents are allowed to see
    Rock = s.Rock;
    Fog = s.Fog;
    Wall = s.Wall;

    // Copy square color
    agentSquareColor.setColor(s.getSquareColor());
    SquareColor = &agentSquareColor;

    // Copy robot info if present (but limited info for agents)
    Robot = s.Robot;
}

AgentSquare::AgentSquare() : Square(), agentSquareColor(Color::None)
{
    // All pointers remain as NotPresentInstance (inherited from Square)
}

// InternalSquare implementation
InternalSquare::InternalSquare() : Square()
{
    SquareColor = &internalSquareColor;
    internalSquareColor.setColor(Color::White);
}

void InternalSquare::setSquareColor(Color c)
{
    internalSquareColor.setColor(c);
}

Color InternalSquare::getSquareColor() const
{
    return internalSquareColor.getColor();
}

void InternalSquare::setRock(bool present)
{
    if (present)
    {
        Rock = &RockInstance;
    }
    else
    {
        Rock = &NotPresentInstance;
    }
}

void InternalSquare::setFog(bool present)
{
    if (present)
    {
        Fog = &FogInstance;
    }
    else
    {
        Fog = &NotPresentInstance;
    }
}

void InternalSquare::setWall(bool present)
{
    if (present)
    {
        Wall = &WallInstance;
    }
    else
    {
        Wall = &NotPresentInstance;
    }
}

void InternalSquare::setRobot(Color robotColor, Direction dir, Color paintColor)
{
    RobotInstance = RobotContent(robotColor, dir, paintColor);
    Robot = &RobotInstance;
}

void InternalSquare::removeRobot()
{
    RobotInstance = RobotContent(Color::None, Direction(0, 1), Color::None);
    Robot = &NotPresentInstance;
}

bool InternalSquare::hasRobot() const
{
    return Robot->isPresent();
}

const RobotContent *InternalSquare::getRobotContent() const
{
    return &RobotInstance;
}

Direction InternalSquare::getRobotDirection() const
{
    return RobotInstance.getDirection();
}

Color InternalSquare::getRobotPaintColor() const
{
    return RobotInstance.getPaintColor();
}

// const overloading?
RobotContent *InternalSquare::getRobotContent()
{
    return &RobotInstance;
}