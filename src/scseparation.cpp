#include "scseparation.h"


SCSeparation::SCSeparation() {
    m = 0;
    n = 0;
    r = 0;
    lazyCuts = 0;
}

SCSeparation::SCSeparation(std::vector<GRBVar>& the_z_bar, const SCBGraph&& the_G, int the_r) {
    m = the_G.m;
    n = the_G.n; 
    r = the_r;
    z_bar = the_z_bar;
    lazyCuts = 0;

    cover = CoverFormulation(&G);
}

void SCSeparation::callback(){
    if (where == GRB_CB_MIPSOL) {      
        // set-covering constraints stay the same
        // blocking constraints: loop through zbar - change upper bounds on x (0,1) based on zbar, make sure you have both and if and an else   
        cover.set_bounds(&z_bar);
        
        // solve follower set-covering model
        x_bar = cover.solve()

        // is optimal follower solution < r? 
        if (x_bar[0] < r){
            // reset LinExpr object to 0
            new_cut = 0;

            // loop through subsets and add to cut the ones that are in the optimal covering sol
            for (int k = 1; int k < m+1; k++){
                if x_bar[k] > 0.5 {
                    new_cut += z_bar[k];
                }  
            }
            
            // add new_cut as a lazy constraint, to be greater than 1
            addLazy(new_cut >= 1);
            std::cout << "\nAdded lazy cut: " << new_cut << " >= " << "1\n"
            std::cout << "-----------------------------------------------\n"
        }
    }
}