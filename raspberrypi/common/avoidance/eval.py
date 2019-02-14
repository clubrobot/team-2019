from math import hypot 

derive_weight = 1
it_weight = 1/200
dist_weight = 1

def eval(path, target, it):
    derive = 0
    for i in range(len(path)-1):
        derive += (hypot(path[i][0] - target[0],path[i][1] - target[1])-hypot(path[i+1][0] - target[0],path[i+1][1] - target[1]))/it

    return  it_weight*it + hypot(path[-1][0]-target[0],path[-1][0]-target[0])*dist_weight + derive_weight*derive
