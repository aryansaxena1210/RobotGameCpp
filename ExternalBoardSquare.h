#pragma once
#include "BoardSquare.h"
#include "InternalBoardSquare.h"

class ExternalBoardSquare : public BoardSquare
{
private:
    SquareInfo squareInfo;
    RobotInfo robot;

public:
    ExternalBoardSquare(const InternalBoardSquare &internal);

    SquareInfo getSquareInfo() const override;
    RobotInfo getRobotInfo() const override;
};