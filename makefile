# Makefile for SCBlocker

# Paths/Environmental Variables

SRCPATH	  = ./src/
BINPATH	  = ./bin/
DATPATH	  = ./dat/
INCPATH   = ./inc/

GRBPATH   = /opt/gurobi900/linux64 # Gurobi Linux
MACPATH   = /Library/gurobi811/mac64 # Gurobi Mac

# *****************************************************
# Variables to control Makefile operation

CXX = clang++
CXXFLAGS = -Wall -g -std=c++11

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o 
	$(CXX) $(CXXFLAGS) -o $(BINPATH)main main.o 

main.o: $(INCPATH)graph.h $(INCPATH)svertex.h $(INCPATH)uvertex.h $(SRCPATH)main.cpp 
	$(CXX) $(CXXFLAGS) -c $(SRCPATH)main.cpp 