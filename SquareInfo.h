#pragma once
#include "PBEnums.h"
#include "RobotInfo.h"

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