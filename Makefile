CXXFLAGS = -std=c++17 -Wall -WExtra -g

OBJS = BoardSquare.o InternalBoardSquare.o ExternalBoardSquare.o \
       Config.o

all: testconfig

testconfig: ConfigTest.cpp $(OBJS)
	g++ $(CXXFLAGS) -o testconfig ConfigTest.cpp $(OBJS)

BoardSquare.o: BoardSquare.cpp BoardSquare.h
	g++ $(CXXFLAGS) -c BoardSquare.cpp

InternalBoardSquare.o: InternalBoardSquare.cpp InternalBoardSquare.h BoardSquare.h
	g++ $(CXXFLAGS) -c InternalBoardSquare.cpp

ExternalBoardSquare.o: ExternalBoardSquare.cpp ExternalBoardSquare.h BoardSquare.h
	g++ $(CXXFLAGS) -c ExternalBoardSquare.cpp

Config.o: Config.cpp Config.h
	g++ $(CXXFLAGS) -c Config.cpp

clean:
	rm -f *.o testconfig
