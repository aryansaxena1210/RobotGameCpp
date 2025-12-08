#include "GameLogic.h"
#include "TestAgents.h"
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

// Singleton instance
static GameLogic *instance = nullptr;

// Private constructor
GameLogic::GameLogic()
    : gameBoard(GameBoard::getInstance("xyzzy")),
      currentTurn(0),
      redPaintBlobsRemaining(0),
      bluePaintBlobsRemaining(0),
      redLongRangeScansRemaining(0),
      blueLongRangeScansRemaining(0),
      redHitTimer(0),
      blueHitTimer(0),
      redRequestedLRS(false),
      blueRequestedLRS(false),
      redLRS(nullptr),
      blueLRS(nullptr)
{
}

// Destructor
GameLogic::~GameLogic()
{
    if (redLRS != nullptr)
    {
        delete[] redLRS;
        redLRS = nullptr;
    }
    if (blueLRS != nullptr)
    {
        delete[] blueLRS;
        blueLRS = nullptr;
    }
}

// Singleton getInstance
GameLogic &GameLogic::getInstance(const string &password)
{
    if (password != "xyzzy")
    {
        throw std::invalid_argument("Invalid password");
    }

    if (instance == nullptr)
    {
        instance = new GameLogic();
    }

    return *instance;
}

// Reset game with default config
void GameLogic::resetGame()
{
    Config defaultConfig;
    resetGame(defaultConfig);
}

// Reset game with specified config
void GameLogic::resetGame(const Config &c)
{
    config = c;
    gameBoard.resetBoard(config);

    currentTurn = 0;
    redPaintBlobsRemaining = config.getPaintBlobLimit();
    bluePaintBlobsRemaining = config.getPaintBlobLimit();
    redLongRangeScansRemaining = config.getLongRangeLimit();
    blueLongRangeScansRemaining = config.getLongRangeLimit();
    redHitTimer = 0;
    blueHitTimer = 0;
    redRequestedLRS = false;
    blueRequestedLRS = false;

    if (redLRS != nullptr)
    {
        delete[] redLRS;
        redLRS = nullptr;
    }
    if (blueLRS != nullptr)
    {
        delete[] blueLRS;
        blueLRS = nullptr;
    }
}

// Play a game between two agents
void GameLogic::playGame(const string &redname, const string &bluename)
{
    cout << "=== Starting Paintbots Game ===" << endl;
    cout << "Red Robot: " << redname << endl;
    cout << "Blue Robot: " << bluename << endl;
    cout << "================================" << endl
         << endl;

    // Get and clone agents from roster
    RobotAgentRoster roster;
    roster.add(new LazyAgent());
    roster.add(new RandomAgent());
    roster.add(new AntiRandom());
    roster.add(new TrackerAgent());
    // TODO: Add AntiRandom and Tracker when implemented

    RobotAgent *redAgentTemplate = roster[redname];
    RobotAgent *blueAgentTemplate = roster[bluename];

    if (redAgentTemplate == nullptr)
    {
        throw std::runtime_error("Red agent '" + redname + "' not found in roster");
    }
    if (blueAgentTemplate == nullptr)
    {
        throw std::runtime_error("Blue agent '" + bluename + "' not found in roster");
    }

    RobotAgent *redAgent = redAgentTemplate->clone();
    RobotAgent *blueAgent = blueAgentTemplate->clone();

    // Set robot colors
    redAgent->setRobotColor(Color::Red);
    blueAgent->setRobotColor(Color::Blue);

    // Print initial board
    cout << "Initial Board State:" << endl;
    printGameState();

    // Main game loop - 300 turns maximum
    for (currentTurn = 1; currentTurn <= 300; currentTurn++)
    {
        cout << "\n=== Turn " << currentTurn << " ===" << endl;

        // Step 1: Get move requests from both agents
        RobotMoveRequest redRequest;
        RobotMoveRequest blueRequest;

        GameBoard::SROW *redSRS = gameBoard.getShortRangeScan(Color::Red);
        GameBoard::SROW *blueSRS = gameBoard.getShortRangeScan(Color::Blue);

        redAgent->getAgentMove(redRequest, redSRS, redLRS);
        blueAgent->getAgentMove(blueRequest, blueSRS, blueLRS);

        // Clean up LRS if they were used this turn
        if (redLRS != nullptr)
        {
            delete[] redLRS;
            redLRS = nullptr;
        }
        if (blueLRS != nullptr)
        {
            delete[] blueLRS;
            blueLRS = nullptr;
        }

        delete[] redSRS;
        delete[] blueSRS;

        // Step 2: Execute moves
        bool redMoveSuccess = gameBoard.moveRobot(redRequest, Color::Red);
        bool blueMoveSuccess = gameBoard.moveRobot(blueRequest, Color::Blue);

        // Check for illegal moves (hit wall or rock)
        if (!redMoveSuccess && redRequest.move == MoveForward)
        {
            cout << "\n=== GAME OVER ===" << endl;
            cout << "Red robot hit a wall or rock! Blue wins!" << endl;
            printGameState();
            delete redAgent;
            delete blueAgent;
            return;
        }
        if (!blueMoveSuccess && blueRequest.move == MoveForward)
        {
            cout << "\n=== GAME OVER ===" << endl;
            cout << "Blue robot hit a wall or rock! Red wins!" << endl;
            printGameState();
            delete redAgent;
            delete blueAgent;
            return;
        }

        // Check for collision
        if (checkCollision())
        {
            cout << "\n=== GAME OVER ===" << endl;
            cout << "Robots collided! " << getWinner() << " wins!" << endl;
            printGameState();
            delete redAgent;
            delete blueAgent;
            return;
        }

        // Step 3: Handle firing
        if (redRequest.fire && redPaintBlobsRemaining > 0)
        {
            cout << "Red robot fires paint blob!" << endl;
            if (gameBoard.paintBlobHit(Color::Red, redRequest))
            {
                cout << "  -> HIT! Blue robot is now painted Red!" << endl;
                applyPaintEffect(Color::Blue, Color::Red);
            }
            else
            {
                cout << "  -> Miss!" << endl;
            }
            redPaintBlobsRemaining--;
        }

        if (blueRequest.fire && bluePaintBlobsRemaining > 0)
        {
            cout << "Blue robot fires paint blob!" << endl;
            if (gameBoard.paintBlobHit(Color::Blue, blueRequest))
            {
                cout << "  -> HIT! Red robot is now painted Blue!" << endl;
                applyPaintEffect(Color::Red, Color::Blue);
            }
            else
            {
                cout << "  -> Miss!" << endl;
            }
            bluePaintBlobsRemaining--;
        }

        // Decrement hit timers
        decrementHitTimers();

        // Step 4: Handle long range scan requests
        if (redRequest.longrangescan && redLongRangeScansRemaining > 0)
        {
            redLRS = gameBoard.getLongRangeScan(Color::Red);
            redLongRangeScansRemaining--;
            cout << "Red robot used long range scan (remaining: "
                 << redLongRangeScansRemaining << ")" << endl;
        }

        if (blueRequest.longrangescan && blueLongRangeScansRemaining > 0)
        {
            blueLRS = gameBoard.getLongRangeScan(Color::Blue);
            blueLongRangeScansRemaining--;
            cout << "Blue robot used long range scan (remaining: "
                 << blueLongRangeScansRemaining << ")" << endl;
        }

        // Step 5: Move fog
        moveFog();

        // Step 6: Print board
        printGameState();
    }

    // Game ended after 300 turns
    cout << "\n=== GAME OVER: 300 turns reached ===" << endl;
    cout << getWinner() << " wins!" << endl;
    printGameState();

    delete redAgent;
    delete blueAgent;
}

// Apply paint effect to a robot
void GameLogic::applyPaintEffect(Color targetRobot, Color paintColor)
{
    if (targetRobot == Color::Red)
    {
        redHitTimer = config.getHitDuration();
    }
    else if (targetRobot == Color::Blue)
    {
        blueHitTimer = config.getHitDuration();
    }

    // Find and update the robot's paint color on the board
    for (int i = 1; i <= BOARD_SZ; i++)
    {
        for (int j = 1; j <= BOARD_SZ; j++)
        {
            InternalSquare &sq = dynamic_cast<InternalSquare &>(
                gameBoard.getSquareContent(Location(i, j)));

            if (sq.hasRobot())
            {
                RobotContent *robot = sq.getRobotContent();
                if (robot->getColor() == targetRobot)
                {
                    robot->setRobotPaintColor(paintColor);
                    cout << "  Paint effect applied: "
                         << (targetRobot == Color::Red ? "Red" : "Blue")
                         << " robot now paints "
                         << (paintColor == Color::Red ? "Red" : "Blue")
                         << " for " << config.getHitDuration() << " turns" << endl;
                    return;
                }
            }
        }
    }
}

// Decrement hit timers and restore paint colors
void GameLogic::decrementHitTimers()
{
    if (redHitTimer > 0)
    {
        redHitTimer--;
        if (redHitTimer == 0)
        {
            // Restore red robot's original paint color
            for (int i = 1; i <= BOARD_SZ; i++)
            {
                for (int j = 1; j <= BOARD_SZ; j++)
                {
                    InternalSquare &sq = dynamic_cast<InternalSquare &>(
                        gameBoard.getSquareContent(Location(i, j)));

                    if (sq.hasRobot())
                    {
                        RobotContent *robot = sq.getRobotContent();
                        if (robot->getColor() == Color::Red)
                        {
                            robot->setRobotPaintColor(Color::Red);
                            cout << "  Paint effect expired: Red robot back to painting Red" << endl;
                            return;
                        }
                    }
                }
            }
        }
    }

    if (blueHitTimer > 0)
    {
        blueHitTimer--;
        if (blueHitTimer == 0)
        {
            // Restore blue robot's original paint color
            for (int i = 1; i <= BOARD_SZ; i++)
            {
                for (int j = 1; j <= BOARD_SZ; j++)
                {
                    InternalSquare &sq = dynamic_cast<InternalSquare &>(
                        gameBoard.getSquareContent(Location(i, j)));

                    if (sq.hasRobot())
                    {
                        RobotContent *robot = sq.getRobotContent();
                        if (robot->getColor() == Color::Blue)
                        {
                            robot->setRobotPaintColor(Color::Blue);
                            cout << "  Paint effect expired: Blue robot back to painting Blue" << endl;
                            return;
                        }
                    }
                }
            }
        }
    }
}

// Move fog randomly
void GameLogic::moveFog()
{
    // Collect all fog locations
    std::vector<Location> fogLocations;
    for (int i = 1; i <= BOARD_SZ; i++)
    {
        for (int j = 1; j <= BOARD_SZ; j++)
        {
            if (gameBoard.getSquareContent(Location(i, j)).Fog->isPresent())
            {
                fogLocations.push_back(Location(i, j));
            }
        }
    }

    // Move each fog square with 20% probability
    for (const Location &fogLoc : fogLocations)
    {
        if (std::rand() % 5 == 0) // 20% chance
        {
            // Try to move to a random adjacent square
            int direction = std::rand() % 4;
            Location newLoc = fogLoc;

            if (direction == 0)
                newLoc.row--; // North
            else if (direction == 1)
                newLoc.col++; // East
            else if (direction == 2)
                newLoc.row++; // South
            else
                newLoc.col--; // West

            // Check if new location is valid (in bounds, not wall, not rock)
            if (newLoc.row >= 1 && newLoc.row <= BOARD_SZ &&
                newLoc.col >= 1 && newLoc.col <= BOARD_SZ)
            {
                Square &targetSquare = gameBoard.getSquareContent(newLoc);
                if (!targetSquare.Wall->isPresent() && !targetSquare.Rock->isPresent())
                {
                    // Move fog
                    InternalSquare &oldSquare = dynamic_cast<InternalSquare &>(
                        gameBoard.getSquareContent(fogLoc));
                    InternalSquare &newSquare = dynamic_cast<InternalSquare &>(targetSquare);

                    oldSquare.setFog(false);
                    newSquare.setFog(true);
                }
            }
        }
    }
}

// Check if robots collided
bool GameLogic::checkCollision() const
{
    Location redLoc, blueLoc;
    bool foundRed = false, foundBlue = false;

    // Find robot locations
    for (int i = 1; i <= BOARD_SZ; i++)
    {
        for (int j = 1; j <= BOARD_SZ; j++)
        {
            const Square &sq = gameBoard.getSquareContent(Location(i, j));
            if (sq.Robot->isPresent())
            {
                const RobotContent *robot = dynamic_cast<const RobotContent *>(sq.Robot);
                if (robot && robot->getColor() == Color::Red)
                {
                    redLoc = Location(i, j);
                    foundRed = true;
                }
                else if (robot && robot->getColor() == Color::Blue)
                {
                    blueLoc = Location(i, j);
                    foundBlue = true;
                }
            }
        }
    }

    return foundRed && foundBlue && (redLoc == blueLoc);
}

// Print current game state
void GameLogic::printGameState() const
{
    gameBoard.testPrintBoard();
    cout << "Turn: " << currentTurn << " | ";
    cout << "Red Score: " << gameBoard.redScore() << " | ";
    cout << "Blue Score: " << gameBoard.blueScore() << endl;
    cout << "Red Paint Blobs: " << redPaintBlobsRemaining << " | ";
    cout << "Blue Paint Blobs: " << bluePaintBlobsRemaining << endl;

    // Show hit timers if active
    if (redHitTimer > 0)
    {
        cout << "Red robot is painting Blue color for " << redHitTimer << " more turns" << endl;
    }
    if (blueHitTimer > 0)
    {
        cout << "Blue robot is painting Red color for " << blueHitTimer << " more turns" << endl;
    }
}

// Get winner based on scores
string GameLogic::getWinner() const
{
    int redScore = gameBoard.redScore();
    int blueScore = gameBoard.blueScore();

    if (redScore > blueScore)
        return "Red";
    else if (blueScore > redScore)
        return "Blue";
    else
        return "Tie";
}