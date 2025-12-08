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

# temp : Util.o temp.o
# 	g++ -o temp Util.o temp.o

# Util.o : Util.h Util.cpp
# 	g++ -c Util.cpp

# temp.o : temp.cpp
# 	g++ -c temp.cpp

# Phony targets
.PHONY: all clean