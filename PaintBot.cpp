
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "GameLogic.h"
#include "Config.h"

using namespace std;

int main(int argc, char *argv[])
{
    // Seed random number generator
    std::srand(std::time(nullptr));

    // Check command line arguments
    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << " <red_robot_name> <blue_robot_name> [config_file]" << endl;
        cout << "Example: " << argv[0] << " LazyAgent RandomAgent" << endl;
        cout << "Example: " << argv[0] << " LazyAgent RandomAgent myconfig.txt" << endl;
        return 1;
    }

    string redRobotName = argv[1];
    string blueRobotName = argv[2];

    try
    {
        // Get GameLogic instance
        GameLogic &logic = GameLogic::getInstance("xyzzy");

        // Load configuration
        if (argc >= 4)
        {
            // Config file provided
            string configFile = argv[3];
            cout << "Loading configuration from: " << configFile << endl;
            Config config(configFile);
            logic.resetGame(config);
        }
        else
        {
            // Use default configuration
            cout << "Using default configuration" << endl;
            logic.resetGame();
        }

        // Play the game
        logic.playGame(redRobotName, blueRobotName);

        return 0;
    }
    catch (const std::runtime_error &e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    catch (const std::invalid_argument &e)
    {
        cerr << "Invalid argument: " << e.what() << endl;
        return 1;
    }
    catch (const std::exception &e)
    {
        cerr << "Unexpected error: " << e.what() << endl;
        return 1;
    }
    catch (...)
    {
        cerr << "Unknown error occurred" << endl;
        return 1;
    }
}