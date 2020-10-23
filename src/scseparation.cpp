#include "../inc/scseparation.h"

bool check_gammarobust(int gamma, std::vector<int> &covered, int n) {
    // loop through covered vector until you find a value < gamma + 1; return false
    // if you don't find one, return true 
    for (int i = 0; i<n; i++){
        if (covered[i]<gamma+1){
            return false;
        }
    }
    return true; 
}

void covered_increment(bool up, int subset, std::vector<int> &covered, SCBGraph* G){
    // add or remove (up = true = add) one to covered number of all elements covered by subset
    for (int i = G->sn_indexes[subset]; i < G->sn_indexes[subset+1]; i++){
        if (up) {
            covered[G->s_neighbors[i]] ++;
        }
        else {
            covered[G->s_neighbors[i]] --;
        }
    }
}

std::vector<double> lift_cover(int gamma, std::vector<double> &x_bar, std::vector<int> &covered, bool &success, SCBGraph* G, int &final_gamma){
    // loop through all instance subsets
    bool robust;
    for (int k = 0; k<G->m; k++){
        if (x_bar[k] < 0.5){
            // only enter this sequence for subsets that aren't in the cover
            // increment the covered vector by this k
            covered_increment(true, k, covered, G);
            // check if this cover is gamma robust
            robust = check_gammarobust(gamma, covered, G->n);
            if (robust){
                // add cover to x_bar if robust and return new x_bar
                x_bar[k] = 1;
                final_gamma++;
                success = true; 
                return x_bar;
            }
            else {
                // revert covered vector
                covered_increment(false, k, covered, G);
            }
        }
    }
    success = false;
    return x_bar;
}

SCSeparation::SCSeparation(std::vector<GRBVar>& the_z_bar, SCBGraph* the_G, const int the_max_gamma) {
    // pre process to lower max_gamma if you can prove global upper bound on gamma robustness
    m = the_G->m;
    n = the_G->n; 
    r = the_G->r;
    G = the_G;
    for (int i = 0; i < n; i++){
        covered.push_back(0);
    }
    int min = 0; 
    for (int k = 1; k < m + 1; k++){
        // construct covered vector (considering the set-cover of all subsets)
        // if subset k is in this set-cover, loop through neighbors and add 1 to covered vector 
        for (int v = G->sn_indexes[k-1]; v < G->sn_indexes[k]; v++){
            covered[G->s_neighbors[v]]++;
        }
    } 
    // if the set-covering solution of the entire subset has a minimum covered value of less than max gamma, we can lower max_gamma to this value 
    // this is because we will never find a higher gamma robust cover by definition 
    min = *std::min_element(covered.begin(), covered.end());
    max_gamma = std::min(min-1, the_max_gamma);
    z_bar = the_z_bar;
    // lazyCuts is a vector, count lazycuts added for each gamma (lazyCuts[0], number of criticals, lazyCuts[1], number of 1-robusts, etc)
    for (int gamma = 0; gamma <= the_max_gamma; gamma++){
        lazyCuts.push_back(0);
    }
    cover = CoverFormulation(the_G);
}

void SCSeparation::callback(){
    if (where == GRB_CB_MIPSOL) {      
        // set-covering constraints stay the same
        // blocking constraints: loop through zbar - change upper bounds on x (0,1) based on zbar, make sure you have both an if and an else   
        clock_t begin = clock();
        double *z_bar_now = new double [m];
        for (int k = 0; k < m; k++){
            z_bar_now[k] = getSolution(z_bar[k]);
        }
        cover.set_bounds(z_bar_now);
        
        for (int i = 0; i < n; i++) {
            covered[i] = 0;
        }

        // solve follower set-covering model
        x_bar = cover.solve();
        std::cout << "Current best set-cover: cardinality = " << x_bar[0] << "\n";
        // for (int k = 1; k < m+1; k++){
        //     std::cout << x_bar[k] << "\n";
        // }

        for (int k = 1; k < m + 1; k++){
            // construct covered vector
            if (x_bar[k] > 0.5) {
                // if subset k is in this set-cover, loop through neighbors and add 1 to covered vector 
                for (int v = G->sn_indexes[k-1]; v < G->sn_indexes[k]; v++){
                    covered[G->s_neighbors[v]]++;
                }
            }
        }

        // is optimal follower solution < r? 
        if (x_bar[0] < r){
            // reset LinExpr object, new cut cardinality element
            x_bar.erase(x_bar.begin());
            int final_gamma = 0;
            if (max_gamma > 0) {
                int gamma = 1;
                bool success = true;
                while (success && (gamma <= max_gamma)){
                    // lifting procedure
                    // check gamma robustness of current x_bar
                    x_bar = lift_cover(gamma, x_bar, covered, success, G, final_gamma);
                    gamma++;
                }
            }
            new_cut = 0;
            temp_cut_cardinality = 0;
            // loop through subsets and add to cut the ones that are in the optimal covering sol
            for (int k = 0; k < m; k++){
                if (x_bar[k] > 0.5) {
                    new_cut += z_bar[k];
                    temp_cut_cardinality ++;
                }  
            }
            cut_cardinality.push_back(temp_cut_cardinality);
            // add new_cut as a lazy constraint, to be greater than 1
            addLazy(new_cut >= final_gamma+1);
            std::cout << "\nAdded lazy cut: " << new_cut << " >= " << final_gamma+1 << "\n";
            std::cout << "-----------------------------------------------\n";
            lazyCuts[final_gamma]++;
        }
        else {
            std::cout << "No lazy cut to add, set-cover >= r\n";
        }
        float time_diff = float(clock() - begin) / CLOCKS_PER_SEC;
        separation_times.push_back(time_diff);
    }
}