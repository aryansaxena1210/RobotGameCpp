#include "GameBoard.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>

using std::string;

GameBoard &GameBoard::getInstance(const std::string &password)
{
    static GameBoard instance; // created once
    if (password != "xyzzy")
        throw std::invalid_argument("Invalid password");
    return instance;
}

GameBoard::GameBoard() : redRobot(Color::None, Direction::North, Color::None, false),
                         blueRobot(Color::None, Direction::North, Color::None, false),
                         redLoc(0, 0), blueLoc(0, 1), turnNum(0), cfg()
{
    // seed RNG
    std::srand((unsigned)std::time(nullptr));
    // initialize board squares (internal default constructor already sets not present)
    for (int r = 0; r < BOARD_SZ; ++r)
        for (int c = 0; c < BOARD_SZ; ++c)
            board[r][c] = InternalSquare();
}

GameBoard::~GameBoard()
{
    // clean up any AgentSquare-created RobotContent objects if allocated by getLongRange/ShortRange (none here)
}

void GameBoard::resetBoard()
{
    cfg = Config(); // default
    resetBoard(cfg);
}

void GameBoard::resetBoard(const Config &c)
{
    cfg = c;
    turnNum = 0;

    // clear board
    for (int r = 0; r < BOARD_SZ; ++r)
    {
        for (int cc = 0; cc < BOARD_SZ; ++cc)
        {
            board[r][cc] = InternalSquare();
            // initial squares are white by default
            board[r][cc].internalSquareColor.setColor(Color::White);
        }
    }

    // place walls at edges
    for (int i = 0; i < BOARD_SZ; ++i)
    {
        board[0][i].Wall = &Square::WallInstance;
        board[BOARD_SZ - 1][i].Wall = &Square::WallInstance;
        board[i][0].Wall = &Square::WallInstance;
        board[i][BOARD_SZ - 1].Wall = &Square::WallInstance;
    }

    // place rocks
    int rockLower = cfg.getRockLowerBound();
    int rockUpper = cfg.getRockUpperBound();
    int rocks = rockLower + (std::rand() % (std::max(1, rockUpper - rockLower + 1)));
    for (int k = 0; k < rocks; ++k)
    {
        int r = 1 + (std::rand() % (BOARD_SZ - 2));
        int c = 1 + (std::rand() % (BOARD_SZ - 2));
        board[r][c].Rock = &Square::RockInstance;
    }

    // place fog
    int fogLower = cfg.getFogLowerBound();
    int fogUpper = cfg.getFogUpperBound();
    int fogs = fogLower + (std::rand() % (std::max(1, fogUpper - fogLower + 1)));
    for (int k = 0; k < fogs; ++k)
    {
        int r = 1 + (std::rand() % (BOARD_SZ - 2));
        int c = 1 + (std::rand() % (BOARD_SZ - 2));
        // fog can coexist with rock (spec allows that)
        board[r][c].Fog = &Square::FogInstance;
    }

    // set all squares white
    for (int r = 0; r < BOARD_SZ; ++r)
        for (int c = 0; c < BOARD_SZ; ++c)
            board[r][c].internalSquareColor.setColor(Color::White);

    // place robots randomly on empty squares
    placeRobotsRandomly();
}

void GameBoard::placeRobotsRandomly()
{
    // simple placement: find two empty squares (not rock/wall)
    bool placedRed = false, placedBlue = false;
    while (!placedRed)
    {
        int r = 1 + (std::rand() % (BOARD_SZ - 2));
        int c = 1 + (std::rand() % (BOARD_SZ - 2));
        if (!board[r][c].Rock->isPresent() && !board[r][c].Wall->isPresent())
        {
            redLoc = Location(r, c);
            redRobot.setPresent(true);
            redRobot.setAgentColor(Color::Red);
            redRobot.setDirection(Direction::North);
            board[r][c].internalRobotContent = redRobot;
            board[r][c].Robot = &board[r][c].internalRobotContent;
            placedRed = true;
        }
    }
    while (!placedBlue)
    {
        int r = 1 + (std::rand() % (BOARD_SZ - 2));
        int c = 1 + (std::rand() % (BOARD_SZ - 2));
        if (!board[r][c].Rock->isPresent() && !board[r][c].Wall->isPresent() && Location(r, c) != redLoc)
        {
            blueLoc = Location(r, c);
            blueRobot.setPresent(true);
            blueRobot.setAgentColor(Color::Blue);
            blueRobot.setDirection(Direction::South);
            board[r][c].internalRobotContent = blueRobot;
            board[r][c].Robot = &board[r][c].internalRobotContent;
            placedBlue = true;
        }
    }
}

bool GameBoard::inBounds(const Location &loc) const
{
    return loc.row >= 0 && loc.row < BOARD_SZ && loc.col >= 0 && loc.col < BOARD_SZ;
}

Square &GameBoard::getSquareContent(const Location &loc)
{
    if (!inBounds(loc))
        throw std::out_of_range("Location out of bounds");
    return board[loc.row][loc.col];
}

bool GameBoard::moveRobot(RobotMoveRequest moveRequest, Color robotColor)
{
    // find robot and its location
    Location loc = (robotColor == Red ? redLoc : blueLoc);
    RobotContent *rc = dynamic_cast<RobotContent *>(board[loc.row][loc.col].Robot);
    if (!rc || !rc->isPresent())
        return false;

    Direction dir = rc->getDirection();

    // apply turn/move
    if (moveRequest.move == TurnLeft)
    {
        Direction nd = dir.RotateLeft90();
        rc->setDirection(nd);
        // update stored robot content and board
        board[loc.row][loc.col].internalRobotContent.setDirection(nd);
        board[loc.row][loc.col].Robot = &board[loc.row][loc.col].internalRobotContent;
    }
    else if (moveRequest.move == TurnRight)
    {
        Direction nd = dir.RotateRight90();
        rc->setDirection(nd);
        board[loc.row][loc.col].internalRobotContent.setDirection(nd);
        board[loc.row][loc.col].Robot = &board[loc.row][loc.col].internalRobotContent;
    }
    else if (moveRequest.move == MoveForward)
    {
        Location nloc = loc + Location(dir.dRow, dir.dCol);
        // check bounds
        if (!inBounds(nloc))
        {
            // running into wall (outside) -> lose: treat as failure
            return false;
        }
        // check wall or rock or other robot
        if (board[nloc.row][nloc.col].Wall->isPresent() || board[nloc.row][nloc.col].Rock->isPresent())
            return false;
        if (board[nloc.row][nloc.col].Robot->isPresent())
        {
            // collision with other robot -> game end in higher level; here return false
            return false;
        }

        // move robot: clear old, set new
        board[loc.row][loc.col].internalRobotContent.setPresent(false);
        board[loc.row][loc.col].Robot = &board[loc.row][loc.col].internalRobotContent;

        board[nloc.row][nloc.col].internalRobotContent = *rc; // copy robot content (color/direction/etc)
        board[nloc.row][nloc.col].internalRobotContent.setPresent(true);
        board[nloc.row][nloc.col].Robot = &board[nloc.row][nloc.col].internalRobotContent;

        // set square left behind to robot's paint color
        board[loc.row][loc.col].internalSquareColor.setColor((robotColor == Color::Red) ? Color::Red : Color::Blue);

        // update stored robot location
        if (robotColor == Red)
            redLoc = nloc;
        else
            blueLoc = nloc;
    }
    // firing is not handled in moveRobot here (fire flag exists in RobotMoveRequest if needed)
    turnNum++;
    return true;
}

bool GameBoard::paintBlobHit(Color shooterColor) const
{
    // simple check: shoot in direction and see if other robot is in straight line without wall/rock blocking
    Location sLoc = (shooterColor == Red ? redLoc : blueLoc);
    const RobotContent *shooter = dynamic_cast<const RobotContent *>(board[sLoc.row][sLoc.col].Robot);
    if (!shooter || !shooter->isPresent())
        return false;
    Direction d = shooter->getDirection();

    Location check = sLoc + Location(d.dRow, d.dCol);
    while (inBounds(check))
    {
        if (board[check.row][check.col].Rock->isPresent())
            return false; // blocked
        if (board[check.row][check.col].Robot->isPresent())
        {
            const RobotContent *rc = dynamic_cast<const RobotContent *>(board[check.row][check.col].Robot);
            if (rc && rc->isPresent())
            {
                // hit found
                return true;
            }
        }
        check = check + Location(d.dRow, d.dCol);
    }
    return false;
}

void GameBoard::setSquareColor(const Location &loc, Color color)
{
    if (!inBounds(loc))
        throw std::out_of_range("Location out of bounds");
    board[loc.row][loc.col].internalSquareColor.setColor(color);
}

const RobotContent &GameBoard::getRobotContent(Color robotColor) const
{
    if (robotColor == Red)
        return redRobot;
    return blueRobot;
}

int GameBoard::blueScore() const
{
    int cnt = 0;
    for (int r = 0; r < BOARD_SZ; ++r)
        for (int c = 0; c < BOARD_SZ; ++c)
            if (board[r][c].internalSquareColor.getColor() == Blue)
                ++cnt;
    return cnt;
}
int GameBoard::redScore() const
{
    int cnt = 0;
    for (int r = 0; r < BOARD_SZ; ++r)
        for (int c = 0; c < BOARD_SZ; ++c)
            if (board[r][c].internalSquareColor.getColor() == Red)
                ++cnt;
    return cnt;
}

int GameBoard::turnNumber() const { return turnNum; }

Location GameBoard::robotLocation(Color robotColor) const
{
    return (robotColor == Red) ? redLoc : blueLoc;
}

AgentSquare **GameBoard::getLongRangeScan(Color robotColor) const
{
    // return a BOARD_SZ x BOARD_SZ array of AgentSquare* (allocated)
    AgentSquare **arr = new AgentSquare *[BOARD_SZ];
    for (int r = 0; r < BOARD_SZ; ++r)
    {
        arr[r] = new AgentSquare[BOARD_SZ];
        for (int c = 0; c < BOARD_SZ; ++c)
        {
            arr[r][c] = AgentSquare(board[r][c]); // copy-limited info
        }
    }
    return arr;
}

AgentSquare **GameBoard::getShortRangeScan(Color robotColor) const
{
    // spec said 9 squares around location (3x3) oriented with robot's forward as up.
    Location loc = robotLocation(robotColor);
    const RobotContent *rc = dynamic_cast<const RobotContent *>(board[loc.row][loc.col].Robot);
    Direction dir = rc ? rc->getDirection() : Direction::North;

    // produce a 3x3 array (allocated as 3 pointers)
    AgentSquare **arr = new AgentSquare *[3];
    for (int r = 0; r < 3; ++r)
        arr[r] = new AgentSquare[3];

    // mapping: we compute coordinates in world space for each local cell (i,j) with center (1,1)
    // local (i,j) offsets: row offset = i-1, col offset = j-1 but rotated so that 'up' is dir
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            int loff = i - 1;
            int coff = j - 1;
            // rotate offset by dir
            int wordr = dir.dRow * loff - dir.dCol * coff;
            int wordc = dir.dCol * loff + dir.dRow * coff;
            Location wloc = loc + Location(wordr, wordc);
            if (inBounds(wloc))
                arr[i][j] = AgentSquare(board[wloc.row][wloc.col]);
            else
                arr[i][j] = AgentSquare(); // empty/obscured
        }
    }
    return arr;
}

void GameBoard::testPrintSquare(const Location &loc) const
{
    if (!inBounds(loc))
        throw std::out_of_range("Location out of bounds");
    const InternalSquare &s = board[loc.row][loc.col];
    char colChar = 'W';
    Color sc = s.internalSquareColor.getColor();
    if (sc == Red)
        colChar = 'R';
    else if (sc == Blue)
        colChar = 'B';

    char obs = '-';
    if (s.Fog->isPresent())
        obs = 'F';
    if (s.Rock->isPresent())
        obs = 'R';
    if (s.Fog->isPresent() && s.Rock->isPresent())
        obs = 'X';
    char rob = '-';
    if (s.Robot->isPresent())
    {
        const RobotContent *rc = dynamic_cast<const RobotContent *>(s.Robot);
        if (rc && rc->isPresent())
            rob = (rc->getColor() == Red) ? 'R' : 'B';
    }
    std::cout << colChar << obs << rob << '\n';
}

void GameBoard::testPrintBoard() const
{
    for (int r = 0; r < BOARD_SZ; ++r)
    {
        for (int c = 0; c < BOARD_SZ; ++c)
        {
            const InternalSquare &s = board[r][c];
            char colChar = 'W';
            Color sc = s.internalSquareColor.getColor();
            if (sc == Red)
                colChar = 'R';
            else if (sc == Blue)
                colChar = 'B';
            char obs = '-';
            if (s.Fog->isPresent())
                obs = 'F';
            if (s.Rock->isPresent())
                obs = 'R';
            if (s.Fog->isPresent() && s.Rock->isPresent())
                obs = 'X';
            char rob = '-';
            if (s.Robot->isPresent())
            {
                const RobotContent *rc = dynamic_cast<const RobotContent *>(s.Robot);
                if (rc && rc->isPresent())
                {
                    rob = (rc->getColor() == Red) ? 'R' : 'B';
                }
            }
            std::cout << colChar << obs << rob << " ";
        }
        std::cout << std::endl
                  << std::endl;
    }
}