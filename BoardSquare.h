#pragma once
#include "RobotInfo.h"
#include "SquareInfo.h"

class BoardSquare
{
public:
    virtual SquareInfo getSquareInfo() const = 0;
    virtual RobotInfo getRobotInfo() const = 0;
    virtual ~BoardSquare() {}
};
