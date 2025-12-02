#include "InternalBoardSquare.h"

InternalBoardSquare::InternalBoardSquare()
{
    content = EMPTY;
    color = WHITE;
    robot.robotPresent = false;
}

void InternalBoardSquare::setContent(PBSquareContent c) { content = c; }
void InternalBoardSquare::setColor(PBColor c) { color = c; }
void InternalBoardSquare::setRobot(const RobotInfo &r) { robot = r; }

SquareInfo InternalBoardSquare::getSquareInfo() const
{
    SquareInfo s;
    s.content = this->content;
    s.color = this->color;
    s.robot = this->robot;
    return s;
}

RobotInfo InternalBoardSquare::getRobotInfo() const
{
    return robot;
}
