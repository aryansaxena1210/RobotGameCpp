#include "GameBoard.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// Singleton instance
static GameBoard *instance = nullptr;

// Constructor - initializes with default config
GameBoard::GameBoard()
    : redRobot(Color::Red, Direction::North, Color::Red),
      blueRobot(Color::Blue, Direction::North, Color::Blue),
      turnNum(0),
      password("xyzzy")
{
    // todo - uncomment this out
    //  std::srand(std::time(nullptr)); // Seed random number generator
    resetBoard();
}

// Destructor
GameBoard::~GameBoard()
{
    // Nothing special needed - no dynamic memory in this class
}

// Singleton getInstance with password protection
GameBoard &GameBoard::getInstance(const std::string &password)
{
    if (password != "xyzzy")
    {
        throw std::invalid_argument("Invalid password");
    }

    if (instance == nullptr)
    {
        instance = new GameBoard();
    }

    return *instance;
}

// Reset board with default configuration
void GameBoard::resetBoard()
{
    Config defaultConfig;
    resetBoard(defaultConfig);
}

// Reset board with specified configuration
void GameBoard::resetBoard(const Config &c)
{
    cfg = c;
    turnNum = 0;

    // Initialize all squares to white (entire array including walls)
    for (int i = 0; i < BOARD_SZ + 2; i++)
    {
        for (int j = 0; j < BOARD_SZ + 2; j++)
        {
            board[i][j].setSquareColor(Color::White);
            board[i][j].setRock(false);
            board[i][j].setFog(false);
            board[i][j].setWall(false);
            board[i][j].removeRobot();
        }
    }

    // Place walls around the border (at indices 0 and BOARD_SZ+1)
    for (int i = 0; i < BOARD_SZ + 2; i++)
    {
        board[0][i].setWall(true);            // Top wall
        board[BOARD_SZ + 1][i].setWall(true); // Bottom wall
        board[i][0].setWall(true);            // Left wall
        board[i][BOARD_SZ + 1].setWall(true); // Right wall
    }

    // Place rocks randomly (in playable area: indices 1 to BOARD_SZ)
    int numRocks = cfg.getRockLowerBound() +
                   (std::rand() % (cfg.getRockUpperBound() - cfg.getRockLowerBound() + 1));

    for (int i = 0; i < numRocks; i++)
    {
        int row, col;
        do
        {
            row = 1 + (std::rand() % BOARD_SZ); // Range: 1 to BOARD_SZ
            col = 1 + (std::rand() % BOARD_SZ); // Range: 1 to BOARD_SZ
        } while (board[row][col].Rock->isPresent());

        board[row][col].setRock(true);
    }

    // Place fog randomly (in playable area: indices 1 to BOARD_SZ)
    int numFog = cfg.getFogLowerBound() +
                 (std::rand() % (cfg.getFogUpperBound() - cfg.getFogLowerBound() + 1));

    for (int i = 0; i < numFog; i++)
    {
        int row, col;
        do
        {
            row = 1 + (std::rand() % BOARD_SZ); // Range: 1 to BOARD_SZ
            col = 1 + (std::rand() % BOARD_SZ); // Range: 1 to BOARD_SZ
        } while (board[row][col].Fog->isPresent());

        board[row][col].setFog(true);
    }

    // Place robots randomly in empty squares
    placeRobotsRandomly();
}

// Place robots in random empty squares
void GameBoard::placeRobotsRandomly()
{
    // Place red robot (in playable area: indices 1 to BOARD_SZ)
    int row, col;
    do
    {
        row = 1 + (std::rand() % BOARD_SZ); // Range: 1 to BOARD_SZ
        col = 1 + (std::rand() % BOARD_SZ); // Range: 1 to BOARD_SZ
    } while (board[row][col].Rock->isPresent() || board[row][col].hasRobot());

    redLoc = Location(row, col);
    Direction randomDir = Direction::North;
    int dirChoice = std::rand() % 4;
    if (dirChoice == 1)
        randomDir = Direction::East;
    else if (dirChoice == 2)
        randomDir = Direction::South;
    else if (dirChoice == 3)
        randomDir = Direction::West;

    redRobot = RobotContent(Color::Red, randomDir, Color::Red);
    board[row][col].setRobot(Color::Red, randomDir, Color::Red);

    // Place blue robot (in playable area: indices 1 to BOARD_SZ)
    do
    {
        row = 1 + (std::rand() % BOARD_SZ); // Range: 1 to BOARD_SZ
        col = 1 + (std::rand() % BOARD_SZ); // Range: 1 to BOARD_SZ
    } while (board[row][col].Rock->isPresent() || board[row][col].hasRobot());

    blueLoc = Location(row, col);
    dirChoice = std::rand() % 4;
    randomDir = Direction::North;
    if (dirChoice == 1)
        randomDir = Direction::East;
    else if (dirChoice == 2)
        randomDir = Direction::South;
    else if (dirChoice == 3)
        randomDir = Direction::West;

    blueRobot = RobotContent(Color::Blue, randomDir, Color::Blue);
    board[row][col].setRobot(Color::Blue, randomDir, Color::Blue);
}

// Check if location is within bounds (playable area: 1 to BOARD_SZ)
bool GameBoard::inBounds(const Location &loc) const
{
    return loc.row >= 1 && loc.row <= BOARD_SZ &&
           loc.col >= 1 && loc.col <= BOARD_SZ;
}

// Get robot location by color
Location GameBoard::robotLocation(Color robotColor) const
{
    if (robotColor == Color::Red)
        return redLoc;
    else
        return blueLoc;
}

// Get square content at location
Square &GameBoard::getSquareContent(const Location &loc)
{
    if (!inBounds(loc))
    {
        throw std::out_of_range("Location out of bounds");
    }
    return board[loc.row][loc.col];
}

// Move robot according to move request
bool GameBoard::moveRobot(RobotMoveRequest moveRequest, Color robotColor)
{
    Location currentLoc = robotLocation(robotColor);
    RobotContent *robot = board[currentLoc.row][currentLoc.col].getRobotContent();
    Direction currentDir = robot->getDirection();

    Location newLoc = currentLoc;
    Direction newDir = currentDir;

    // todo remove
    std::cout << "\n=================== \nMOVE robot: " << robotColor << " with request: (" << moveRequest.fire << ", " << moveRequest.longrangescan << ", " << moveRequest.move << ") \n=====================\n";

    // Process turn/move
    switch (moveRequest.move)
    {
    case TurnLeft:
        newDir = currentDir.RotateLeft90();
        robot->setRobotDir(newDir);
        break;

    case TurnRight:
        newDir = currentDir.RotateRight90();
        robot->setRobotDir(newDir);
        break;

    case MoveForward:
        newLoc.row += currentDir.dRow;
        newLoc.col += currentDir.dCol;

        // Check if move is legal
        if (!inBounds(newLoc))
            return false;

        if (board[newLoc.row][newLoc.col].Wall->isPresent())
            return false;

        if (board[newLoc.row][newLoc.col].Rock->isPresent())
            return false;

        if (board[newLoc.row][newLoc.col].hasRobot())
        {
            // todo  - check if you dont wanna return false, player may want to end the game by running into opponents robot
            return false;
        }
        // Move is legal - paint current square and move robot
        board[currentLoc.row][currentLoc.col].setSquareColor(robot->getPaintColor());
        board[newLoc.row][newLoc.col].setRobot(robotColor, currentDir, robot->getPaintColor());
        board[currentLoc.row][currentLoc.col].removeRobot();

        // Update robot location
        if (robotColor == Color::Red)
            redLoc = newLoc;
        else
            blueLoc = newLoc;

        break;

    case NoMove:
        // Do nothing
        break;
    }

    // Update robot references
    if (robotColor == Color::Red)
        redRobot = *board[redLoc.row][redLoc.col].getRobotContent();
    else
        blueRobot = *board[blueLoc.row][blueLoc.col].getRobotContent();

    turnNum++;
    return true;
}

// Check if paint blob would hit opponent
bool GameBoard::paintBlobHit(Color shooterColor) const
{
    Location shooterLoc = robotLocation(shooterColor);
    Color targetColor = (shooterColor == Color::Red) ? Color::Blue : Color::Red;
    Location targetLoc = robotLocation(targetColor);

    const RobotContent &shooter = (shooterColor == Color::Red) ? redRobot : blueRobot;
    Direction shootDir = shooter.getDirection();

    Location checkLoc = shooterLoc;

    // Travel in the direction the robot is facing
    while (true)
    {
        checkLoc.row += shootDir.dRow;
        checkLoc.col += shootDir.dCol;

        if (!inBounds(checkLoc))
            return false;

        // Paint blob stops at walls
        if (board[checkLoc.row][checkLoc.col].Wall->isPresent())
            return false;

        // Paint blob stops at rocks
        if (board[checkLoc.row][checkLoc.col].Rock->isPresent())
            return false;

        // Check if we hit the target robot
        if (checkLoc == targetLoc)
            return true;
    }

    return false;
}

bool GameBoard::paintBlobHit(Color robotColor, RobotMoveRequest moveRequest)
{
    // Only check if fire was requested
    if (!moveRequest.fire)
        return false;

    Location shooterLoc = robotLocation(robotColor);
    Color targetColor = (robotColor == Color::Red) ? Color::Blue : Color::Red;
    Location targetLoc = robotLocation(targetColor);

    const RobotContent &shooter = (robotColor == Color::Red) ? redRobot : blueRobot;
    Direction shootDir = shooter.getDirection();

    Location checkLoc = shooterLoc;

    // Travel in the direction the robot is facing
    while (true)
    {
        checkLoc.row += shootDir.dRow;
        checkLoc.col += shootDir.dCol;

        if (!inBounds(checkLoc))
            return false;

        // Paint blob stops at walls
        if (board[checkLoc.row][checkLoc.col].Wall->isPresent())
            return false;

        // Paint blob stops at rocks
        if (board[checkLoc.row][checkLoc.col].Rock->isPresent())
            return false;

        // Check if we hit the target robot
        if (checkLoc == targetLoc)
            return true;
    }

    return false;
}

// Set square color
void GameBoard::setSquareColor(const Location &loc, Color color)
{
    if (!inBounds(loc))
    {
        throw std::out_of_range("Location out of bounds");
    }
    board[loc.row][loc.col].setSquareColor(color);
}

// Get robot content by color
const RobotContent &GameBoard::getRobotContent(Color robotColor) const
{
    if (robotColor == Color::Red)
        return redRobot;
    else
        return blueRobot;
}

// Calculate blue score (only count playable area: indices 1 to BOARD_SZ)
int GameBoard::blueScore() const
{
    int score = 0;
    for (int i = 1; i <= BOARD_SZ; i++)
    {
        for (int j = 1; j <= BOARD_SZ; j++)
        {
            if (board[i][j].getSquareColor() == Color::Blue)
                score++;
        }
    }
    return score;
}

// Calculate red score (only count playable area: indices 1 to BOARD_SZ)
int GameBoard::redScore() const
{
    int score = 0;
    for (int i = 1; i <= BOARD_SZ; i++)
    {
        for (int j = 1; j <= BOARD_SZ; j++)
        {
            if (board[i][j].getSquareColor() == Color::Red)
                score++;
        }
    }
    return score;
}

// Get current turn number
int GameBoard::turnNumber() const
{
    return turnNum;
}

// Get long range scan (entire BOARD_SZ x BOARD_SZ playable board, north-up orientation)
GameBoard::ROW *GameBoard::getLongRangeScan(Color robotColor) const
{
    // Allocate array of ROW (each ROW is AgentSquare[BOARD_SZ + 2])
    ROW *scan = new ROW[BOARD_SZ + 2];

    // Copy entire board including walls to AgentSquares
    for (int i = 0; i < BOARD_SZ + 2; i++)
    {
        for (int j = 0; j < BOARD_SZ + 2; j++)
        {
            scan[i][j] = AgentSquare(board[i][j]);
        }
    }

    return scan;
}

// Get short range scan (3x3 around robot, robot-forward orientation)
GameBoard::SROW *GameBoard::getShortRangeScan(Color robotColor) const
{
    const int SCAN_SZ = 3;
    const int SCAN_OFFSET = SCAN_SZ / 2; // Robot at center

    Location robotLoc = robotLocation(robotColor);
    const RobotContent &robot = (robotColor == Color::Red) ? redRobot : blueRobot;
    Direction robotDir = robot.getDirection();

    // Allocate array of SROW (each SROW is AgentSquare[3])
    SROW *scan = new SROW[SCAN_SZ];

    // Check if robot is in fog
    if (board[robotLoc.row][robotLoc.col].Fog->isPresent())
    {
        // Return obscured squares except center
        for (int i = 0; i < SCAN_SZ; i++)
        {
            for (int j = 0; j < SCAN_SZ; j++)
            {
                if (i == SCAN_OFFSET && j == SCAN_OFFSET)
                {
                    // Center square shows fog
                    scan[i][j] = AgentSquare(Color::White, false, true, false, Color::None);
                }
                else
                {
                    // All other squares are obscured (empty white)
                    scan[i][j] = AgentSquare(Color::White, false, false, false, Color::None);
                }
            }
        }
        return scan;
    }

    // Map scan coordinates to board coordinates based on robot direction
    for (int scanRow = 0; scanRow < SCAN_SZ; scanRow++)
    {
        for (int scanCol = 0; scanCol < SCAN_SZ; scanCol++)
        {
            // Offset from robot (in scan coordinates, 0,0 is top-left, robot at 1,1)
            int offsetRow = scanRow - SCAN_OFFSET;
            int offsetCol = scanCol - SCAN_OFFSET;

            // Rotate offset based on robot direction to get board offset
            int boardOffsetRow, boardOffsetCol;
            if (robotDir.dRow == -1 && robotDir.dCol == 0) // North
            {
                boardOffsetRow = offsetRow;
                boardOffsetCol = offsetCol;
            }
            else if (robotDir.dRow == 0 && robotDir.dCol == 1) // East
            {
                boardOffsetRow = offsetCol;
                boardOffsetCol = -offsetRow;
            }
            else if (robotDir.dRow == 1 && robotDir.dCol == 0) // South
            {
                boardOffsetRow = -offsetRow;
                boardOffsetCol = -offsetCol;
            }
            else // West
            {
                boardOffsetRow = -offsetCol;
                boardOffsetCol = offsetRow;
            }

            int boardRow = robotLoc.row + boardOffsetRow;
            int boardCol = robotLoc.col + boardOffsetCol;

            // Check if location is in bounds (or if it's a wall)
            if (boardRow >= 0 && boardRow < BOARD_SZ + 2 &&
                boardCol >= 0 && boardCol < BOARD_SZ + 2)
            {
                scan[scanRow][scanCol] = AgentSquare(board[boardRow][boardCol]);
            }
            else
            {
                // Out of array bounds - return obscured square
                scan[scanRow][scanCol] = AgentSquare();
            }
        }
    }

    return scan;
}

// Print entire playable board (3-character format)
void GameBoard::testPrintBoard() const
{
    for (int i = 1; i <= BOARD_SZ; i++)
    {
        for (int j = 1; j <= BOARD_SZ; j++)
        {
            testPrintSquare(Location(i, j));
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

// Print single square (4-character format)
void GameBoard::testPrintSquare(const Location &loc) const
{
    // Check if within full array bounds
    if (loc.row < 0 || loc.row >= BOARD_SZ + 2 ||
        loc.col < 0 || loc.col >= BOARD_SZ + 2)
    {
        std::cout << "????";
        return;
    }

    // Character 1: Square color
    Color sqColor = board[loc.row][loc.col].getSquareColor();
    char colorChar;
    if (sqColor == Color::Red)
        colorChar = 'R';
    else if (sqColor == Color::Blue)
        colorChar = 'B';
    else
        colorChar = 'W';

    // Character 2: Obstructions
    bool hasRock = board[loc.row][loc.col].Rock->isPresent();
    bool hasFog = board[loc.row][loc.col].Fog->isPresent();
    char obstChar;
    if (hasRock && hasFog)
        obstChar = 'X';
    else if (hasRock)
        obstChar = 'R';
    else if (hasFog)
        obstChar = 'F';
    else
        obstChar = '-';

    // Character 3: Robot
    char robotChar = '-';
    if (board[loc.row][loc.col].hasRobot())
    {
        Color robotColor = board[loc.row][loc.col].getRobotContent()->getColor();
        if (robotColor == Color::Red)
            robotChar = 'R';
        else if (robotColor == Color::Blue)
            robotChar = 'B';
    }

    // Character 4: Robot Direction
    char dirChar = '-';
    if (board[loc.row][loc.col].hasRobot())
    {
        Direction robotDir = board[loc.row][loc.col].getRobotContent()->getDirection();
        if (robotDir.dRow == -1 && robotDir.dCol == 0)
            dirChar = 'n'; // North
        else if (robotDir.dRow == 0 && robotDir.dCol == 1)
            dirChar = 'e'; // East
        else if (robotDir.dRow == 1 && robotDir.dCol == 0)
            dirChar = 's'; // South
        else if (robotDir.dRow == 0 && robotDir.dCol == -1)
            dirChar = 'w'; // West
    }

    std::cout << colorChar << obstChar << robotChar << dirChar;
}