#include <iostream>
#include <vector>

/* we define a vertex class to hold some possibly useful information in another format from the graph
*/

class Vertex {
public:
    int cost;
    int name;
    bool svertex;

    Vertex(int the_name, int the_cost, bool s_u);
    void printVertex();
};

class Neighborhood {
public:
    int home;
    std::vector<int> neighbors = {};

    Neighborhood(int index);
};


