#pragma once
#include "Util.h"
#include "Square.h"
#include "Content.h"
#include "Config.h"
#include <stdexcept>
#include <string>

static const int BOARD_SZ = 5;

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
    bool longrangescan;
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
    bool paintBlobHit(Color robotColor, RobotMoveRequest moveRequest);
    void setSquareColor(const Location &loc, Color color);
    const RobotContent &getRobotContent(Color robotColor) const;

    int blueScore() const;
    int redScore() const;
    int turnNumber() const;

    typedef AgentSquare ROW[BOARD_SZ + 2];
    typedef AgentSquare SROW[3];
    // todo fix method signature using typdefs above (piazza)
    ROW *getLongRangeScan(Color robotColor) const;
    SROW *getShortRangeScan(Color robotColor) const;

    void testPrintBoard() const;
    void testPrintSquare(const Location &loc) const;

private:
    GameBoard(); // singleton
    GameBoard(const GameBoard &) = delete;
    GameBoard &operator=(const GameBoard &) = delete;

    void placeRobotsRandomly();
    bool inBounds(const Location &loc) const;
    Location robotLocation(Color robotColor) const;
    InternalSquare board[BOARD_SZ + 2][BOARD_SZ + 2];

    // robot state
    RobotContent redRobot;
    RobotContent blueRobot;
    Location redLoc;
    Location blueLoc;

    int turnNum;
    Config cfg;
    std::string password;
};