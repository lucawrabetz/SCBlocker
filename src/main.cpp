#include "../inc/scbformulation.h"

int main() {
    setenv("GRB_LICENSE_FILE", "/Users/lucawrabetz/gurobi.lic", 1);
    
    try {
        const std::string datafile = "dat/example2.graph";
        SCBGraph* G = new SCBGraph(datafile);
        SCBFormulation SCB = SCBFormulation(G, 2);

        std::vector<double> sol;
        sol = SCB.solve();

        if (sol.size() > 0) {
            std::cout << "Obj: " << sol[0] << "\n";

            for (int k = 1; k < G->m+1; k++) {
                // adjust the index (k-1) because the vector sol has the obj_val in index 0
                std::cout << "z_" + std::to_string(k-1) << ": " << sol[k] << "\n";
            }
        }
    }
    catch (GRBException& ex) {
        std::cout << "[SRCFILE:MAIN.CPP]Error code: "<< ex.getErrorCode() << "\n";
        std::cout << ex.getMessage() << "\n";
    }
    catch (...) {
        std::cout << "Other error during optimization.\n";
    }
}
