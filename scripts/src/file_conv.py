from SCB_graph import SCB_Graph
import os

def csv_graph(file, n, t, r):
    G = SCB_Graph()
    G.read_csv(file, n, t, r)
    solutions = G.enum_SC_solutions()
    return G, solutions

def list_to_constr(list):
    strings = []
    RHS = "<= -1\n"
    for i in list: 
        strings.append("-x{} ".format(i+1))
    LHS = ''.join(strings)
    full_constr = LHS + RHS
    return full_constr

def binary_constr(n):
    lower_list = []
    upper_list = []
    for i in range(n):
        lower_list.append('x{} >= 0\n'.format(i+1))
        upper_list.append('x{} <= 1\n'.format(i+1))
    lower_string = ''.join(lower_list)
    upper_string = ''.join(upper_list)
    full_set = lower_string + upper_string
    return full_set

def graph_porta(file, n, t, r, outputpath, name):
    G, solutions = csv_graph(file, n, t, r)
    print(solutions)
    path = os.path.join(outputpath, name)
    with open(path, 'w') as f:
        f.write('DIM = {}\n'.format(G.n))
        f.write('VALID\n')
        for i in range(G.n):
            f.write('1 ')
        f.write('\n')
        f.write('\n')
        # f.write('LOWER BOUNDS\n')
        # for i in range(G.n):
        #     f.write('0 ')
        # f.write('\n')
        # f.write('UPPER BOUNDS\n')
        # for i in range(G.n):
        #     f.write('1 ')
        f.write('\n')
        f.write('\n')
        f.write('\n')
        f.write('\n')
        f.write('INEQUALITIES_SECTION\n')
        for solution in solutions:
            f.write(list_to_constr(solution))
        f.write(binary_constr(n))
        f.write('END\n')

graph_porta('../dat/instance2.csv', 4, 4, 3, '../dat/', 'test.ieq')
