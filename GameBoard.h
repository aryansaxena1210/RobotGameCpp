#pragma once
#include "Util.h"
#include "Square.h"
#include "Content.h"
#include "Config.h"
#include <stdexcept>
#include <string>

enum RobotMove
{
    TurnLeft,
    TurnRight,
    MoveForward,
    NoMove
};

struct RobotMoveRequest
{
    RobotMove move;
    bool fire;
};

class GameBoard
{
public:
    static GameBoard &getInstance(const std::string &password);
    ~GameBoard();

    void resetBoard();
    void resetBoard(const Config &c);

    Square &getSquareContent(const Location &loc);
    bool moveRobot(RobotMoveRequest moveRequest, Color robotColor);
    bool paintBlobHit(Color shooterColor) const;
    void setSquareColor(const Location &loc, Color color);
    const RobotContent &getRobotContent(Color robotColor) const;

    int blueScore() const;
    int redScore() const;
    int turnNumber() const;

    AgentSquare **getLongRangeScan(Color robotColor) const;
    AgentSquare **getShortRangeScan(Color robotColor) const;

    void testPrintBoard() const;
    void testPrintSquare(const Location &loc) const;

    // ustility
    static const int BOARD_SZ = 15;

private:
    GameBoard(); // singleton
    GameBoard(const GameBoard &) = delete;
    GameBoard &operator=(const GameBoard &) = delete;

    void placeRobotsRandomly();
    bool inBounds(const Location &loc) const;
    Location robotLocation(Color robotColor) const;
    InternalSquare board[BOARD_SZ][BOARD_SZ];

    // robot state
    RobotContent redRobot;
    RobotContent blueRobot;
    Location redLoc;
    Location blueLoc;

    int turnNum;
    Config cfg;
    std::string password;
};