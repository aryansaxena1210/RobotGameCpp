#include "GameBoard.h"
#include "Config.h"
#include <iostream>
#include <string>
#include <string.h>

void runMove(GameBoard &gameBoard, Color color, RobotMove move, bool fire, const std::string &label);
void runRed(GameBoard &gb, RobotMove m, bool f, const std::string &label);
void runBlue(GameBoard &gb, RobotMove m, bool f, const std::string &label);

// Manual Moves for robots test
int main(int argc, char *argv[])
{

    Config config;

    // Check if config file was provided as command line argument
    if (argc >= 2)
    {
        std::string configFile = argv[1];
        std::cout << "Loading configuration from: " << configFile << std::endl;
        config = Config(configFile);
    }
    else
    {
        std::cout << "No configuration file provided, using default configuration." << std::endl;
    }

    // Get GameBoard instance with password
    GameBoard &gameBoard = GameBoard::getInstance("xyzzy");

    // Reset board with the loaded (or default) configuration
    gameBoard.resetBoard(config);

    std::cout << "\nGameBoard initialized successfully!" << std::endl;
    std::cout << "Board size: " << BOARD_SZ << "x" << BOARD_SZ << std::endl;
    std::cout << "\nConfiguration settings:" << std::endl;
    std::cout << "  Hit Duration: " << config.getHitDuration() << std::endl;
    std::cout << "  Paint Blob Limit: " << config.getPaintBlobLimit() << std::endl;
    std::cout << "  Rock Bounds: " << config.getRockLowerBound() << " - " << config.getRockUpperBound() << std::endl;
    std::cout << "  Fog Bounds: " << config.getFogLowerBound() << " - " << config.getFogUpperBound() << std::endl;
    std::cout << "  Long Range Limit: " << config.getLongRangeLimit() << std::endl;

    std::cout << "\nInitial Board State:" << std::endl;
    std::cout << "Turn: " << gameBoard.turnNumber() << std::endl;
    std::cout << "Red Score: " << gameBoard.redScore() << std::endl;
    std::cout << "Blue Score: " << gameBoard.blueScore() << std::endl;
    std::cout << "\nLegend: [Color][Obstruction][Robot][Direction]" << std::endl;
    std::cout << "  Color: W=White, R=Red, B=Blue" << std::endl;
    std::cout << "  Obstruction: -=None, R=Rock, F=Fog, X=Rock+Fog" << std::endl;
    std::cout << "  Robot: -=None, R=Red Robot, B=Blue Robot" << std::endl;
    std::cout << "  Direction: n=North, e=East, s=South, w=West, -=None" << std::endl;
    std::cout << "\nBoard:" << std::endl;
    gameBoard.testPrintBoard();

    std::cout << "\n=== Requested Test Sequence ===" << std::endl;

    // 1. blue turn right
    runBlue(gameBoard, TurnRight, false, "Move 1: Blue turns right");

    // 2. blue forward
    runBlue(gameBoard, MoveForward, false, "Move 2: Blue moves forward");

    // 3. blue turn right
    runBlue(gameBoard, TurnRight, false, "Move 3: Blue turns right");

    // 4. blue shoot
    runBlue(gameBoard, NoMove, true, "Move 4: Blue fires paint");

    // 5. red forward
    runRed(gameBoard, MoveForward, false, "Move 5: Red moves forward");

    // 6. red forward
    runRed(gameBoard, MoveForward, false, "Move 6: Red moves forward");

    std::cout << "\n=== End of Sequence ===" << std::endl;
}

void runMove(GameBoard &gameBoard, Color color, RobotMove move, bool fire, const std::string &label)
{
    RobotMoveRequest req;
    req.move = move;
    req.fire = fire;

    std::cout << "\n"
              << label << std::endl;

    bool success = gameBoard.moveRobot(req, color);
    std::cout << "Move successful: " << (success ? "YES" : "NO") << std::endl;

    gameBoard.testPrintBoard();
}

// Convenience wrappers
inline void runRed(GameBoard &gb, RobotMove m, bool f, const std::string &label)
{
    runMove(gb, Color::Red, m, f, label);
}

inline void runBlue(GameBoard &gb, RobotMove m, bool f, const std::string &label)
{
    runMove(gb, Color::Blue, m, f, label);
}

// Robot move randomly test:
// int main(int argc, char *argv[])
// {
//     try
//     {
//         Config config;

//         // Check if config file was provided as command line argument
//         if (argc >= 2)
//         {
//             std::string configFile = argv[1];
//             std::cout << "Loading configuration from: " << configFile << std::endl;
//             config = Config(configFile);
//         }
//         else
//         {
//             std::cout << "No configuration file provided, using default configuration." << std::endl;
//         }

//         // Get GameBoard instance with password
//         GameBoard &gameBoard = GameBoard::getInstance("xyzzy");

//         // Reset board with the loaded (or default) configuration
//         gameBoard.resetBoard(config);

//         std::cout << "\nGameBoard initialized successfully!" << std::endl;
//         std::cout << "Board size: " << BOARD_SZ << "x" << BOARD_SZ << std::endl;
//         std::cout << "\nConfiguration settings:" << std::endl;
//         std::cout << "  Hit Duration: " << config.getHitDuration() << std::endl;
//         std::cout << "  Paint Blob Limit: " << config.getPaintBlobLimit() << std::endl;
//         std::cout << "  Rock Bounds: " << config.getRockLowerBound() << " - " << config.getRockUpperBound() << std::endl;
//         std::cout << "  Fog Bounds: " << config.getFogLowerBound() << " - " << config.getFogUpperBound() << std::endl;
//         std::cout << "  Long Range Limit: " << config.getLongRangeLimit() << std::endl;

//         std::cout << "\nInitial Board State:" << std::endl;
//         std::cout << "Turn: " << gameBoard.turnNumber() << std::endl;
//         std::cout << "Red Score: " << gameBoard.redScore() << std::endl;
//         std::cout << "Blue Score: " << gameBoard.blueScore() << std::endl;
//         std::cout << "\nLegend: [Color][Obstruction][Robot][Direction]" << std::endl;
//         std::cout << "  Color: W=White, R=Red, B=Blue" << std::endl;
//         std::cout << "  Obstruction: -=None, R=Rock, F=Fog, X=Rock+Fog" << std::endl;
//         std::cout << "  Robot: -=None, R=Red Robot, B=Blue Robot" << std::endl;
//         std::cout << "  Direction: n=North, e=East, s=South, w=West, -=None" << std::endl;
//         std::cout << "\nBoard:" << std::endl;
//         gameBoard.testPrintBoard();

//         // Print robot information
//         const RobotContent &redRobot = gameBoard.getRobotContent(Color::Red);
//         const RobotContent &blueRobot = gameBoard.getRobotContent(Color::Blue);

//         std::cout << "\nRobot Information:" << std::endl;
//         std::cout << "  Red Robot - Color: Red, Direction: ("
//                   << redRobot.getDirection().dRow << ", "
//                   << redRobot.getDirection().dCol << "), Paint Color: ";
//         if (redRobot.getPaintColor() == Color::Red)
//             std::cout << "Red";
//         else if (redRobot.getPaintColor() == Color::Blue)
//             std::cout << "Blue";
//         else
//             std::cout << "None";
//         std::cout << std::endl;

//         std::cout << "  Blue Robot - Color: Blue, Direction: ("
//                   << blueRobot.getDirection().dRow << ", "
//                   << blueRobot.getDirection().dCol << "), Paint Color: ";
//         if (blueRobot.getPaintColor() == Color::Red)
//             std::cout << "Red";
//         else if (blueRobot.getPaintColor() == Color::Blue)
//             std::cout << "Blue";
//         else
//             std::cout << "None";
//         std::cout << std::endl;

//         std::cout << "=====================================================================" << std::endl;

//         std::cout << "\n=== Testing Robot Movements (10 turns) ===" << std::endl;

//         // Run 10 turns
//         for (int turn = 1; turn <= 10; turn++)
//         {
//             std::cout << "\n--- Turn " << turn << " ---" << std::endl;

//             // Red robot move
//             RobotMoveRequest redMove;
//             redMove.move = (turn % 3 == 0) ? TurnRight : MoveForward;
//             redMove.fire = false;
//             redMove.longrangescan = false;

//             bool redSuccess = gameBoard.moveRobot(redMove, Color::Red);
//             std::cout << "Red: " << (redMove.move == MoveForward ? "MoveForward" : "TurnRight")
//                       << " - " << (redSuccess ? "SUCCESS" : "FAILED") << std::endl;

//             // Blue robot move
//             RobotMoveRequest blueMove;
//             blueMove.move = (turn % 4 == 0) ? TurnLeft : MoveForward;
//             blueMove.fire = false;
//             blueMove.longrangescan = false;

//             bool blueSuccess = gameBoard.moveRobot(blueMove, Color::Blue);
//             std::cout << "Blue: " << (blueMove.move == MoveForward ? "MoveForward" : "TurnLeft")
//                       << " - " << (blueSuccess ? "SUCCESS" : "FAILED") << std::endl;

//             // Print current state
//             std::cout << "Red Score: " << gameBoard.redScore()
//                       << " | Blue Score: " << gameBoard.blueScore() << std::endl;

//             gameBoard.testPrintBoard();
//         }

//         std::cout << "\n=====================================================================" << std::endl;
//         std::cout << "\nFinal Scores:" << std::endl;
//         std::cout << "Red Score: " << gameBoard.redScore() << std::endl;
//         std::cout << "Blue Score: " << gameBoard.blueScore() << std::endl;
//         std::cout << "Total Turns: " << gameBoard.turnNumber() << std::endl;

//         std::cout << "\nGameBoard test completed successfully!" << std::endl;

//         return 0;
//     }
//     catch (const std::runtime_error &e)
//     {
//         std::cerr << "Runtime Error: " << e.what() << std::endl;
//         return 1;
//     }
//     catch (const std::invalid_argument &e)
//     {
//         std::cerr << "Invalid Argument: " << e.what() << std::endl;
//         return 1;
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error: " << e.what() << std::endl;
//         return 1;
//     }
//     catch (...)
//     {
//         std::cerr << "Unknown error occurred" << std::endl;
//         return 1;
//     }
// }