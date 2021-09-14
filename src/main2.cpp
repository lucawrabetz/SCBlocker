#include "../inc/coverformulation.h"
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

        const std::string datafile = "/Users/lucawrabetz/Dropbox/SCB/SCBlocker/dat/example1.graph";
        const std::string outputfile = "rvalue.txt";
        // const std::string datafile = argv[1];
        // const std::string outputfile = argv[2];
        // const int max_gamma = std::stoi(argv[3]);
        SCBGraph* G = new SCBGraph(datafile);
        CoverFormulation SC = CoverFormulation(G);

        std::vector<double> sol;
        sol = SC.solve();

        std::ofstream myfile;
        myfile.open(outputfile, std::ofstream::trunc);
        myfile << dbl_str(sol[0]);
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
