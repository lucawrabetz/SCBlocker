#pragma once

#include <time.h>
#include "coverformulation.h"

/* we define the SCSeparation class containing all relevant objects to generate cuts to the
   SCBFormulation model:
        - z_bar (vector* of GRBVar so we can see current decision variables of SCB), and x_bar vector of doubles
        - GRBLinExpr object new_cut to construct the new constraint to be added after we solve CoverFormulation

        - lightweight ints m, n, r, lazyCuts
        - CoverFormulation object (containing )

        - functions: constructor(s), callback, printSol()
*/

class SCSeparation : public GRBCallback {
public:
    std::vector<GRBVar> z_bar;
    std::vector<double> x_bar;
    GRBLinExpr new_cut; 

    int max_gamma;
    int m; 
    int n;
    int r; 
    int temp_cut_cardinality;
    std::vector<int> lazyCuts; 
    std::vector<int> cut_cardinality;
    std::vector<float> separation_times;
    CoverFormulation cover;

    std::vector<int> covered; 
    SCBGraph* G;

    SCSeparation(){};
    SCSeparation(std::vector<GRBVar>& the_z_bar, SCBGraph* the_G, int the_max_gamma);
    
    float cut_density = 0;
    float avg_separation_time = 0;
protected:
    // bool SCSeparation::check_gammarobust(int gamma);
    // void SCSeparation::covered_increment(bool up, int subset);
    void callback();
    void printSeparation();
};