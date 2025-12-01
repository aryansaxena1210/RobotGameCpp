#pragma once
#include "RobotInfo.h"

enum PBSquareContent
{
    EMPTY,
    ROCK,
    FOG,
    ROCKFOG,
    WALL
};

enum PBColor
{
    RED,
    BLUE,
    WHITE
};

class SquareInfo
{
public:
    PBSquareContent content;
    PBColor color;
    RobotInfo robot;

    SquareInfo()
    {
        content = EMPTY;
        color = WHITE;
        robot.robotPresent = false;
        robot.color = WHITE;
        robot.direction = NORTH;
    }
};