#include "../inc/coverformulation.h"

int main() {
    setenv("GRB_LICENSE_FILE", "/Users/lucawrabetz/gurobi.lic", 1);
    
    try {
        const std::string datafile = "dat/example1.graph";
        const SCBGraph G = SCBGraph(datafile);
        
        CoverFormulation test = CoverFormulation(G);
        
        std::vector<double> sol;
        sol = test.solve();

        std::cout << "Obj: " << sol[0] << "\n";

        for (int k = 1; k < G.m+1; k++) {
            std::cout << "x_" + std::to_string(k-1) << ": " << sol[k] << "\n";
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
