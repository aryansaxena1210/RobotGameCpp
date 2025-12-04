#include "Content.h"

#include "Content.h"

// NotPresent implementation
bool NotPresent::isPresent() const
{
    return false;
}

// RockContent implementation
bool RockContent::isPresent() const
{
    return true;
}

// FogContent implementation
bool FogContent::isPresent() const
{
    return true;
}

// WallContent implementation
bool WallContent::isPresent() const
{
    return true;
}

// SquareColorContent implementation
SquareColorContent::SquareColorContent(Color c) : color(c) {}

bool SquareColorContent::isPresent() const
{
    return color != Color::None;
}

Color SquareColorContent::getColor() const
{
    return color;
}

void SquareColorContent::setColor(Color c)
{
    color = c;
}

// RobotContent implementation
RobotContent::RobotContent(Color c, Direction d, Color p)
    : color(c), direction(d), paintColor(p), present(c != Color::None) {}

bool RobotContent::isPresent() const
{
    return color != Color::None;
}

Color RobotContent::getColor() const
{
    return color;
}

Direction RobotContent::getDirection() const
{
    return direction;
}

Color RobotContent::getPaintColor() const
{
    return paintColor;
}

void RobotContent::setDirection(Direction d)
{
    direction = d;
}

void RobotContent::setPaintColor(Color c)
{
    paintColor = c;
}

void RobotContent::setPresent(bool p)
{
    present = p;
}

void RobotContent::setAgentColor(Color c)
{
    color = c;
}
// void RobotContent::setColor(Color c)
// {
//     color = c;
// }
