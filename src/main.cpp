#include "../inc/scbformulation.h"
#include <iostream> 
#include <fstream>
#include <sstream>

std::string flt_str(float in) {
        std::ostringstream ss;
        ss << in;
        std::string s(ss.str());
        return s;
}

std::string dbl_str(double in) {
        std::ostringstream ss;
        ss << in;
        std::string s(ss.str());
        return s;
}

int main(int argc, char* argv[]) {
    setenv("GRB_LICENSE_FILE", "/Users/lucawrabetz/gurobi.lic", 1);
    
    try {
        const clock_t begin = clock();
        const std::string datafile = "/Users/lucawrabetz/Dropbox/SCB/SCBlocker/dat/example1.graph";
        const std::string outputfile = "test.txt";
        const int max_gamma = 1;
        // const std::string datafile = argv[1];
        // const std::string outputfile = argv[2];
        // const int max_gamma = std::stoi(argv[3]);
        SCBGraph* G = new SCBGraph(datafile);
        SCBFormulation SCB = SCBFormulation(G, max_gamma);

        std::vector<double> sol;
        sol = SCB.solve();

        if (sol.size() > 0) {
            std::cout << "Obj: " << sol[0] << "\n";

            for (int k = 1; k < G->m+1; k++) {
                // adjust the index (k-1) because the vector sol has the obj_val in index 0
                std::cout << "z_" + std::to_string(k-1) << ": " << sol[k] << "\n";
            }
        }
        std::ofstream myfile;
        myfile.open(outputfile);

        std::string lazy_cuts;
        for (int gamma = 0; gamma <= max_gamma; gamma++){
            lazy_cuts += flt_str(SCB.lazy_cuts[gamma]) + ",";
        }
        std::string cut_density_avg = flt_str(SCB.cut_density_avg);
        std::string avg_sep_time = flt_str(SCB.avg_sep_time);
        std::string running_time = flt_str(SCB.running_time);
        std::string gap = dbl_str(SCB.gap);

        std::string outputfile_string = lazy_cuts + cut_density_avg + "," + avg_sep_time + "," + running_time + "," + gap;
        myfile << outputfile_string;
        myfile.close();
    }
    catch (GRBException& ex) {
        std::cout << "[SRCFILE:MAIN.CPP]Error code: "<< ex.getErrorCode() << "\n";
        std::cout << ex.getMessage() << "\n";
    }
    catch (...) {
        std::cout << "Other error during optimization.\n";
    }
}
