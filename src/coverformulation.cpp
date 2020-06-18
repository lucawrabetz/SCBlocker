#include "coverformulation.h"
#include "gurobi_c++.h"

// ------ Return Vector of Doubles - Current Best Solution ------

std::vector<double> SCBFormulation(const SCBGraph &G, const std::vector<double> &z_bar) {

    // ------ Initialize Model and Environment ------
    
    GRBEnv *env = new GRBEnv();
    GRBModel scmodel = GRBModel(*env);

    GRBVar *x = new GRBVar[G.n];
    GRBVar *y = new GRBVar[G.n];

    // ------ Initialize Variables ------

    std::string varName; 

    for (int k = 0; k < G.m; k++) {
        // for each S_vertex k, initialize z_k, binary variable with lb 0, ub 1, and cost 1.0
        varName = "x_" + std::to_string(k);
        x[k] = scmodel.addVar(0.0, 1.0, 1.0, GRB_BINARY, varName);
    }

    for (int i = 0; i < G.n; i++) {
        // for each U_vertex i, initialize y_i, binary variable with lb 0, ub 1, and cost p_i
        varName = "y_" + std::to_string(i);
        y[i] = scmodel.addVar(0.0, 1.0, G.u_vertices[i]->cost, GRB_BINARY, varName);
    }

    // ------ Initialize Constraints ------
    
    // ------ Set-Covering Constraints ------
    std::vector<Vertex*> curr_neighbors; // vector to store neighbors of current element/u_vertex i
    std::vector<const int> cn_sizes; // vector to store size of neighborhood
    GRBLinExpr constr; // memory allocation for LINEXPR constraint object 

    for (int i = 0; i < G.n; i++) {
        constr = 0;
        curr_neighbors = G.u_vertices[i]->neighbors;
        cn_sizes.push_back(curr_neighbors.size());

        // build set_covering constraint for element/u_vertex i
        for (int k = 0; k < cn_sizes[i]; k++) {
            constr += x[curr_neighbors[k]->name]; // add x_k to constraint LHS
        }

        scmodel.addConstr(constr, GRB_GREATER_EQUAL, 1.0); // add set-covering constraint

    }

    // ------ Z-Bar Blocking Constraints ------
    for (int k = 0; k < G.m; k++) {
        // add a blocking constraint for every subset/s_vertex s_k 
        scmodel.addConstr(x[k], GRB_LESS_EQUAL, (1.0 - z_bar[k]));
    }
    
    // ------ Optimize ------

    scmodel.optimize();

    // ------ Output ------
    
    std::vector<double> solution;
    
    for (int k = 0; k < G.m; k++) {
        solution.push_back(x[k].get(GRB_DoubleAttr_X));
    }

    return solution;
    
}