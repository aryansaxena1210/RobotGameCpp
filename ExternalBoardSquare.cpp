#include "ExternalBoardSquare.h"

ExternalBoardSquare::ExternalBoardSquare(const InternalBoardSquare &internal)
{
    squareInfo = internal.getSquareInfo();
    robot = internal.getRobotInfo();
}

SquareInfo ExternalBoardSquare::getSquareInfo() const
{
    return squareInfo;
}

RobotInfo ExternalBoardSquare::getRobotInfo() const
{
    return robot;
}