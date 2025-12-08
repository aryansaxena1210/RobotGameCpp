#pragma once
#include "Util.h"

class Content
{
public:
    virtual bool isPresent() const = 0;
    virtual ~Content() = default;
};

class NotPresent : public Content
{
public:
    bool isPresent() const;
};

class RockContent : public Content
{
public:
    bool isPresent() const;
};

class FogContent : public Content
{
public:
    bool isPresent() const;
};

class WallContent : public Content
{
public:
    bool isPresent() const;
};

class SquareColorContent : public Content
{
private:
    Color color;

public:
    SquareColorContent(Color c);
    bool isPresent() const;
    Color getColor() const;
    void setColor(Color c);
    const SquareColorContent &operator=(const SquareColorContent &rhs);
};

class RobotContent : public Content
{
private:
    Color color;
    Direction direction;
    Color paintColor;
    bool present;

public:
    RobotContent(Color c, Direction d, Color p);
    bool isPresent() const;
    Color getColor() const;
    Direction getDirection() const;
    Color getPaintColor() const;
    void setRobotDir(Direction d);
    void setRobotPaintColor(Color c);
    void setRobotColor(Color c);

    void setPresent(bool p);
    void setAgentColor(Color c);
};
