#pragma once
#include "BoardSquare.h"

class InternalBoardSquare : public BoardSquare
{
private:
    PBSquareContent content;
    PBColor color;
    RobotInfo robot;

public:
    InternalBoardSquare();

    void setContent(PBSquareContent c);
    void setColor(PBColor c);
    void setRobot(const RobotInfo &r);

    SquareInfo getSquareInfo() const override;
    RobotInfo getRobotInfo() const override;
};
