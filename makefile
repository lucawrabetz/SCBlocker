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

all:
	$(CXX) $(CXXFLAGS) $(SRCPATH)scbformulation.cpp $(SRCPATH)scseparation.cpp $(SRCPATH)coverformulation.cpp \
	$(SRCPATH)graph.cpp $(SRCPATH)vertex.cpp \
	-o $(BINPATH)main $(SRCPATH)main.cpp -I$(INCMAC) $(CPPLIBMAC)

clean:
	rm -rf $(BINPATH)*.o $(BINPATH)*.dSYM $(BINPATH)main

# main: main.o coverformulation.o graph.o vertex.o
# 	$(CXX) $(CXXFLAGS) -o $(BINPATH)main main.o scbformulation.o scseparation.o coverformulation.o graph.o vertex.o -I$(INCMAC) $(CPPLIBMAC)

# main.o: $(INCPATH)graph.h $(SRCPATH)main.cpp
# 	$(CXX) $(CXXFLAGS) -c $(SRCPATH)main.cpp

# scbformulation.o: $(INCPATH)scbformulation.h $(SRCPATH)scbformulation.cpp
# 	$(CXX) $(CXXFLAGS) -c $(SRCPATH)scbformulation.cpp

# scseparation.o: $(INCPATH)scseparation.h $(SRCPATH)scseparation.cpp
# 	$(CXX) $(CXXFLAGS) -c $(SRCPATH)scseparation.cpp

# coverformulation.o: $(INCPATH)coverformulation.h $(SRCPATH)coverformulation.cpp
# 	$(CXX) $(CXXFLAGS) -c $(SRCPATH)coverformulation.cpp

# graph.o: $(INCPATH)graph.h $(SRCPATH)graph.cpp
# 	$(CXX) $(CXXFLAGS) -c $(SRCPATH)graph.cpp

# vertex.o: $(INCPATH)vertex.h $(SRCPATH)vertex.cpp
# 	$(CXX) $(CXXFLAGS) -c $(SRCPATH)vertex.cpp
