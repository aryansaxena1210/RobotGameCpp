CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

OBJS = BoardSquare.o InternalBoardSquare.o ExternalBoardSquare.o \
       Config.o

all: testconfig

testconfig: ConfigTest.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o testconfig ConfigTest.cpp $(OBJS)

BoardSquare.o: BoardSquare.cpp BoardSquare.h SquareInfo.h RobotInfo.h PBEnums.h
	$(CXX) $(CXXFLAGS) -c BoardSquare.cpp

InternalBoardSquare.o: InternalBoardSquare.cpp InternalBoardSquare.h BoardSquare.h \
                       SquareInfo.h RobotInfo.h PBEnums.h
	$(CXX) $(CXXFLAGS) -c InternalBoardSquare.cpp

ExternalBoardSquare.o: ExternalBoardSquare.cpp ExternalBoardSquare.h BoardSquare.h \
                       SquareInfo.h RobotInfo.h PBEnums.h InternalBoardSquare.h
	$(CXX) $(CXXFLAGS) -c ExternalBoardSquare.cpp

Config.o: Config.cpp Config.h
	$(CXX) $(CXXFLAGS) -c Config.cpp

clean:
	rm -f *.o testconfig
