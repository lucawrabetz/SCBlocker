#pragma once

#include "gurobi_c++.h"
#include "graph.h"

class SCBSeparation : public GRBCallback {
public:
    GRBVar *z;
    std::vector<double> x_bar;
    SCBGraph *G;
    int m; 
    int n;
    int r; 
    int lazyCuts; 
    SCBSeparation(GRBVar *z_vars, const std::vector<double> &x_bar, SCBGraph &Graph, int r_var);

protected:
    void callback();
    void printSol(double *z_bar, double *x_bar, double *y_bar);

};