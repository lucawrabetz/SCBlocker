#include "graph.h"

int main() {
    std::string datafile = "dat/example1.graph";
    SCBGraph G = SCBGraph(datafile);
    G.printGraph();
}
