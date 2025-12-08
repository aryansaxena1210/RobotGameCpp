
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Object files
OBJS = Util.o Content.o Square.o Config.o GameBoard.o

# Targets
all: testgameboard

# Main executable for Part B
testgameboard: TestGameBoard.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o testgameboard TestGameBoard.o $(OBJS)

# Config test from Part A (if you want to keep it)
testconfig: ConfigTest.o Config.o
	$(CXX) $(CXXFLAGS) -o testconfig ConfigTest.o Config.o

# Object file dependencies
TestGameBoard.o: TestGameBoard.cpp GameBoard.h Config.h Square.h Content.h Util.h
	$(CXX) $(CXXFLAGS) -c TestGameBoard.cpp

ConfigTest.o: ConfigTest.cpp Config.h
	$(CXX) $(CXXFLAGS) -c ConfigTest.cpp

GameBoard.o: GameBoard.cpp GameBoard.h Square.h Content.h Config.h Util.h
	$(CXX) $(CXXFLAGS) -c GameBoard.cpp

Square.o: Square.cpp Square.h Content.h Util.h
	$(CXX) $(CXXFLAGS) -c Square.cpp

Content.o: Content.cpp Content.h Util.h
	$(CXX) $(CXXFLAGS) -c Content.cpp

Config.o: Config.cpp Config.h
	$(CXX) $(CXXFLAGS) -c Config.cpp

Util.o: Util.cpp Util.h
	$(CXX) $(CXXFLAGS) -c Util.cpp

# Clean
clean:
	rm -f *.o testgameboard testconfig 

Util.o : Util.h Util.cpp
	g++ -c Util.cpp

temp: temp.cpp GameLogic.o RobotAgentRoster.o TestAgents.o Config.o Util.o Content.o Square.o GameBoard.o
	g++ -o temp temp.cpp GameLogic.o RobotAgentRoster.o TestAgents.o Config.o Util.o Content.o Square.o GameBoard.o

GameLogic.o: GameLogic.cpp GameLogic.h GameBoard.h RobotAgent.h
	g++ -c GameLogic.cpp
RobotAgentRoster.o: RobotAgentRoster.cpp RobotAgentRoster.h RobotAgent.h
	g++ -c RobotAgentRoster.cpp

TestAgents.o: TestAgents.cpp TestAgents.h RobotAgent.h
	g++ -c TestAgents.cpp


# Phony targets
.PHONY: all clean


# # Compiler and flags
# CXX = g++
# CXXFLAGS = -std=c++11 -Wall -Wextra -g

# # Object files
# OBJS = Util.o Content.o Square.o Config.o GameBoard.o

# # Targets
# all: testgameboard

# # Main executable for Part B
# testgameboard: TestGameBoard.o $(OBJS)
# 	$(CXX) $(CXXFLAGS) -o testgameboard TestGameBoard.o $(OBJS)

# # Config test from Part A (if you want to keep it)
# testconfig: ConfigTest.o Config.o
# 	$(CXX) $(CXXFLAGS) -o testconfig ConfigTest.o Config.o

# # Temp test executable for personal testing
# temp: temp.o GameLogic.o RobotAgentRoster.o TestAgents.o Config.o Util.o Content.o Square.o GameBoard.o
# 	$(CXX) $(CXXFLAGS) -o temp temp.o GameLogic.o RobotAgentRoster.o TestAgents.o Config.o Util.o Content.o Square.o GameBoard.o

# # Object file dependencies
# TestGameBoard.o: TestGameBoard.cpp GameBoard.h Config.h Square.h Content.h Util.h
# 	$(CXX) $(CXXFLAGS) -c TestGameBoard.cpp

# ConfigTest.o: ConfigTest.cpp Config.h
# 	$(CXX) $(CXXFLAGS) -c ConfigTest.cpp

# temp.o: temp.cpp GameLogic.h GameBoard.h RobotAgent.h Config.h
# 	$(CXX) $(CXXFLAGS) -c temp.cpp

# GameBoard.o: GameBoard.cpp GameBoard.h Square.h Content.h Config.h Util.h
# 	$(CXX) $(CXXFLAGS) -c GameBoard.cpp

# GameLogic.o: GameLogic.cpp GameLogic.h GameBoard.h RobotAgent.h
# 	$(CXX) $(CXXFLAGS) -c GameLogic.cpp

# RobotAgentRoster.o: RobotAgentRoster.cpp RobotAgentRoster.h RobotAgent.h
# 	$(CXX) $(CXXFLAGS) -c RobotAgentRoster.cpp

# TestAgents.o: TestAgents.cpp TestAgents.h RobotAgent.h
# 	$(CXX) $(CXXFLAGS) -c TestAgents.cpp

# Square.o: Square.cpp Square.h Content.h Util.h
# 	$(CXX) $(CXXFLAGS) -c Square.cpp

# Content.o: Content.cpp Content.h Util.h
# 	$(CXX) $(CXXFLAGS) -c Content.cpp

# Config.o: Config.cpp Config.h
# 	$(CXX) $(CXXFLAGS) -c Config.cpp

# Util.o: Util.cpp Util.h
# 	$(CXX) $(CXXFLAGS) -c Util.cpp

# # Clean
# clean:
# 	rm -f *.o testgameboard testconfig temp

# # Phony targets
# .PHONY: all clean