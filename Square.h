#pragma once
#include "Content.h"
#include <array>
#include <stdexcept>

class Square
{
    // convert 'every __<Rock,Fog,etc>__ IS A grid-square'   to
    //  "every square HAS A ___"
    //  i know, a lot more member variables but what would you rather, more (O(n)) nmemeber variables (one each) or more subclasses O(2^n - combinitorial)?

public:
    Content *Rock;
    Content *Fog;
    Content *SquareColor;
    Content *Robot;
    Content *Wall;

    static NotPresent NotPresentInstance; // this is NOT just declaration, but rather initializartion? WRONG! static member hai, will not be initialized
    static RockContent RockInstance;
    static FogContent FogInstance;
    static WallContent WallInstance;

    Square();
    virtual ~Square() = default;
};

class ObscuredSquare : public Square
{
public:
    ObscuredSquare();
};

class AgentSquare : public Square
{
public:
    SquareColorContent agentSquareColor;
    AgentSquare();
    AgentSquare(const Square &s); // copy allowed limited info
};

class InternalSquare : public Square
{
public:
    SquareColorContent internalSquareColor;
    RobotContent internalRobotContent;

    InternalSquare();
};
