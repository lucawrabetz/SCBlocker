#include "../inc/scbformulation.h"

SCBFormulation::SCBFormulation(SCBGraph* G, int the_r) {
    // ------ Initialize model and environment ------
    scbenv = new GRBEnv();
    scbmodel = new GRBModel(*scbenv);
    scbmodel->getEnv().set(GRB_IntParam_LazyConstraints, 1);
    scbmodel->set(GRB_IntAttr_ModelSense, 1);

    // ------ Variables and int parameters ------
    n = G->n;
    m = G->m;
    r = the_r;

    // ------ Decision variable - z_k for each s_k in S_vertices ------
    std::string varname;
    for (int k = 0; k < m; k++) {
        varname = "z_" + std::to_string(k);
        z.push_back(scbmodel->addVar(0, 1, G->s_costs[k], GRB_BINARY, varname));
    }

    // ----- No Constraints initially - just lazy cuts -----
    // ----- Initialize Set-Cover Separation Object -----
    sep = SCSeparation(z, G, r);

}

std::vector<double> SCBFormulation::solve() {
    std::vector<double> solution;
    try {
        // ----- Set Callback on Model -----
        scbmodel->setCallback(&sep);

        // ----- Optimize Model -----
        scbmodel->optimize(); 

        std::cout << "Model Status: " << scbmodel->get(GRB_IntAttr_Status) << "\n";
        if (scbmodel->get(GRB_IntAttr_Status) == GRB_OPTIMAL){
            solution.push_back(scbmodel->get(GRB_DoubleAttr_ObjVal));
            for (int k = 0; k < m; k++) {
                solution.push_back(z[k].get(GRB_DoubleAttr_X));
            }
            std::cout << "Optimal solution found\n";
        } 
        else if (scbmodel->get(GRB_IntAttr_Status) == GRB_INFEASIBLE) {
            std::cout << "Instance Infeasible\n";
        } 
        else {
            std::cout << scbmodel->get(GRB_IntAttr_Status) << "\n";
        }

        return solution;

    } catch(GRBException e) {
        std::cout << "Gurobi error number [SCBMODEL]: " << e.getErrorCode() << "\n";
        std::cout << e.getMessage() << "\n";

    } catch (...) {
        std::cout << "Non-gurobi error during optimization [SCBMODEL]" << "\n";
    }
    return solution;
}

void SCBFormulation::printSol() {
    
}

