#include "scbformulation.h"

SCBFormulation::SCBFormulation(const SCBGraph&& G, int the_r) {
    // ------ Initialize model and environment ------
    scbenv = new GRBEnv();
    scbmodel = new GRBModel(*scbenv);
    scbmodel->getEnv().set(GRB_IntParam_LazyConstraints, 1);
    scbmodel->set(GRB_IntAttr_ModelSense, 1);

    // ------ Variables and int parameters ------
    n = G.n;
    m = G.m;
    r = the_r;

    // ------ Decision variable - z_k for each s_k in S_vertices ------
    std::string varname;
    for (int k = 0; k < m; k++) {
        varname = "z_" + std::to_string(k);
        z.push_back(scbmodel->addVar(0, 1, G.s_costs[k], GRB_BINARY, varname));
    }

    // ----- No Constraints initially - just lazy cuts -----

}


void SCBFormulation::solve() {
    try {
        // ----- Initialize Set-Cover Separation Object -----
        sep = SCSeparation(&z, &&G, r);

        // ----- Set Callback on Model -----
        scbmodel.setCallback(&sep);

        // ----- Optimize Model -----
        scbmodel.optimize();  

    } catch(GRBException e) {
        std::cout << "Gurobi error number [SCBMODEL]: " << e.getErrorCode() << "\n";
        std::cout << e.getMessage() << "\n";

    } catch (...) {
        std::cout << "Non-gurobi error during optimization [SCBMODEL]" << "\n";
    }
}

void SCBFormulation::printSol() {
    
}

// std::vector<double> solution;
// solution.push_back(G->n);
// solution.push_back(G->m);
// solution.push_back(r);
// solution.push_back(cb.lazyCuts);
// solution.push_back(model.get(GRB_DoubleAttr_Runtime));
// solution.push_back(model.get(GRB_IntAttr_NumVars));
// solution.push_back(model.get(GRB_IntAttr_NumBinVars));
// solution.push_back(model.get(GRB_IntAttr_NumConstrs));
// solution.push_back(model.get(GRB_DoubleAttr_NodeCount));
// solution.push_back(model.get(GRB_DoubleAttr_ObjVal));
// solution.push_back(model.get(GRB_DoubleAttr_MIPGap));