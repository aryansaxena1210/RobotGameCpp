#pragma once
#include "GameBoard.h"
#include "RobotAgentRoster.h"
#include "RobotAgent.h"
#include "Config.h"
#include <string>
#include <stdexcept>

using std::string;

class GameLogic
{
private:
    // Singleton pattern
    GameLogic();
    GameLogic(const GameLogic&) = delete;
    GameLogic& operator=(const GameLogic&) = delete;
    
    // Game state
    GameBoard& gameBoard;
    Config config;
    int currentTurn;
    
    // Robot state tracking
    int redPaintBlobsRemaining;
    int bluePaintBlobsRemaining;
    int redLongRangeScansRemaining;
    int blueLongRangeScansRemaining;
    int redHitTimer;
    int blueHitTimer;
    
    // Flags for long range scan requests
    bool redRequestedLRS;
    bool blueRequestedLRS;
    GameBoard::ROW* redLRS;
    GameBoard::ROW* blueLRS;
    
    // Helper methods
    void applyPaintEffect(Color targetRobot, Color paintColor);
    void decrementHitTimers();
    void moveFog();
    bool checkCollision() const;
    void printGameState() const;
    string getWinner() const;
    
public:
    static GameLogic& getInstance(const string& password);
    ~GameLogic();
    
    void resetGame();
    void resetGame(const Config& c);
    void playGame(const string& redname, const string& bluename);
};