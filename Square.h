#pragma once
#include "Content.h"
#include "Util.h"

class InternalSquare;

class Square
{
public:
    Content *Rock = &NotPresentInstance;
    Content *Fog = &NotPresentInstance;
    Content *SquareColor = &NotPresentInstance;
    Content *Robot = &NotPresentInstance;
    Content *Wall = &NotPresentInstance;

    static NotPresent NotPresentInstance;
    static RockContent RockInstance;
    static FogContent FogInstance;
    static WallContent WallInstance;

    Square();
    virtual ~Square() = default;
};

class ObscuredSquare : public Square
{
public:
    ObscuredSquare();
};

class AgentSquare : public Square
{
private:
    SquareColorContent agentSquareColor = SquareColorContent(Color::None);

    static RobotContent agentRobotRed;
    static RobotContent agentRobotBlue;

public:
    AgentSquare(Color squareColor, bool rockPresent, bool fogPresent, bool robotPresent, Color robotColor);
    AgentSquare(InternalSquare s);
    AgentSquare();
};

class InternalSquare : public Square
{
private:
    SquareColorContent internalSquareColor = SquareColorContent(Color::None);
    RobotContent RobotInstance = RobotContent(Color::None, Direction{0, 1}, Color::None);
    RobotContent internalRobotContent = RobotContent(Color::None, Direction::North, Color::None);

public:
    InternalSquare();

    // Additional helper methods
    void setSquareColor(Color c);
    Color getSquareColor() const;
    void setRock(bool present);
    void setFog(bool present);
    void setWall(bool present);
    void setRobot(Color robotColor, Direction dir, Color paintColor);
    void removeRobot();
    bool hasRobot() const;
    const RobotContent *getRobotContent() const;
    RobotContent *getRobotContent();
    Direction getRobotDirection() const;
    Color getRobotPaintColor() const;
    // friend class GameBoard;
};