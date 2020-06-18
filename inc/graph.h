#pragma once 

#include <fstream>
#include <string>
#include <sstream>

#include "vertex.h"

class SCBGraph {
public:
    int m, n, l; // number of S_Vertices, number of U_Vertices, number of edges
    std::vector<Vertex*> s_vertices = {};
    std::vector<Vertex*> u_vertices = {};

    std::vector<double> s_costs = {};
    std::vector<double> u_costs = {};

    SCBGraph(){
        m = 0;
        n = 0; 
        l = 0;
    }

    SCBGraph(std::string filename) {
        std::string line; 
        std::ifstream myfile (filename);

        if (myfile.is_open()) {
            int counter = 1; 
            const char* cline;
            std::vector<int> costs;
            int currentcost_int;
            std::string currentcost_str;
            int u; 
            int v;

            while (getline(myfile, line)) {
                if (counter > 3) {
                    // lines 4... of the file are the edges
                    cline = line.c_str(); 
                    sscanf(cline, "%d %d", &u, &v);

                    s_vertices[u]->neighbors.push_back(u_vertices[v]);
                    u_vertices[v]->neighbors.push_back(s_vertices[u]);
                } 

                else if (counter == 3) {
                    costs.clear();
                    std::stringstream ss(line);

                    while (ss >> currentcost_str) {
                        currentcost_int = std::stoi(currentcost_str, nullptr, 10);
                        costs.push_back(currentcost_int);
                    }

                    for (int i = 0; i < n; i++) {
                        // initialize a new u_vertex for all 0...n-1, add to vector
                        Vertex* uvertex = new Vertex(i, costs[i], false); 
                        u_vertices.push_back(uvertex);
                        u_costs.push_back(costs[i]);
                    }
                }

                else if (counter == 2) {
                    // line 2 of the file are the blocking costs of S_vertices
                    costs.clear();
                    std::stringstream ss(line);

                    while (ss >> currentcost_str) {
                        currentcost_int = std::stoi(currentcost_str, nullptr, 10);
                        costs.push_back(currentcost_int);
                    }

                    for (int k = 0; k < m; k++) {
                        // initialize a new s_vertex for all 0...m-1, add to vector
                        Vertex* svertex = new Vertex(k, costs[k], true); 
                        s_vertices.push_back(svertex);
                        s_costs.push_back(costs[k]);
                    }    
                }

                else {
                    // line 1 of the file is m and n and l 
                    cline = line.c_str(); 
                    sscanf(cline, "%d %d %d", &m, &n, &l);
                }
                counter++;
            }
        }
    }

    void printGraph() {
        std::cout << "S Vertices:\n";
        for (int k = 0; k < m; k++) {
            s_vertices[k]->printVertex();
        }

        std::cout << "U Vertices:\n";
        for (int i = 0; i < n; i++) {
            u_vertices[i]->printVertex();
        }
    }

};

