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

    std::vector<GRBVar> x; 
    std::vector<GRBVar> y;

public:
    CoverFormulation(const SCBGraph& G);
    void set_bounds(std::vector<GRBVar>& z_bar);
    std::vector<double> solve();
};