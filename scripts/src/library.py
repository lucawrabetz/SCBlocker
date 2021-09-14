from itertools import chain, combinations

def powerset(set):
    x = len(set)
    powerset = []
    for i in range(1 << x):
        powerset.append([set[j] for j in range(x) if (i & (1 << j))])
    return powerset

def isValidCover(G, Stemp):
    total = set()
    for i in Stemp:
        total = total | set(G.ADJ_1[i])
    total = list(total)
    if len(total) == G.t:
        return True
    return False
