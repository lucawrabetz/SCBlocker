# Makefile for SCBlocker

# Paths/Environmental Variables

SRCPATH	  = ./src/
BINPATH	  = ./bin/
DATPATH	  = ./dat/
INCPATH   = ./inc/

MACPATH   = /Library/gurobi902/mac64
INCMAC    = $(MACPATH)/include
CPPLIBMAC = -L$(MACPATH)/lib/ -lgurobi_c++ -lgurobi90 $(CPPSTDLIB) -lpthread -lm


# *****************************************************
# Variables to control Makefile operation

CXX = clang++
CXXFLAGS = -Wall -g -std=c++11

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o coverformulation.o graph.o vertex.o
	$(CXX) $(CXXFLAGS) -o $(BINPATH)main main.o coverformulation.o graph.o vertex.o -I$(INCMAC) $(CPPLIBMAC)

main.o: $(INCPATH)graph.h $(SRCPATH)main.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCPATH)main.cpp

coverformulation.o: $(INCPATH)coverformulation.h $(SRCPATH)coverformulation.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCPATH)coverformulation.cpp

graph.o: $(INCPATH)graph.h $(SRCPATH)graph.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCPATH)graph.cpp

vertex.o: $(INCPATH)vertex.h $(SRCPATH)vertex.cpp
	$(CXX) $(CXXFLAGS) -c $(SRCPATH)vertex.cpp
