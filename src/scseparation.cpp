#include "../inc/scseparation.h"

SCSeparation::SCSeparation(std::vector<GRBVar>& the_z_bar, SCBGraph* the_G) {
    m = the_G->m;
    n = the_G->n; 
    r = the_G->r;
    z_bar = the_z_bar;
    lazyCuts = 0;

    cover = CoverFormulation(the_G);
}

void SCSeparation::callback(){
    if (where == GRB_CB_MIPSOL) {      
        // set-covering constraints stay the same
        // blocking constraints: loop through zbar - change upper bounds on x (0,1) based on zbar, make sure you have both and if and an else   
        clock_t begin = clock();
        double *z_bar_now = new double [m];
        for (int k = 0; k < m; k++){
            z_bar_now[k] = getSolution(z_bar[k]);
        }
        cover.set_bounds(z_bar_now);

        delete z_bar_now;
        
        // solve follower set-covering model
        x_bar = cover.solve();
        std::cout << "Current best set-cover: cardinality = " << x_bar[0] << "\n";
        
        for (int k = 1; k < m + 1; k++){
            std::cout << x_bar[k] << "\n";
        }

        // is optimal follower solution < r? 
        if (x_bar[0] < r){
            // reset LinExpr object, new cut cardinality element
            new_cut = 0;
            temp_cut_cardinality = 0;
            // loop through subsets and add to cut the ones that are in the optimal covering sol
            for (int k = 1; k < m + 1; k++){
                if (x_bar[k] > 0.5) {
                    // adjust the index (k-1) because the vector x_bar has the obj_val in index 0
                    new_cut += z_bar[k-1];
                    temp_cut_cardinality ++;
                }  
            }
            cut_cardinality.push_back(temp_cut_cardinality);
            // add new_cut as a lazy constraint, to be greater than 1
            addLazy(new_cut >= 1);
            std::cout << "\nAdded lazy cut: " << new_cut << " >= " << "1\n";
            std::cout << "-----------------------------------------------\n";
            lazyCuts ++;
        }
        else {
            std::cout << "No lazy cut to add, set-cover >= r\n";
        }
        float time_diff = float(clock() - begin) / CLOCKS_PER_SEC;
        separation_times.push_back(time_diff);
    }
}