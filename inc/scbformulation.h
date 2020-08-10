#pragma once

#include "scseparation.h"

/* we define the SCBFormulation class containing all relevant objects to solve an instance
   of the set-cover-blocker problem: 
        - the gurobi environment and model for itself

        - lightweight integer values n, m, r assigned based on the graph to be accessible 

        - a vector of gurobi variable objects for the solution

        - an SCSeparation object for the call back (contains pointer to z, x_bar, new constraint object, ints, coverformulation)
        - functions: constructor, solve, printSol()
*/

class SCBFormulation{
private:
    GRBEnv* scbenv;
    GRBModel* scbmodel;

    SCBGraph& G;

    int n;
    int m;
    int r;

    std::vector<GRBVar> z;
    SCSeparation sep;

public:  
    SCBFormulation(SCBGraph& G, int the_r);
    void solve();
    void printSol();
};
