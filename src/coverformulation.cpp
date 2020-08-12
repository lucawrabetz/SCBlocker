#include "../inc/coverformulation.h"

CoverFormulation::CoverFormulation() {
    n = 0;
    m = 0; 
}

CoverFormulation::CoverFormulation(SCBGraph* G) {
    // ------ Initialize model and environment ------
    covenv = new GRBEnv();
    covmodel = new GRBModel(*covenv);
    covmodel->set(GRB_IntAttr_ModelSense, 1);

    n = G->n;
    m = G->m;

    // ------ Decision variables ------
    std::string varname; 
    for (int k = 0; k < m; k++) {
        // for each S_vertex k, initialize z_k, binary variable with lb 0, ub 1, and cost 1.0
        varname = "x_" + std::to_string(k);
        x.push_back(covmodel->addVar(0.0, 1.0, 1.0, GRB_BINARY, varname));
    }

    for (int i = 0; i < n; i++) {
        // for each U_vertex i, initialize y_i, binary variable with lb 0, ub 1, and cost penalty_i
        varname = "y_" + std::to_string(i);
        y.push_back(covmodel->addVar(0.0, 1.0, G->u_costs[i], GRB_CONTINUOUS, varname));
    }

    // ------ Initialize Constraints ------
    // we SET the set-covering constraints, the blocking constraints are in the set_bounds() method
    GRBLinExpr constr; // LINEXPR constraint object 
    int start_index; // start delimeter for neighboorhood of current i in u_vertices 
    int end_index; // end delimeter for neighboorhood of current i in u_vertices

    for (int i = 0; i < n; i++) {
        constr = 0;
        start_index = G->un_indexes[i];
        end_index = G->un_indexes[i+1];

        // build set_covering constraint for element/u_vertex i
        for (int k = start_index; k < end_index; k++) {
            constr += x[G->u_neighbors[k]]; // add x_k to constraint LHS
        }

        constr += y[i]; // add y_i to the constraint LHS
        covmodel->addConstr(constr, GRB_GREATER_EQUAL, 1.0); // add set-covering constraint
    }

    covmodel->update();
}

void CoverFormulation::set_bounds(double* z_bar_now) {
    for (int k = 0; k < m; k++) {
        // for each s_k in s_vertices, check the interdiction binary in z_bar
        // if it is 1, we set the upper bound on x_k to 0
        // if it is 0, we set the upper bound on x_k to 1
        if (z_bar_now[k] > 0.5) {
            x[k].set(GRB_DoubleAttr_UB, 0);
        }
        else {
            x[k].set(GRB_DoubleAttr_UB, 1);
        }
    }
    covmodel->update();
    counter ++;
}

std::vector<double> CoverFormulation::solve() {
    std::vector<double> solution;
    try {
        // ------ Optimize ------
        covmodel->optimize();

        // ------ Output ------
        // solution[0] will be the objective value
        // solution[1...m+1] will be the variables
        solution.push_back(covmodel->get(GRB_DoubleAttr_ObjVal));

        for (int k = 0; k < m; k++) {
            solution.push_back(x[k].get(GRB_DoubleAttr_X));

        }
        
        return solution;
    }
    catch (GRBException e) {
        std::cout << "[SRCFILE:COVERFORMULATION.CPP::SOLVE()]Error code: "<< e.getErrorCode() << "\n";
        std::cout << e.getMessage() << "\n";
    }
    catch (...) {
        std::cout << "Other error during optimization.\n";
    }
    return solution;
}