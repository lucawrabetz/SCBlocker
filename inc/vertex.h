#include <iostream>
#include <vector>
// we define to classes for a vertex from S and U in the bipartite graph
// an S_vertex will have a vector of U_vertex pointers that it covers
// a U_vertex will have a vector of S_vertex pointers that covers it 

class Vertex {
public:
    std::vector<Vertex*> neighbors = {};
    int cost;
    int name;
    bool svertex;

    Vertex(int the_name, int the_cost, bool s_u) {
        name = the_name;
        cost = the_cost;
        svertex = s_u;
    }

    void printVertex() {
        if (svertex) {
            std::cout << "S vertex " << name << " with cost " << cost << ", covering:\n";
            for (int i = 0; i < neighbors.size(); i++){
                std::cout << "      - " << neighbors[i]->name << "\n";
            }
        }

        else {
            std::cout << "U vertex " << name << " with cost " << cost << ", covered by:\n";
            for (int i = 0; i < neighbors.size(); i++){
                std::cout << "      - " << neighbors[i]->name << "\n";
            }
        }
    }
};

