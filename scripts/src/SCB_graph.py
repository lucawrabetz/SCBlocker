from random import randint
from csv import reader
from library import powerset
from library import isValidCover

class SCB_Graph:
    # liftedconstraints
    lifted_constr = 0
    # subsets
    n = 0
    # elements
    t = 0
    # edges (undirected)
    m = 0
    # minimum set cover
    r = 0
    # cost to block subsets
    c = []
    # penalty cost for no cover
    k = []
    # adjacency list (subsets)
    ADJ_1 = []
    # adjacency list (elements)
    ADJ_2 = []
    # maximum cost
    maxC = 0
    # inner model
    # SC_model = Model("Set Cover")
    # set cover variable
    x = {}
    # no cover penalty variable
    y = {}

    # read graph
    def read(self, inputfile):
        f = open(inputfile, 'r')

        # read line 1 to assign parameter values
        line = f.readline()
        fields = str.split(line)
        self.n = int(fields[0])
        self.t = int(fields[1])
        self.m = int(fields[2])
        self.r = int(fields[3])
        # read line 2 to fill cost list
        line = f.readline()
        fields = str.split(line)
        for i in range(self.n):
            self.c.append(int(fields[i]))
        # read line 3 to fill penalty cost list
        line = f.readline()
        fields = str.split(line)
        for j in range(self.t):
            self.k.append(int(fields[j]))
        # read rest of lines to fill edges (adjacency)
        for i in range(self.n):
            self.ADJ_1.append([])
        for j in range(self.t):
            self.ADJ_2.append([])
        for line in f:
            fields = line.split(' ')
            i = int(fields[0])
            j = int(fields[1])
            self.ADJ_1[i].append(j)
            self.ADJ_2[j].append(i)

        f.close
        self.maxC = max(self.c)
        self.maxC = self.maxC*self.n

    def read_csv(self, inputfile, n, t, r):
        # initialize parameters
        self.n = n
        self.t = t
        self.m = 0 # will update
        self.r = r
        # create costs with random numbers
        for i in range(self.n):
            self.c.append(randint(50, 150))
            self.ADJ_1.append([])
        for i in range(self.t): 
            self.k.append(randint(850, 1250))
            self.ADJ_2.append([])
        # read csv file adjacency matrix to fill out class
        with open(inputfile) as file:
            csv_reader = reader(file)
            subset_counter = 0
            for subset in csv_reader:
                element_counter = 0
                for element in subset:
                    if element == '1':
                        self.ADJ_1[subset_counter].append(element_counter)
                        self.ADJ_2[element_counter].append(subset_counter)
                        self.m += 1
                    element_counter += 1
                subset_counter += 1
        self.maxC = max(self.c)
        self.maxC = self.maxC*self.n
                
    def printG(self):
        print('n = %d' % self.n)
        print('t = %d' % self.t)
        print('m = %d' % self.m)
        print('r = %d' % self.r)
        print('c = {}'.format(self.c))

        print("\nAdjacency, subsets: \n")
        for i in range(self.n):
            print(self.ADJ_1[i])
        print("\nAdjacency, elements: \n")
        for j in range(self.t):
            print(self.ADJ_2[j])

        print('\nM = %d' % self.maxC)

    def enum_SC_solutions(self):
        all_sols = powerset([i for i in range(self.n)])
        del all_sols[0]
        sols = []
        for sol in all_sols:
            if len(sol) < self.r:
                sols.append(sol)
        # print("r = {}".format(self.r))
        valid_sols = []
        for sol in sols:
            if isValidCover(self, sol):
                valid_sols.append(sol)
        return valid_sols


