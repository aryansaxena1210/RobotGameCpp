#pragma once
#include "SquareInfo.h"

enum PBDirection
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

class RobotInfo
{
public:
    bool robotPresent;
    PBColor color;
    PBDirection direction;

    RobotInfo()
    {
        robotPresent = false;
        color = WHITE;
        direction = NORTH;
    }
};