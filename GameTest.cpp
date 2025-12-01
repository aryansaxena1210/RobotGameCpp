#include "GameBoard.h"
#include "Config.h"
#include <iostream>

int main(int argc, char **argv)
{
    try
    {
        Config cfg;
        if (argc > 1)
        {
            cfg = Config(std::string(argv[1]));
        }
        GameBoard &gb = GameBoard::getInstance("xyzzy");
        gb.resetBoard(cfg);
        std::cout << "Initial board:\n";
        gb.testPrintBoard();

        std::cout << "Red score: " << gb.redScore() << " Blue score: " << gb.blueScore() << "\n";
        std::cout << "Turn number: " << gb.turnNumber() << "\n";

        // test a move: attempt to move red forward
        RobotMoveRequest rmr{MoveForward, false};
        bool ok = gb.moveRobot(rmr, Color::Red);
        std::cout << "Move red forward returned: " << (ok ? "true" : "false") << "\n";
        gb.testPrintBoard();

        // test long range scan (remember to delete returned arrays to avoid leaks in extended tests)
        AgentSquare **lrs = gb.getLongRangeScan(Color::Red);
        // ... optional quick check:
        if (lrs)
        {
            std::cout << "Long range scan first square present color? ";
            // print color char
            // cleanup
            for (int i = 0; i < GameBoard::BOARD_SZ; ++i)
                delete[] lrs[i];
            delete[] lrs;
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}