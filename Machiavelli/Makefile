.PHONY: clean

CXX=g++
CXXFLAGS=-c -std=c++11 -g -Og
LDFLAGS=-lpthread

all: main

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

main: main.o Player.o Socket.o throw.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -rf main *.o
