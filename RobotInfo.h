#pragma once
#include "PBEnums.h"

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