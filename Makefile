CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

SRCS = Util.cpp Content.cpp Square.cpp GameBoard.cpp Config.cpp TestGameBoard.cpp
HDRS = Util.h Content.h Square.h GameBoard.h Config.h

OBJS = $(SRCS:.cpp=.o)

all: testgameboard

Content.o : Content.cpp Content.h
	gcc -c Content.cpp -o Content.o
	
testgameboard: $(OBJS)
	$(CXX) $(CXXFLAGS) -o testgameboard $(OBJS)

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o testgameboard


temp: temp.o Content.o Util.o Square.o
	g++ -o temp temp.o Content.o Util.o Square.o
	./temp

temp.o : temp.cpp
	g++ -c temp.cpp

Content.o: Content.cpp Content.h Util.h
	g++ -c Content.cpp

Util.o: Util.cpp Util.h
	g++ -c Util.cpp

Square.o : Square.cpp Square.h
	g++ -c Square.cpp