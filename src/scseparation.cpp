#include "../inc/scseparation.h"

SCSeparation::SCSeparation(std::vector<GRBVar>& the_z_bar, SCBGraph* the_G, int the_r) {
    m = the_G->m;
    n = the_G->n; 
    r = the_r;
    z_bar = the_z_bar;
    lazyCuts = 0;

    cover = CoverFormulation(the_G);
}

void SCSeparation::callback(){
    if (where == GRB_CB_MIPSOL) {      
        // set-covering constraints stay the same
        // blocking constraints: loop through zbar - change upper bounds on x (0,1) based on zbar, make sure you have both and if and an else   
        double *z_bar_now = new double [m];
        for (int k = 0; k < m; k++){
            z_bar_now[k] = getSolution(z_bar[k]);
        }
        cover.set_bounds(z_bar_now);

        delete z_bar_now;
        
        // solve follower set-covering model
        x_bar = cover.solve();

        // is optimal follower solution < r? 
        if (x_bar[0] < r){
            // reset LinExpr object to 0
            new_cut = 0;

            // loop through subsets and add to cut the ones that are in the optimal covering sol
            for (int k = 1; k < m + 1; k++){
                if (x_bar[k] > 0.5) {
                    new_cut += z_bar[k-1];
                }  
            }
            
            // add new_cut as a lazy constraint, to be greater than 1
            addLazy(new_cut >= 1);
            std::cout << "\nAdded lazy cut: " << new_cut << " >= " << "1\n";
            std::cout << "-----------------------------------------------\n";
        }
    }
}