#pragma once
#include "Util.h"

enum Color
{
    Red,
    Blue,
    White,
    None
};

class Content // basically an interface but 'interface' keyword does not exist in cpp
{
public:
    virtual bool isPresent() const = 0;
    virtual ~Content() {}
};

class NotPresent : public Content
{
public:
    bool isPresent() const override { return false; }
};

class RockContent : public Content
{
public:
    bool isPresent() const override { return true; }
};

class FogContent : public Content
{
public:
    bool isPresent() const override { return true; }
};

class WallContent : public Content
{
public:
    bool isPresent() const override { return true; }
};

class SquareColorContent : public Content
{
private:
    Color color;

public:
    SquareColorContent(Color c = Color::None) : color(c) {}
    bool isPresent() const override { return color != Color::None; }
    Color getColor() const { return color; }
    void setColor(Color c) { color = c; }
};

class RobotContent : public Content
{
private:
    Color agentColor;
    Direction direction;
    Color paintColor;
    bool present;

public:
    RobotContent(Color c = Color::None, Direction d = Direction::North, Color p = Color::None, bool prs = false)
        : agentColor(c), direction(d), paintColor(p), present(prs) {}

    bool isPresent() const override { return present; }
    Color getColor() const { return agentColor; }
    Direction getDirection() const { return direction; }
    Color getPaintColor() const { return paintColor; }

    void setPresent(bool v) { present = v; }
    void setDirection(const Direction &d) { direction = d; }
    void setAgentColor(Color c) { agentColor = c; }
    void setPaintColor(Color p) { paintColor = p; }
};
