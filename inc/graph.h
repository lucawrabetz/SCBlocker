#pragma once 

#include <fstream>
#include <string>
#include <sstream>

#include "vertex.h"

/* Graph class defined to read a graph from a file and generate the set-covering problem structure 
   of a bipartite graph to solve an instance of SCB.

    - vectors of Vertex objects for S and U

    - vectors of doubles for costs on elements of S and U

    - vectors of ints for neighbors of each S vertex, and vector of starting index for each
    - vectors of ints for neighbors of each U vertex, and vector of starting index for each

    - functions: constructor(s), printGraph(), setDebugLevel()
*/

class SCBGraph {
private:
    int m_debug_level = 0;
public:
    // three values to set a logging level for how much output when printing 
    const int debug_high = 1;
    const int debug_low = 0;

    int m, n, l; // number of S_Vertices, number of U_Vertices, number of edges
    std::vector<Vertex> s_vertices = {}; // vector of vertex objects for s_vertices, size m
    std::vector<Vertex> u_vertices = {}; // vector of vertex objects for u_vertices, size n

    std::vector<double> s_costs = {}; // vector of blocking costs on s_vertices, size m
    std::vector<double> u_costs = {}; // vector of penalty costs on u_vertices, size n

    std::vector<int> s_neighbors = {}; // full contiguous vector of neighbors of each s_vertex
    std::vector<int> sn_indexes = {}; // vector of starting index for each s_vertex in s_neighbors, size m+1
    std::vector<int> u_neighbors = {}; // full contiguous vector of neighbors of each u_vertex
    std::vector<int> un_indexes = {}; // vector of starting index for each u_vertex in u_neighbors, size n+1

    SCBGraph(); // default constructor 
    SCBGraph(const std::string& filename); // constructor from filename 
    void printGraph(); // logging/debugging function to print graph
    void setDebugLevel(int new_level);
};

