#include "scbformulation.h"
#include "gurobi_c++.h"

// ------ Return vector of doubles - solution and other info ------

std::vector<double> SCBFormulation(SCBGraph &G, int r) {

    // ------ Initialize Model and Environment ------
    
    GRBEnv *env = new GRBEnv();
    GRBModel model = GRBModel(*env);

    GRBVar *z = new GRBVar[G.n];

    // ------ Initialize Variables ------

    std::string varName; 

    for (int k = 0; k < G.m; k++) {
        // for each S_vertex k, initialize z_k, binary variable with lb 0, ub 1, and cost c_k
        varName = "z_" + std::to_string(k);
        z[k] = model.addVar(0.0, 1.0, G.s_vertices[k]->cost, GRB_BINARY, varName);
    }

    // ------ Initialize Constraints ------


    // ------ Callback ------
    
    model.set(GRB_IntParam_LazyConstraints, 1); // Lazy Cuts



    // ------ Optimize ------


    // ------ Output ------
    
    std::vector<double> solution;
    solution.push_back(G.n);
    solution.push_back(G.m);
    solution.push_back(r);
    solution.push_back(cb.lazyCuts);
    solution.push_back(model.get(GRB_DoubleAttr_Runtime));
    solution.push_back(model.get(GRB_IntAttr_NumVars));
    solution.push_back(model.get(GRB_IntAttr_NumBinVars));
    solution.push_back(model.get(GRB_IntAttr_NumConstrs));
    solution.push_back(model.get(GRB_DoubleAttr_NodeCount));
    solution.push_back(model.get(GRB_DoubleAttr_ObjVal));
    solution.push_back(model.get(GRB_DoubleAttr_MIPGap));
}