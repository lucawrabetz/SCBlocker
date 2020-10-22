#include "../inc/scbformulation.h"

SCBFormulation::SCBFormulation(SCBGraph* G, const int the_max_gamma) {
    // ------ Initialize model and environment ------
    scbenv = new GRBEnv();
    scbmodel = new GRBModel(*scbenv);
    scbmodel->getEnv().set(GRB_IntParam_LazyConstraints, 1);
    scbmodel->set(GRB_IntAttr_ModelSense, 1);

    // ------ Variables and int parameters ------
    max_gamma = the_max_gamma;
    n = G->n;
    m = G->m;
    r = G->r;

    // ------ Decision variable - z_k for each s_k in S_vertices ------
    std::string varname;
    for (int k = 0; k < m; k++) {
        varname = "z_" + std::to_string(k);
        z.push_back(scbmodel->addVar(0, 1, G->s_costs[k], GRB_BINARY, varname));
    }

    // ----- No Constraints initially - just lazy cuts -----
    // ----- Initialize Set-Cover Separation Object -----
    sep = SCSeparation(z, G, max_gamma);
    scbmodel->set(GRB_DoubleParam_TimeLimit, 120);
}

std::vector<double> SCBFormulation::solve() {
    std::vector<double> solution;
    try {
        std::vector<double> x_0 = sep.cover.solve();
        std::cout << "Initial best set-cover: cardinality = " << x_0[0] << "\n";
        
        for (int k = 1; k < m + 1; k++){
            std::cout << x_0[k] << "\n";
        }

        // ----- Set Callback on Model -----
        scbmodel->setCallback(&sep);

        clock_t model_begin = clock();
        // ----- Optimize Model -----
        scbmodel->optimize(); 
        running_time = float(clock() - model_begin) / CLOCKS_PER_SEC;
        lazy_cuts = sep.lazyCuts;

        for (int i = 0; i < sep.separation_times.size(); i++) {
            avg_sep_time += sep.separation_times[i]; 
        }
        avg_sep_time = avg_sep_time/sep.separation_times.size();

        for (int i = 0; i < sep.cut_cardinality.size(); i++) {
            cut_density_avg += sep.cut_cardinality[i]; 
        } 
        cut_density_avg = ((cut_density_avg/sep.cut_cardinality.size()) / m) * 100;

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
        gap = scbmodel->get(GRB_DoubleParam_MIPGap);
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

