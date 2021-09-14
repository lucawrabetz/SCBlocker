from random import randint
from random import sample
import numpy as np 
import math
import os

'''
ONE LIST IN DATA = []
["U"] - values of n
["S"] - values of m
["|S|"] - values of |S| (mean in rand normal) 
["c~"] - (variance multiplier for costs) types of instance 
["r"] - (% increment from optimal SC) values of r
'''

DATA = {
    "U": [25, 75],
    "S": [10],
    "|S|": [],
    "c~": [0],
    "r": [1.1]
}

'''
PATHS AND FILES
1. Set an experiment name in var EXPERIMENT, preferebly date with underscores
2. Ensure directories inputs/<EXPERIMENT>, outputs/LEFT_<EXPERIMENT>, outputs/RIGHT_<EXPERIMENT>, outputs/SHELL_<EXPERIMENT> exist
'''

EXPERIMENT = "OCT_24_2020/"
MAX_GAMMA = 0

INPUT_DIRECTORY = "inputs/"
OUTPUT_DIRECTORY = "outputs/"
SCB_DIRECTORY = "../"
RUN_DIRECTORY = os.path.join(SCB_DIRECTORY, EXPERIMENT)

LEFT_TABLE = os.path.join(OUTPUT_DIRECTORY, "LEFT_" + EXPERIMENT)
RIGHT_TABLE = os.path.join(OUTPUT_DIRECTORY, "RIGHT_" + EXPERIMENT)
SHELL_TABLE = os.path.join(RUN_DIRECTORY, "SHELL_" + EXPERIMENT)

def prepend_line(file_name, line):
    """ Insert given string as a new line at the beginning of a file """
    # define name of temporary dummy file
    dummy_file = file_name + '.bak'
    # open original file in read mode and dummy file in write mode
    with open(file_name, 'r') as read_obj, open(dummy_file, 'w') as write_obj:
        # Write given line to the dummy file
        write_obj.write(line + '\n')
        # Read lines from original file one by one and append them to the dummy file
        for line in read_obj:
            write_obj.write(line)
    # remove original file
    os.remove(file_name)
    # Rename dummy file as the original file
    os.rename(dummy_file, file_name)

def create_instance(experiment_name, n, m, Sd, cvar, rinc):
    # create output file
    outputpath = os.path.join(INPUT_DIRECTORY, "{}-{}-{}.graph".format(experiment_name, n, m, ))
    file = open(outputpath, 'w')
    # calculate parameters for blocks
    n = int(round(m*ratio1))

    # calculate r by solving SCModel to optimality and taking floor
    r = None

    ### WRITING LINE 3
    # calculate penalty cost on elements of U and write line 3
    k = []
    for i in range(n):
        k.append(randint(200, 400))
    k = ' '.join(map(str, k))
    file.write("{}\n".format(k))

    ### WRITING LINES 4 -
    #define and write edges
    edges_created = 0
    target_list0 = list(range(n)) # list of i \in 0,...,n-1
    covered = [0 for i in range(n)] # start covered at 0 for every element
    subset_card = [round(i) for i in list(np.random.normal(Sd, Sd*0.2, m))] # random normal sampling of subset sizes based on Sd 
    
    for k in range(m):
        covering = sample(list(range(n)), subset_card[k])
        for j in covering:
            file.write("{} {}\n".format(i, j))
            covered[j] += 1
        edges_created += subset_card[k]
    
    c = [0 for i in range(m)]

    for i in range(m):
        c[i] = sum([randint(1, (1 + 2*cvar)) for j in subset_card[i]])

    file.close()

    ### WRITING LINE 2
    # write line 2 (subset cost)
    c = " ".join([str(j) for j in c])
    prepend_line(outputfile, "{}".format(c))

    ### WRITING LINE 1
    # write line 1
    line1 = "{} {} {} {}".format(m, n, edges_created, r)
    prepend_line(outputfile, "{}".format(line1))

    # return strings directly as first 5 columns of output table
    if cvar == 0:
        inst_type = "A"
    elif cvar == 1:
        inst_type = "B"
    else: 
        inst_type = "C"
    
    return [str(n), str(m), str(Sd), str(round(edges_created/n)), inst_type]

def generate(data, name, max_gamma):
    # for i in range(MAX_GAMMA+1):
    #     # for each value of gamma 
    #     left_table_path = os.path.join(OUTPUT_DIRECTORY, LEFT_TABLE, "lefttable{}{}.txt".format(name, "G"+str(i)))
    #     file = open(left_table_path, 'w')
    #     shell_path = os.path.join(OUTPUT_DIRECTORY, SHELL_TABLE, "{}{}.sh".format(name, "G"+str(i)))
    #     file2 = open(, 'w')
        # for n in data["U"]:
        #     # for each possible universe size (n)
        #     for m in data["S"]:
        #         # for each possible number of subsets (m)
        #         for Sd in data["|S|"]:
        #             # for each possible subset density
        #             for cvar in data["c~"]:
        #                 # for each type of instance (cost variance)
        #                 for rincd in data["r"]:
        #                     # for each increment of r of the optimal set cover (%)


    
    call = "bin/main/" + " dat/informs2020/instance" + str(block) + str(index) + ".graph dat/informs2020/outputfiles/output" + str(block) + str(index) 
    file2.write(call+"\n")  
    new_line = create_instance(m, block, index, ratio1, ratio2, ratio3) 
    new_line = ",".join(new_line)+"\n"
    file.write(new_line)   
    ratio1 -= 1
    ratio2 += .1
    ratio3 += .05


generate(data, EXPERIMENT)
