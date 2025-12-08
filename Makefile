# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g

# Object files
OBJS = Config.o Util.o Content.o Square.o GameBoard.o RobotAgentRoster.o TestAgents.o GameLogic.o

# Default target - does NOT include temp
all: testgameboard

# TestGameBoard executable
testgameboard: TestGameBoard.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o testgameboard TestGameBoard.cpp $(OBJS)

# Temp testing executable (separate target)
temp: temp.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o temp temp.cpp $(OBJS)

# Object file rules
Config.o: Config.cpp Config.h
	$(CXX) $(CXXFLAGS) -c Config.cpp

Util.o: Util.cpp Util.h
	$(CXX) $(CXXFLAGS) -c Util.cpp

Content.o: Content.cpp Content.h Util.h
	$(CXX) $(CXXFLAGS) -c Content.cpp

Square.o: Square.cpp Square.h Content.h Util.h
	$(CXX) $(CXXFLAGS) -c Square.cpp

GameBoard.o: GameBoard.cpp GameBoard.h Square.h Content.h Util.h Config.h
	$(CXX) $(CXXFLAGS) -c GameBoard.cpp

RobotAgentRoster.o: RobotAgentRoster.cpp RobotAgentRoster.h RobotAgent.h
	$(CXX) $(CXXFLAGS) -c RobotAgentRoster.cpp

TestAgents.o: TestAgents.cpp TestAgents.h RobotAgent.h GameBoard.h
	$(CXX) $(CXXFLAGS) -c TestAgents.cpp

GameLogic.o: GameLogic.cpp GameLogic.h GameBoard.h RobotAgent.h RobotAgentRoster.h Config.h TestAgents.h
	$(CXX) $(CXXFLAGS) -c GameLogic.cpp

# Clean
clean:
	rm -f *.o testgameboard temp

.PHONY: all clean temp