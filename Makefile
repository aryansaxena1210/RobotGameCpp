CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

SRCS = Util.cpp Content.cpp Square.cpp GameBoard.cpp Config.cpp TestGameBoard.cpp
HDRS = Util.h Content.h Square.h GameBoard.h Config.h

OBJS = $(SRCS:.cpp=.o)

all: testgameboard

testgameboard: $(OBJS)
	$(CXX) $(CXXFLAGS) -o testgameboard $(OBJS)

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o testgameboard
