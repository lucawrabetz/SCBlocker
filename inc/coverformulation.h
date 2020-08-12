#include "graph.h"
#include "/Library/gurobi902/mac64/include/gurobi_c++.h"

/* we define the CoverFormulation class containing all relevant objects to exist within the lifetime of the SCB algorithm, 
   solving the set-covering instance given by current blocking variables when called upon by the callback object class, SCSeparation
   Contains:
        - gurobi environment and model for the cover model

        - lightweight ints n and m

        - vectors of decision variables x and y (y is the penalty decision to not cover an element)

        - functions: constructor, set_bounds(), solve
*/

class CoverFormulation{
private:
    GRBEnv* covenv;
    GRBModel* covmodel;

    int n;
    int m;
    int counter = 0; 
    std::string model_name = "cover_model" + std::to_string(counter) + ".lp";

    std::vector<GRBVar> x; 
    std::vector<GRBVar> y;

public:
    CoverFormulation();
    CoverFormulation(SCBGraph* G);
    void set_bounds(double* z_bar_now);
    std::vector<double> solve();
};