#include "../inc/vertex.h"

Vertex::Vertex(int the_name, int the_cost, bool s_u) {
    name = the_name;
    cost = the_cost;
    svertex = s_u;
}

void Vertex::printVertex() {
    if (svertex) {
        std::cout << "S vertex " << name << " with blocking cost " << cost << "\n";
    }
    else {
        std::cout << "U vertex " << name << " with penalty cost " << cost << "\n";
    }
}

Neighborhood::Neighborhood(int index) {
    home = index; 
}

