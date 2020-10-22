#include "../inc/graph.h"

SCBGraph::SCBGraph(){
    m = 0;
    n = 0; 
    l = 0;
}

SCBGraph::SCBGraph(const std::string& filename) {
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

        std::vector<Neighborhood> temporary_neigh_s = {}; // temporary vector of neighbor class objects for each s_vertex, size m
        std::vector<Neighborhood> temporary_neigh_u = {}; // temporary vector of neighbor class objects for each u_vertex, size n

        while (getline(myfile, line)) {
            if (line != "") {
                if (counter > 3) {
                    // lines 4... of the file are the edges
                    cline = line.c_str(); 
                    sscanf(cline, "%d %d", &u, &v);
                    temporary_neigh_s[u].neighbors.push_back(v);
                    temporary_neigh_u[v].neighbors.push_back(u);
                } 

                else if (counter == 3) {
                    costs.clear();
                    std::stringstream ss(line);
                    int i = 0;

                    while (ss >> currentcost_str) {
                        currentcost_int = std::stoi(currentcost_str, nullptr, 10);
                        costs.push_back(currentcost_int);

                        // add a temporary edge neighboorhood class for u
                        Neighborhood neighborhood = Neighborhood(i);
                        temporary_neigh_u.push_back(neighborhood);

                        // initialize a new u_vertex for all 0...n-1, add to vector
                        Vertex uvertex = Vertex(i, costs[i], false); 
                        u_vertices.push_back(uvertex);
                        u_costs.push_back(costs[i]);
                        i++;
                    }
                }

                else if (counter == 2) {
                    // line 2 of the file are the blocking costs of S_vertices
                    costs.clear();
                    std::stringstream ss(line);
                    int k = 0;

                    while (ss >> currentcost_str) {
                        currentcost_int = std::stoi(currentcost_str, nullptr, 10);
                        costs.push_back(currentcost_int);

                        // add a temporary edge neighboorhood class for s
                        Neighborhood neighborhood = Neighborhood(k);
                        temporary_neigh_s.push_back(neighborhood);

                        // initialize a new s_vertex for all 0...m-1, add to vector
                        Vertex svertex = Vertex(k, costs[k], true); 
                        s_vertices.push_back(svertex);
                        s_costs.push_back(costs[k]);
                        k++;
                    }   
                }

                else {
                    // line 1 of the file is m and n and l 
                    cline = line.c_str(); 
                    sscanf(cline, "%d %d %d %d", &m, &n, &l, &r);
                }
            counter++;
            }
        }

        // use the temporary edges neighbor classes to build the contiguous neighbor vectors
        int tot_neighbors;
        counter = 0;
        for (int k = 0; k < m; k++) {
            // s_vertices
            sn_indexes.push_back(counter);
            tot_neighbors = temporary_neigh_s[k].neighbors.size(); // number of neighbors of vertex s_k (for indexing vector, next iteration)

            for (int j = 0; j < tot_neighbors; j++) {
                s_neighbors.push_back(temporary_neigh_s[k].neighbors[j]);
            }
            counter = counter + tot_neighbors; // starting index for next s_vertex
        }
        // add m+1th value to sn_indexes so we know the number of neighbors for mth s_vertex
        sn_indexes.push_back(counter);

        counter = 0;
        for (int i = 0; i < n; i++) {
            // u_vertices
            un_indexes.push_back(counter);
            tot_neighbors = temporary_neigh_u[i].neighbors.size(); // number of neighbors of vertex i (for indexing vector, next iteration)

            for (int j = 0; j < tot_neighbors; j++) {
                u_neighbors.push_back(temporary_neigh_u[i].neighbors[j]);
            }
            counter = counter + tot_neighbors; // starting index for next u_vertex
        }
        // add n+1th value to un_indexes so we know the number of neighbors for nth u_vertex
        un_indexes.push_back(counter);
    }
}

void SCBGraph::printGraph() {
    std::cout<<"SCB Bipartite Graph: \n\n\n";
    int tot_neighbors;

    std::cout<< "S_Vertices: \n";
    for (int k = 0; k < m; k++) {
        s_vertices[k].printVertex();
        tot_neighbors = sn_indexes[k+1] - sn_indexes[k];

        if (m_debug_level == debug_high) {
            std::cout << "s_costs: " << s_costs[k] << "\n";
            std::cout << "sn_index: " << sn_indexes[k] << "\n";
        }
        std::cout << tot_neighbors << " neighbors: " << "\n";

        for (int j = sn_indexes[k]; j < sn_indexes[k] + tot_neighbors; j++) {
            std::cout << "    - U_Vertex " << s_neighbors[j] << "\n";
        }
    }
    std::cout<< "\n\n\nU_Vertices: \n";
    for (int i = 0; i < n; i++) {
        u_vertices[i].printVertex();
        tot_neighbors = un_indexes[i+1] - un_indexes[i];
        
        if (m_debug_level == debug_high) {
            std::cout << "u_costs: " << u_costs[i] << "\n";
            std::cout << "un_index: " << un_indexes[i] << "\n";
        }
        std::cout << tot_neighbors << " neighbors: " << "\n";

        for (int j = un_indexes[i]; j < un_indexes[i] + tot_neighbors; j++) {
            std::cout << "    - S_Vertex " << u_neighbors[j] << "\n";
        }
    }
}

void SCBGraph::setDebugLevel(int new_level) {
    try {
        if (new_level != debug_high and 
            new_level != debug_low) {
            throw new_level;
        }
        else {
            m_debug_level = new_level;
        }
    }
    catch (int e) {
        std::cout << "New_level must be 0 or 1\n";
        std::cout << e << " inputted\n";
    }
}