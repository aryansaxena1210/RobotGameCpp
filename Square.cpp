#include "Square.h"

// static instances
NotPresent Square::NotPresentInstance;
RockContent Square::RockInstance;
FogContent Square::FogInstance;
WallContent Square::WallInstance;

Square::Square()
{
    Rock = &NotPresentInstance;
    Fog = &NotPresentInstance;
    SquareColor = &NotPresentInstance;
    Robot = &NotPresentInstance;
    Wall = &NotPresentInstance;
}

ObscuredSquare::ObscuredSquare() : Square()
{
    // Obscured squares keep everything NotPresent to hide info
}

AgentSquare::AgentSquare() : Square(), agentSquareColor(Color::None)
{
    SquareColor = &agentSquareColor;
}

AgentSquare::AgentSquare(const Square &s) : Square(), agentSquareColor(Color::None)
{
    // Copy only allowed info: square color, fog/rock/wall presence, robot presence/color but NOT internal-only data
    // Copy color if present
    if (s.SquareColor->isPresent())
    {
        // attempt dynamic cast to SquareColorContent (safe if our design)
        const SquareColorContent *sc = dynamic_cast<const SquareColorContent *>(s.SquareColor);
        if (sc)
            agentSquareColor.setColor(sc->getColor());
        SquareColor = &agentSquareColor;
    }
    // For obstructions and robot we keep boolean presence info by keeping pointers to shared instances
    if (s.Rock->isPresent())
        Rock = &RockInstance;
    if (s.Fog->isPresent())
        Fog = &FogInstance;
    if (s.Wall->isPresent())
        Wall = &WallInstance;

    // Robot: if present, create a local RobotContent (non-shared) with visible info
    if (s.Robot->isPresent())
    {
        const RobotContent *rc = dynamic_cast<const RobotContent *>(s.Robot);
        if (rc)
        {
            // create a new RobotContent on the heap and assign Robot pointer
            RobotContent *rcopy = new RobotContent(rc->getColor(), Direction::North, rc->getPaintColor(), true);
            Robot = rcopy;
        }
    }
}

InternalSquare::InternalSquare() : Square(), internalSquareColor(Color::None), internalRobotContent(Color::None, Direction::North, Color::None, false)
{
    SquareColor = &internalSquareColor;
    Robot = &internalRobotContent;
}
